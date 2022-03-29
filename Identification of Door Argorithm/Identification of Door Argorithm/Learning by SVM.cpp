#include < stdio.h>  
#include < opencv2\opencv.hpp>  
//#include < opencv2\gpu\gpu.hpp>  

using namespace cv;
using namespace std;


void SVMTraining()
{

	//Read Hog feature from XML file  
	///////////////////////////////////////////////////////////////////////////  
	printf("1. Feature data xml load\n");
	//create xml to read  
	FileStorage read_PositiveXml("Positive.xml", FileStorage::READ);
	FileStorage read_NegativeXml("Negative.xml", FileStorage::READ);

	//Positive Mat  
	Mat pMat;
	read_PositiveXml["Descriptor_of_images"] >> pMat;
	//Read Row, Cols  
	int pRow, pCol;
	pRow = pMat.rows; pCol = pMat.cols;

	//Negative Mat  
	Mat nMat;
	read_NegativeXml["Descriptor_of_images"] >> nMat;
	//Read Row, Cols  
	int nRow, nCol;
	nRow = nMat.rows; nCol = nMat.cols;

	//Rows, Cols printf  
	printf("   pRow=%d pCol=%d, nRow=%d nCol=%d\n", pRow, pCol, nRow, nCol);
	//release  
	read_PositiveXml.release();
	//release  
	read_NegativeXml.release();
	/////////////////////////////////////////////////////////////////////////////////  

	//Make training data for SVM  
	/////////////////////////////////////////////////////////////////////////////////  
	printf("2. Make training data for SVM\n");
	//descriptor data set  
	Mat PN_Descriptor_mtx(pRow + nRow, pCol, CV_32FC1); //in here pCol and nCol is descriptor number, so two value must be same;  
	memcpy(PN_Descriptor_mtx.data, pMat.data, sizeof(float)* pMat.cols * pMat.rows);
	int startP = sizeof(float)* pMat.cols * pMat.rows;
	memcpy(&(PN_Descriptor_mtx.data[startP]), nMat.data, sizeof(float)* nMat.cols * nMat.rows);
	//data labeling  
	Mat labels(pRow + nRow, 1, CV_32FC1, Scalar(-1.0));
	labels.rowRange(0, pRow) = Scalar(1.0);
	/////////////////////////////////////////////////////////////////////////////////  

	//Set svm parameter  
	/////////////////////////////////////////////////////////////////////////////////  
	printf("4. SVM training\n");
	CvSVM svm;
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 10000, 1e-6);
	/////////////////////////////////////////////////////////////////////////////////  

	//Training  
	/////////////////////////////////////////////////////////////////////////////////  
	svm.train(PN_Descriptor_mtx, labels, Mat(), Mat(), params);

	//Trained data save  
	/////////////////////////////////////////////////////////////////////////////////  
	printf("5. SVM xml save\n");
	svm.save("trainedSVM.xml");

	// FileStorage hogXml("testXML.xml", FileStorage::WRITE); //FileStorage::READ  
	// write(hogXml, "Data", PN_Descriptor_mtx);  
	// write(hogXml, "Label", labels);  
	// hogXml.release();  
}