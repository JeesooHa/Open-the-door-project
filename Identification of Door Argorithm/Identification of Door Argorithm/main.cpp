#include "Learning by SVM.h"
#include "HOG feature extraction.h"
#include "Classify.h"

#include <windows.h>
#include <NuiApi.h>
#include <tchar.h>

#define POSITIVE 1
#define NAGATIVE 0

void InitializeKinect();
int createRGBImage(HANDLE h, IplImage* Color);

bool ConFigMode = false;

int _tmain(int argc, _TCHAR* argv[])
{
	if (ConFigMode)
	{
		HogExtraction(POSITIVE);
		HogExtraction(NAGATIVE);

		SVMTraining();
	}

	else
	{
		
		HANDLE colorStreamHandle;
		HANDLE nextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		HRESULT hr;

		InitializeKinect();

		IplImage *Color = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);

		Mat Frame;

		hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, nextColorFrameEvent, &colorStreamHandle);

		if (FAILED(hr))
		{
			cout << "Could not open ImageStream" << endl;
			return hr;
		}

		while (true)
		{
			WaitForSingleObject(nextColorFrameEvent, 1000);

			createRGBImage(colorStreamHandle, Color);

			Frame = cvarrToMat(Color);

			Classify(Frame);


			if (cvWaitKey(10) == 0x001b)
			{
				break;
			}
		}
		
	}
	
	//NuiShutdown();
	return 0;
}

void InitializeKinect()
{
	bool FailToConnect;

	do
	{
		HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR);

		if (FAILED(hr))
		{
			system("cls");
			cout << "\nFailed to Connect!\n\n";
			FailToConnect = true;
			system("PAUSE");
		}
		else
		{
			cout << "\nConnection Established!\n\n";
			FailToConnect = false;
		}

	} while (FailToConnect);

}

int createRGBImage(HANDLE h, IplImage* Color)
{
	const NUI_IMAGE_FRAME *pImageFrame = NULL;

	HRESULT hr = NuiImageStreamGetNextFrame(h, 1000, &pImageFrame);

	if (FAILED(hr))
	{
		cout << "Creating RGB Image Failed\n";
		return -1;
	}

	INuiFrameTexture *pTexture = pImageFrame->pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		BYTE *pBuffer = (BYTE*)LockedRect.pBits;
		cvSetData(Color, pBuffer, LockedRect.Pitch);
		//cvShowImage("Color Image", Color);

	}

	NuiImageStreamReleaseFrame(h, pImageFrame);

	return 0;
}