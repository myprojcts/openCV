#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;


//shapes detection learning

void getContours(Mat imgdil, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	string objType;

	findContours(imgdil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(0, 255, 0), 3);

	for (int i = 0; i != (contours.size()); ++i) {
		int area = contourArea(contours[i]);
		//cout << area << endl;

		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());

		if (area > 150) {  //фильтрация по размеру области (чтобы не выделялся шум)


			double perimeter = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * perimeter, true);
			//drawContours(img, conPoly, i, Scalar(0, 255, 0), 2);
			//cout << i+1 << "Number of angles: " << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			int objCor = (int)conPoly[i].size();

			if (objCor == 3) {
				objType = "Triangle";
			}

			if (objCor == 4)
			{
				double aspRatio = (double)boundRect[i].width / (double)boundRect[i].height;
				cout << aspRatio << endl;
				if (aspRatio > 0.95 && aspRatio < 1.05) {
					objType = "Square";
				}
				else {
					objType = "Rectangle";
				}
			}
			if (objCor > 4) {
				objType = "Circle";
			}

			drawContours(img, conPoly, i, Scalar(0, 255, 0), 2);
			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			putText(img, objType, { boundRect[i].x + 10, boundRect[i].y + 10 }, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 0, 0), 1);




		}

	}


}

void main() {

	string path = "Resources/9.avi"; //импотрируем видео из файла с ресурсами//
	VideoCapture cap(path);
	Mat img;
	
	Mat imgblur, imggray, imgcanny, imgdil;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));


	//подготовка изображения


	

	while (true) {

		cap.read(img);

		cvtColor(img, imggray, COLOR_BGR2GRAY);
		GaussianBlur(imggray, imgblur, Size(5, 5), 3, 3);
		Canny(imgblur, imgcanny, 30, 45);
		dilate(imgcanny, imgdil, kernel);

		
		getContours(imgdil, img);

		
		imshow("image", imgdil);
		waitKey(1);
	}
	

	//
	
	//


	
	//imshow("image_Gray", imggray);
	//imshow("image_GB", imgblur);
	//imshow("image_Canny", imgcanny);
	//imshow("image_DIl", imgdil);
	

}