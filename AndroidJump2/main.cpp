#include <iostream>
#include <cstdlib>
#include <opencv2\opencv.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;

int main()
{
	while(true)
	{
		system("Adb\\adb shell screencap -p /sdcard/autojump.png");
		system("Adb\\adb pull /sdcard/autojump.png .");
		Mat src = imread("autojump.png");
		Mat tpSrc = imread("template.bmp");

		Mat rs_src;
		resize(src, rs_src, Size(src.cols / 2, src.rows / 2));

		Mat res;
		matchTemplate(rs_src, tpSrc, res, CV_TM_CCOEFF_NORMED);

		double minVal, maxVal;  
		Point minLoc, maxLoc, matchLoc;  
		minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, Mat() ); 

		matchLoc = maxLoc;

		int center_x = rs_src.cols / 2 + 11;
		int center_y = rs_src.rows / 2 + 10;

		int human_x = matchLoc.x + tpSrc.cols / 2;
		int human_y = matchLoc.y + tpSrc.rows / 2 + 42;

		Mat result = rs_src.clone();
		result.at<Vec3b>(center_y, center_x)[0] = 0;
		result.at<Vec3b>(center_y, center_x)[1] = 0;
		result.at<Vec3b>(center_y, center_x)[2] = 0;

		result.at<Vec3b>(human_y, human_x)[0] = 255;
		result.at<Vec3b>(human_y, human_x)[1] = 255;
		result.at<Vec3b>(human_y, human_x)[2] = 255;

		result.at<Vec3b>(center_y * 2 - human_y, center_x * 2 - human_x)[0] = 0;
		result.at<Vec3b>(center_y * 2 - human_y, center_x * 2 - human_x)[1] = 255;
		result.at<Vec3b>(center_y * 2 - human_y, center_x * 2 - human_x)[2] = 255;

		//cout << center_x << ", " << center_y << " # " << human_x << ", " << human_y << endl;

		double x0 = center_x;
		double y0 = center_y;

		double x1 = human_x;
		double y1 = human_y;

		double k1 = -0.5773;
		double k2 = 0.5773;

		if(x0 < x1)
		{
			k1 = 0.5773;
			k2 = -0.5773;
		}

		double b1 = y0 - k1 * x0;
		double b2 = y1 - k2 * x1;

		double xc = (b2 - b1) / (k1 - k2);
		double yc = k1 * xc + b1;

		result.at<Vec3b>(yc, xc)[0] = 0;
		result.at<Vec3b>(yc, xc)[1] = 0;
		result.at<Vec3b>(yc, xc)[2] = 255;

		imshow("Result", result);
		imwrite("result.bmp", result);
		waitKey(0);

		double dis = pow(double(center_x - human_x), 2)
			+ pow(double(center_y - human_y), 2);
		dis = sqrt(dis);

		int lastTime = 5.5 * dis;
		
		char cmd[100] = "Adb\\adb shell input swipe 320 410 320 410 ";
		char strLast[10] = "";
		itoa(lastTime, strLast, 10);
		strcat(cmd, strLast);
		//system(cmd);

		Sleep(4000);
	}
	return 0;
}