#include < stdio.h>  
#include < opencv2\opencv.hpp>  
#include <NuiApi.h>
//#include < opencv2\gpu\gpu.hpp>  

#define ROIWIDTH 4*20
#define ROIHEIGHT 3*20

USHORT s;
USHORT realdepth = s & 0xfff8 >> 3;

using namespace cv;
using namespace std;

void Classify(Mat img)
{
	CvSVM svm;
	svm.load("trainedSVM.xml");

		Mat img_gray;

		//gray  
		cvtColor(img, img_gray, CV_RGB2GRAY);
		

		for (int i = 0; ((ROIHEIGHT / 3 * i) + (ROIHEIGHT) )<= img.rows; i++)
		{
			for (int j = 0;( (ROIWIDTH/3 * j) + (ROIWIDTH) )<= img.cols; j++)
			{
				Rect ROI;
				ROI.x = ROIWIDTH / 3 * j;
				ROI.y = ROIHEIGHT / 3 * i;
				ROI.width = ROIWIDTH;
				ROI.height = ROIHEIGHT;

				imshow("origin", img);

				//Extract HogFeature  
				HOGDescriptor d(Size(32, 16), Size(8, 8), Size(4, 4), Size(4, 4), 9);
				vector< float> descriptorsValues;
				vector< Point> locations;
				d.compute(img_gray(ROI), descriptorsValues, Size(0, 0), Size(0, 0), locations);
				//vector to Mat  
				Mat fm = Mat(descriptorsValues);

				//Classification whether data is positive or negative  
				int result = svm.predict(fm);
				
				
				if (result == 1) { rectangle(img_gray, ROI, Scalar(0, 0, 255)); NuiTransformDepthImageToSkeleton(ROI.x,ROI.y,realdepth,NUI_IMAGE_RESOLUTION_640x480); }

				
			}
		}

		//show image  
		imshow("origin", img_gray);
	
}