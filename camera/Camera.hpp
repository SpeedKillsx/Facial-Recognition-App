// Camera.hpp
#include <QObject>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>

class Camera : public QObject {
    Q_OBJECT

public:
    Camera(int deviceID, int width, int height, const std::string& name, QLabel* videoLabel);

    void startCamera();
    void stopCamera();
    void setFrame(cv::Mat &frame);

private slots:
    void captureFrame();

private:
    cv::VideoCapture cap;
    QLabel* videoLabel;
    std::string name;
    QTimer timer;

    // Déclarez les variables ici
    bool faceCaptured = false;  // Variable pour suivre si le visage est capturé
    std::string identifiedFace = "Reconizing...";  // Nom du visage détecté
    void setFace(cv::Mat &face);

signals:
    void frameReady(const QImage &image);
};

