#include "MainWindow.hpp"
#include "window.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateFrame(const QImage& image) {
    ui->frame->setPixmap(QPixmap::fromImage(image));
}
