#ifndef __FACE__
#define __FACE__
#include<string>
#include <Python.h>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
class Face
{
private:
    cv::Mat frame;
    cv::Mat face;
    cv::CascadeClassifier haarCascadeFace;
    bool faceCaptured = false;
public:
    Face(cv::Mat frame);
    void faceDetection();
    void startDetectionInThread();
    void setFrame(cv::Mat &frame);
    void setFace(cv::Mat &face);
    std::string faceIdentification();
    void launchFaceRecognition();
    bool isRecognitionStarted =false;
    cv::Mat getFrame();
    bool isFaceCaptured() const { return faceCaptured; }
    //std::string run_python_script_and_get_result(const std::string& script);
};

#endif