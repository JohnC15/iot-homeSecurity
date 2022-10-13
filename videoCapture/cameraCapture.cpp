/*******************************************************************************
 * Copyright (c) 2022 John Choo <john.choo.plano@gmail.com>
 * 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v2.0 which accompany this distribution.
 * 
 * Capture and save video, as mp4 and frame as jpeg image, from external USB camera
 * using OpenCV C++ library.
 *******************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;
using namespace chrono;

const string VIDEO_OUTPUT_PATH = "./output/movies/video.mp4"; 
const string CAMERA_DEVICE = "/dev/video2";
const string IMAGE_DIR = "./output/images";
const int FPS = 3;

int main() {
    cv::VideoCapture capture;  // read from camera
    cv::VideoWriter writer;    // save as mp4
    int codec = cv::VideoWriter::fourcc('M', 'P', '4', 'V');  // use mp4 format
    Size sizeFrame(480,360);  // limit output video to 360p   

    capture.open(CAMERA_DEVICE);  // capture camera @ 480p and 3fps
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    capture.set(cv::CAP_PROP_FPS, FPS);
    
    double dWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "camera width = " << dWidth << ", height = " << dHeight << endl;

    if (!capture.isOpened()) { 
        cerr << "Cannot open camera"; 
        throw std::runtime_error("Camera not found");
    }
    
    writer.open(VIDEO_OUTPUT_PATH, codec, FPS, sizeFrame, true); 
    if (!writer.isOpened()) {
        cerr << "Cannot open video output";
        throw std::runtime_error("Failed to create MP4 file");
    }

    Mat frame, xframe;
    for (unsigned long index=1;;index++)
    {
        capture >> frame;
        if (frame.empty()) {
            cerr << "Failed to capture" << endl;
            break;
        }

        // Optional resize as write video output to lower resolution
        resize(frame,xframe,sizeFrame);        
        writer.write(xframe);  

        // Optionally, display the frame in the created window
        imshow("video", frame);

        // Save frame as jpeg image
        // TODO: Add timestamp
        string path = IMAGE_DIR + "/frame" + std::to_string(index) + ".jpg";
        imwrite(path, frame);

        // Wait for ESC keypressed and exit
        if (cv::waitKey (25) == 27) 
           break;  
    }

    // Cleanup
    capture.release();
    writer.release();
    destroyAllWindows();

    return 0;
}

