#include < stdio.h>  
#include < opencv2\opencv.hpp>  
//#include < opencv2\gpu\gpu.hpp>  

using namespace cv;
using namespace std;

void HogExtraction(int mode)
{
	
	//variables  
	char FullFileName[100];
	char FirstFileName[100];
	char SaveHogDesFileName[100];
	int FileNum;
	

	if (mode == 1)
	{
		sprintf_s(FirstFileName, "Positive Image\\");
		sprintf_s(SaveHogDesFileName, "Positive.xml");
		FileNum = 40;
		
	}
	else
	{
		sprintf_s(FirstFileName, "Negative Image\\");
		sprintf_s(SaveHogDesFileName, "Negative.xml");
		FileNum = 121;
	}
	

	vector< vector < float> > v_descriptorsValues;
	vector< vector < Point> > v_locations;


	for (int i = 0; i< FileNum; ++i)
	{
		sprintf_s(FullFileName, "%s%d.jpg", FirstFileName, i + 1);
		printf("%s\n", FullFileName);

		//read image file  
		Mat img, img_gray;
		img = imread(FullFileName);

		

		//resizing  
	    resize(img, img, Size(4*20,3*20)); //Size(64,48) ); //Size(32*2,16*2)); //Size(80,72) );   
		
		//gray  
		cvtColor(img, img_gray, CV_RGB2GRAY);

		//extract feature  
		HOGDescriptor d(Size(32, 16), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		vector< float> descriptorsValues;
		vector< Point> locations;
		d.compute(img_gray, descriptorsValues, Size(0, 0), Size(0, 0), locations);

		//printf("descriptor number =%d\n", descriptorsValues.size() );  
		v_descriptorsValues.push_back(descriptorsValues);
		v_locations.push_back(locations);
		//show image  
		//imshow("origin", img);

	}

	//refer to this address -> http://feelmare.blogspot.kr/2014/04/the-example-source-code-of-2d-vector.html  
	//save to xml  
	FileStorage hogXml(SaveHogDesFileName, FileStorage::WRITE); //FileStorage::READ  
	//2d vector to Mat  
	int row = v_descriptorsValues.size(), col = v_descriptorsValues[0].size();
	printf("col=%d, row=%d\n", row, col);
	Mat M(row, col, CV_32F);
	//save Mat to XML  
	for (int i = 0; i< row; ++i)
		memcpy(&(M.data[col * i * sizeof(float)]), v_descriptorsValues[i].data(), col*sizeof(float));
	//write xml  
	write(hogXml, "Descriptor_of_images", M);

	//write(hogXml, "Descriptor", v_descriptorsValues );  
	//write(hogXml, "locations", v_locations );  
	hogXml.release();

}