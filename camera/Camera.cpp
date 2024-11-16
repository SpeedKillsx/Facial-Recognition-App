#include <Python.h>

#include "Camera.hpp"
#include <QImage>
#include <QPixmap>
#include <QMetaObject>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face-recognition/Face.hpp"
Camera::Camera(int deviceID, int width, int height, const std::string& name, QLabel* videoLabel)
    : cap(deviceID), videoLabel(videoLabel), name(name) {
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    // Connect the timer to the frame capture slot
    connect(&timer, &QTimer::timeout, this, &Camera::captureFrame);
}

void Camera::startCamera() {
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return;
    }
    timer.start(30);  // Start capturing frames every 30ms
}

void Camera::stopCamera() {
    timer.stop();
    cap.release();
}

void Camera::captureFrame() {
    cv::Mat frame;
    
    cap.read(frame);
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
        return;
    }
    Face face(frame);
    face.faceDetection();
    face.launchFaceRecognition();
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);  // Convert to RGB
    QImage qImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    emit frameReady(qImage);  // Emit the frame to be used by the main thread
}