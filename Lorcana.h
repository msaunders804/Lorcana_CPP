#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/img_hash.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h>
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;


//Menu Functions
void DisplayMenu();
void ExecuteChoice(int choice);

//Image manipulation functions
void TakeImage();
void ImageManipulation(Mat& frame);
void ExtractCardImage(Mat& image_copy, const vector<std::vector<cv::Point>> contours);


//TrainingPrep functions
void GeneratedDistorted(list<string> set_num_list, const string path, int num_distortions);
list<string> GenerateIdList();
Mat RandomDistortion(string path);
Mat DistortColor();
Mat DistortBlur();
Mat DistortContrast();
Mat DistortFlip();
Mat DistortSharpness();
pair<vector<Mat>, vector<int>> GenLabeledArrays(const string path, int batchSize, const string type);
void saveArrays(string& filename, vector<Mat>& imageArray, vector<int>& labelArray);

//Training functions
pair<vector<Mat>, vector<int>> loadArrays();
