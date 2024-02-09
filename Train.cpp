#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

// Function to load image array and label array from a file
pair<vector<Mat>, vector<int>> loadArrays() {
    vector<Mat> imageArray;
    vector<int> labelArray;
    int batchNum = 1;
    string filename = "trainingArrays" + to_string(batchNum);

    while (ifstream(filename, ios::binary).is_open()) {
        ifstream input(filename, ios::binary);
        size_t imageSize, labelSize;
        input.read(reinterpret_cast<char*>(&imageSize), sizeof(size_t));
        input.read(reinterpret_cast<char*>(&labelSize), sizeof(size_t));

        // Read image data
        for (size_t i = 0; i < imageSize; ++i) {
            size_t rows, cols;
            input.read(reinterpret_cast<char*>(&rows), sizeof(size_t));
            input.read(reinterpret_cast<char*>(&cols), sizeof(size_t));
            Mat image;
            image.create(rows, cols, CV_32F);
            input.read(reinterpret_cast<char*>(image.data), image.total() * image.elemSize());
            imageArray.push_back(image);
        }

        // Read label data
        vector<int> tempLabelArray(labelSize);
        input.read(reinterpret_cast<char*>(tempLabelArray.data()), labelSize * sizeof(int));
        labelArray.insert(labelArray.end(), tempLabelArray.begin(), tempLabelArray.end());

        input.close();
        batchNum++;
        filename = "trainingArrays" + to_string(batchNum);
    }

    cout << "Arrays Loaded!";
    return { imageArray, labelArray };
}
