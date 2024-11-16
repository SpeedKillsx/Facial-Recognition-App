#include <Python.h>
#include <QApplication>
#include <QPushButton>
#include "GUI/MainWindow.hpp"
#include "camera/Camera.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    // Create and configure the Camera instance
    Camera camera(0, 640, 480, "Camera", mainWindow.findChild<QLabel*>("frame"));  // Use the QLabel from the UI

    // Connect the Camera frame signal to the MainWindow update slot
    QObject::connect(&camera, &Camera::frameReady, &mainWindow, &MainWindow::updateFrame);

    // Connect button to start camera functionality
    QObject::connect(mainWindow.findChild<QPushButton*>("pushButton"), &QPushButton::clicked, [&]() {
        camera.startCamera();  // Start the camera when the button is clicked
    });

    return app.exec();
}
