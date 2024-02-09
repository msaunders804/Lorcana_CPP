#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

list<string> GenerateIdList() {
	int set;
	cout << "How many sets are there? ";
	cin >> set;
	list<string> set_num_list;
	for (int i = 1; i <= set; i++) {
		for (int j = 1; j < 217; j++) {
			set_num_list.push_back(to_string(i) + '-' + to_string(j));
		}
	}
	return set_num_list;
}

//Function to call distortion i times based on how many we want to generate
void GeneratedDistorted(list<string> set_num_list, const string path, int num_distortions) {
	Mat distorted;
	//Path = folder cards are store; Later in code specific file name is appended
	int images_created = 0;
	int cards_distored = 0;

	if (fs::exists(path)) {
		cout << "Images already created" << endl;
		return;
	}

	for (string id : set_num_list)
	{
		ifstream file;
		cards_distored += 1;
		for (int i = 0; i < num_distortions; i++) {
			file.open(path + "\\" + id + "-" + to_string(i) + ".jpg");
				if (file) { continue; }
				else {
					cout << "Distorted Image Created" << endl;
					images_created += 1;
					distorted = RandomDistortion(path + "\\" + id + ".jpg");
					string full_path = path + "\\" + id + "-" + to_string(i) + ".jpg";
					imwrite(full_path, distorted);
				}
		}
	}
}


//Function to distort color
Mat DistortColor(string path) {
	Mat frame = imread(path, IMREAD_COLOR);
	Mat distorted_image = frame.clone();
	cvtColor(distorted_image, distorted_image, COLOR_BGR2HSV);
	double satFactor = (rand() % 2 == 0) ? 0.5 : 1.5;
	distorted_image.convertTo(distorted_image, -1, satFactor, 0);
	cvtColor(distorted_image, distorted_image, COLOR_HSV2BGR);

	return distorted_image;
}

//Function to blur
Mat DistortBlur(string path) {
	Mat frame = imread(path, IMREAD_COLOR);
	Mat distorted_image = frame.clone();
	GaussianBlur(distorted_image, distorted_image, Size(5, 5), 0);
	return distorted_image;
}

//Function to adjust contrast
Mat DistortContrast(string path) {
	Mat frame = imread(path, IMREAD_COLOR);
	Mat contrastImage = frame.clone();

	double contrastFactor = (rand() % 2 == 0) ? 0.5 : 1.5;
	contrastImage.convertTo(contrastImage, -1, contrastFactor, 0);
	return contrastImage;
}

//Function to flip
Mat DistortFlip(string path) {
	Mat frame = imread(path, IMREAD_COLOR);
	Mat roated_image = frame.clone();
	Size initialSize(frame.cols, frame.rows);

	double scale = 0.95 + static_cast<double>(rand()) / RAND_MAX * .01;
	Size scaledSize(static_cast<int>(initialSize.width * scale), static_cast<int>(initialSize.height * scale));

	Scalar bgColor(rand() % 256, rand() % 256, rand() % 256);
	Mat background = Mat::zeros(initialSize, frame.type());
	background.setTo(bgColor);

	Point centerBox((initialSize.width - scaledSize.width) / 2, (initialSize.height - scaledSize.height) / 2);

	Mat scaledImage = frame.clone();
	resize(scaledImage, scaledImage, scaledSize);

	// Randomly select an angle to skew the image
	int maxAngle = 5;
	int skewAngle = rand() % (2 * maxAngle + 1) - maxAngle;

	// Add the scaled image to our color background
	Mat rotatedImage;
	warpAffine(scaledImage, rotatedImage, cv::getRotationMatrix2D(cv::Point(scaledSize.width / 2, scaledSize.height / 2), skewAngle, 1.0), scaledSize, cv::INTER_LINEAR, cv::BORDER_CONSTANT, bgColor);

	rotatedImage.copyTo(background(Rect(centerBox, scaledSize)));

	if (rand() % 2 == 1) {
		flip(background, background, -1);
	}

	return background;

}

