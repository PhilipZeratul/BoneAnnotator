#include "scribblearea.h"
#include "ui_scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScribbleArea)
{
    ui->setupUi(this);
    zoomScale = 2;
}

ScribbleArea::~ScribbleArea()
{
    delete ui;
}

bool ScribbleArea::openImage(const QString &fileName)
{
    qInfo() << fileName;
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    originalImage = loadedImage;
//    originalMat = qImageToMat(originalImage);

//    resizeImage(originalMat, displayMat, zoomScale);

//    displayImage = matToQImage(displayMat);
    displayImage = originalImage;

    isModified = false;
    update();
    return true;
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);

    painter.drawImage(dirtyRect, displayImage, dirtyRect);
}

void ScribbleArea::setZoomScale(int scale)
{
    zoomScale = scale;
    qInfo() << "zoomScale =" << zoomScale;
}

void ScribbleArea::resizeImage(cv::Mat originalMat, cv::Mat resizedMat, int scale)
{
    cv::resize(originalMat, resizedMat, cv::Size(0, 0), scale, scale);

     qInfo() << "originalMat.size =" << originalMat.rows << originalMat.cols << "resizeMat.size =" << resizedMat.rows << resizedMat.cols;
}

QImage ScribbleArea::matToQImage(cv::Mat mat)
{
    return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
}

cv::Mat ScribbleArea::qImageToMat(QImage qImage)
{
    return cv::Mat(qImage.height(), qImage.width(),
                   CV_8UC3, qImage.bits(), qImage.bytesPerLine());
}
