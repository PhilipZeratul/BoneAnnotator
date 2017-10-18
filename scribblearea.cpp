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
    isImageOpend = true;
    boneVector.clear();

    update();
    return true;
}

void ScribbleArea::setZoomScale(int scale)
{
    zoomScale = scale;
}

void ScribbleArea::setIsImageOpened(bool value)
{
    isImageOpend = value;
}

void ScribbleArea::resizeDisplayImage()
{
    resizeImage(&originalImage, &displayImage, zoomScale);
    drawBone();
    update();
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);

    painter.drawImage(dirtyRect, displayImage, dirtyRect);
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && isImageOpend && isClickedOnImage(event->pos()))
    {
        boneVector.push_back(std::vector<QPoint>());
        boneVector.back().push_back(event->pos() / zoomScale);

        qInfo() << "mouse right button pressed." << event->pos() << "boneVector.size =" << boneVector.size();
    }

    if (event->button() == Qt::LeftButton && isImageOpend && isClickedOnImage(event->pos()))
    {
        if (boneVector.size() > 0)
        {
            boneVector.back().push_back(event->pos() / zoomScale);
        }
        qInfo() << "mouse left button pressed." << event->pos();
        test_ShowBoneList();
    }

    drawBone();
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    //if ((event->buttons() & Qt::LeftButton) && scribbling)
    //    drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    /*if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }*/
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

bool ScribbleArea::isClickedOnImage(QPoint pos)
{
    if ((pos.x() <= displayImage.width()) && (pos.y() <= displayImage.height()))
        return true;

    return false;
}

void ScribbleArea::drawBone()
{
    int lineWidth = 2 * zoomScale;
    int hue = 0;
    QPainter painter(&displayImage);
    QColor lineColor;

    isModified = true;
    for (int i = 0; i < (int)boneVector.size(); i++)
    {
        for (int j = 1; j < (int)boneVector[i].size(); j++)
        {
            hue = std::min(hue, 359);
            painter.setPen(QPen(lineColor.fromHsv(hue, 255, 255), lineWidth,
                                Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            hue += 20;

            QPoint startPoint = boneVector[i][j - 1] * zoomScale;
            QPoint endPoint = boneVector[i][j]*zoomScale;
            painter.drawLine(startPoint, endPoint);
            int rad = (lineWidth / 2) + 2;
            update(QRect(startPoint, endPoint).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad));
        }
    }
}

void ScribbleArea::test_ShowBoneList()
{
    for (int i = 0; i < (int)boneVector.size(); i++)
    {
        for (int j = 0; j < (int)boneVector[i].size(); j++)
        {
            qInfo() << "boneVector" << i << j << "=" << boneVector[i][j];
        }
    }
}