//Function to adjust sharpness
Mat DistortShaprness(string path) {
	Mat frame = imread(path, IMREAD_COLOR);
	Mat SharpImage = frame.clone();
	double sharpFactor = (rand() % 2 == 0) ? 0.5 : 1.5;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5 + sharpFactor, -1, 0, -1, 0);
	filter2D(SharpImage, SharpImage, SharpImage.depth(), kernel);

	return SharpImage;
}

//Function to determine which random type of diststortion will occur
Mat RandomDistortion(string path)
{
	int distortion_type = rand() % 5;
	Mat dis_image;
	switch (distortion_type) {
	case 0:
		dis_image = DistortColor(path);
		break;
	case 1:
		dis_image = DistortBlur(path);
		break;
	case 2:
		dis_image = DistortContrast(path);
		break;
	case 3:
		dis_image = DistortFlip(path);
		break;
	case 4:
		dis_image = DistortShaprness(path);
		break;
	}
	return dis_image;

}

//Function to prep images and labels for training
pair<vector<Mat>, vector<int>> GenLabeledArrays(const string path, int batchSize, string type)
{
	vector<Mat> imageArray;
	vector<int> labelArray;
	int currentBatchSize = 0;
	int batchNum = 1;
	Mat resized;

	for (const auto& entry : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(entry) && entry.path().extension() == ".jpg") {
			Mat image = imread(entry.path().string());
			if (!image.empty()) {
				resize(image, image, Size(image.cols/ 4, image.rows / 4));
				image.convertTo(image, CV_32F);
				image /= 255;

				string labelString = entry.path().stem().string();
				string delim = "-";
				vector<string> tokens;
				size_t start = 0, end = 0;

				while ((end = labelString.find_first_of(delim, start)) != string::npos) {
					if (end != start)
					{
						tokens.push_back(labelString.substr(start, end - start));
					}
					start = end + 1;
				}
				if (start < labelString.size()) {
					tokens.push_back(labelString.substr(start));
				}

				int label = stoi(tokens[0] + tokens[1]);

				imageArray.push_back(image);
				labelArray.push_back(label);

				image.release();

				currentBatchSize++;
				if (currentBatchSize >= batchSize) {
					string file = type + "Arrays" + to_string(batchNum);
					saveArrays(file, imageArray, labelArray);
					batchNum++;
					cout << "Now processing batch " + batchNum << endl;
					if (!imageArray.empty()) {
						imageArray.clear();
					}
					if (!labelArray.empty()) {
						labelArray.clear();
					}
					currentBatchSize = 0;
				}
			}
			image.release();
		}
		else {
			cerr << "Error Loading Image" << entry.path() << endl;
		}
	}

	if (!imageArray.empty()) {
		string ArrayFile = type + "Arrays" + to_string(batchNum);
		cout << "Final BATCH" << endl;
		saveArrays(ArrayFile, imageArray, labelArray);
		if (!imageArray.empty()) {
			imageArray.clear();
		}
		if (!labelArray.empty()) {
			labelArray.clear();
		}
	}
	return { imageArray,labelArray };
}

void saveArrays(string& filename, vector<Mat>& imageArray, vector<int>& labelArray) {
	ofstream file(filename, ios::binary);
	if (file.is_open()) {
		// Save the size of the arrays
		size_t imageSize = imageArray.size();
		size_t labelSize = labelArray.size();

		file.write(reinterpret_cast<const char*>(&imageSize), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&labelSize), sizeof(size_t));

		// Save image data
		for (const auto& image : imageArray) {
			size_t rows = image.rows;
			size_t cols = image.cols;
			file.write(reinterpret_cast<const char*>(&rows), sizeof(size_t));
			file.write(reinterpret_cast<const char*>(&cols), sizeof(size_t));
			file.write(reinterpret_cast<const char*>(image.data), image.total() * image.elemSize());

		}
		// Save label data
		file.write(reinterpret_cast<const char*>(labelArray.data()), labelSize * sizeof(int));

		file.close();
		cout << "Arrays saved to: " << filename << endl;
	}
	else {
		cerr << "Error saving arrays to: " << filename << endl;
	}

}

