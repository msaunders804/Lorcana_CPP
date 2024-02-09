#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

void DisplayMenu()
{
    cout << "===== Menu =====" << std::endl;
    cout << "1. Look at my picture" << std::endl;
    cout << "2. Training Prep " << std::endl;
    cout << "3. Train/Test" << std::endl;
    cout << "4. Exit" << std::endl;
    cout << "================" << std::endl;
}

void ExecuteChoice(int choice)
{
    switch (choice) {
    case 1:
        //Run Image manipulation
        TakeImage();
        break;
    case 2:
    {
        string path;
        cout << "Enter your storage path: ";
        cin >> path;
        list<string> set_num_list = GenerateIdList();
        string trainingPath = path + "\\training";
        GeneratedDistorted(set_num_list, trainingPath,6);
        auto [trainImageArrray, trainLabelArray] = GenLabeledArrays(trainingPath, 1000, "training");
        string testingPath = path + "\\testing";
        GeneratedDistorted(set_num_list, testingPath,2);
        auto [testImageArrray, testLabelArray] = GenLabeledArrays(testingPath,1000, "testing");
        break; 
    }
    case 3:
        auto [imageArray, labelArray] = loadArrays();
        cout << "Test if load was correct. Size: " << imageArray.size() << " images and " << labelArray.size() << " Labels" << endl;
        for (size_t i = 0; i < imageArray.size(); ++i) {
            cout << "Label: " << labelArray[i] << "\n";
            imshow("Image", imageArray[i]);
            waitKey(0);
        }
    }
}
