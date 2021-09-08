#include "OCVProc.h"

OCVProc::OCVProc(int selectedCameraIndex, wxPanel* surface)
{
	displayImage = wxBitmap();
	streamPanel = surface;
	if (camera.open(selectedCameraIndex, cv::CAP_MSMF))
	{
		cameraRes.width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
		cameraRes.height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
		displayImage = wxBitmap(cameraRes.width, cameraRes.height,24);
	}
}

void OCVProc::StartCameraStream()
{
	cameraStream = std::thread(&OCVProc::previewLoop, this);
}

void OCVProc::StopCameraStream()
{
}

void OCVProc::previewLoop()
{
	cv::Mat gray, gauss, canny;
	while (1)
	{
		auto currTime = std::chrono::high_resolution_clock::now();
		camera >> framePreProc;
		// Convert to grayscale
		cv::cvtColor(framePreProc, gray, cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(gray, gauss, cv::Size(3, 3), 0);
		// Use Canny instead of threshold to catch squares with gradient shading
		cv::Canny(gray, canny, 100, 200);

		wxClientDC dc(streamPanel);
		Helpers::ConvertMatBitmapTowxBitmapMSW(framePreProc, displayImage);
		dc.DrawBitmap(displayImage, wxPoint(0, 0), false);


		if ((std::chrono::high_resolution_clock::now() - currTime) < std::chrono::milliseconds(fpsIntervalMS))
			std::this_thread::sleep_until(currTime + std::chrono::milliseconds(fpsIntervalMS));
	}
}
