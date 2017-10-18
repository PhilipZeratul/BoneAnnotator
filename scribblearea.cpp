#include "scribblearea.h"
#include "ui_scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScribbleArea)
{
    ui->setupUi(this);
    zoomScale = 1;
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
    resizeDisplayImage();

    isModified = false;
    update();
    return true;
}

void ScribbleArea::setZoomScale(int scale)
{
    zoomScale = scale;
}

void ScribbleArea::resizeDisplayImage()
{
    resizeImage(&originalImage, &displayImage, zoomScale);
    update();
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);

    painter.drawImage(dirtyRect, displayImage, dirtyRect);
}

void ScribbleArea::resizeImage(QImage *sourceImage, QImage *resizedImage, int scale)
{
    cv::Mat sourceMat = qImageToMat(sourceImage);
    cv::Mat resizedMat;
    cv::resize(sourceMat, resizedMat, cv::Size(0, 0), scale, scale);
    *resizedImage = matToQImage(&resizedMat);
}

cv::Mat ScribbleArea::qImageToMat(QImage *qImage)
{
    return cv::Mat(qImage->height(), qImage->width(),
                          CV_8UC4, qImage->bits(), qImage->bytesPerLine());
}

QImage ScribbleArea::matToQImage(cv::Mat *mat)
{
    return QImage(mat->data, mat->cols, mat->rows, mat->step, QImage::Format_RGB32);
}
