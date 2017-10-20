#include "scribblearea.h"
#include "ui_scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScribbleArea)
{
    ui->setupUi(this);
    isScribbling = false;
}

ScribbleArea::~ScribbleArea()
{
    delete ui;
}

bool ScribbleArea::openImage(const QString &fileName)
{
    isModified = false;
    isImageOpend = true;
    isScribbling = false;

    qInfo() << fileName;
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    boneVector.clear();
    originalImage = loadedImage;
    resizeOriginalImage();

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

void ScribbleArea::resizeOriginalImage()
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
    if (isImageOpend && isClickedOnImage(event->pos()))
    {
        if (event->button() == Qt::RightButton)
        {
            boneVector.push_back(std::vector<QPoint>());
            boneVector.back().push_back(event->pos() / zoomScale);
        }

        if (event->button() == Qt::LeftButton)
        {
            if (boneVector.size() > 0)
                boneVector.back().push_back(event->pos() / zoomScale);
        }
        isScribbling = true;
        drawBone();



        ///
        //test_ShowBoneList();
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (isImageOpend && isClickedOnImage(event->pos()) && isScribbling)
    {
        tempBonePoint = event->pos();
        drawBone();
    }
    else
    {
        tempBonePoint.setX(0);
        tempBonePoint.setY(0);
    }
    qInfo() << "mouseMoveEvent: " << tempBonePoint << "isImageOpend =" << isImageOpend << "isClickedOnImage = " << isClickedOnImage(event->pos()) << "isScribbling =" << isScribbling;
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
    resizeImage(&originalImage, &displayImage, zoomScale);

    int lineWidth = 3 * zoomScale;
    int pointWidth = 6 * zoomScale;
    int hue = 0;
    int rad;
    QPainter painter(&displayImage);
    QColor color;
    QPoint startPoint;
    QPoint endPoint;

    isModified = true;
    for (int i = 0; i < (int)boneVector.size(); i++)
        for (int j = 0; j < (int)boneVector[i].size(); j++, hue += 15)
        {
            hue = std::min(hue, 344);
            color.setAlpha(255);
            painter.setPen(QPen(color.fromHsv(hue, 255, 255), lineWidth,
                                Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            if (j == 0)
            {
                startPoint = boneVector[i][j] * zoomScale;
                endPoint = startPoint;
            }
            else
            {
                startPoint = boneVector[i][j - 1] * zoomScale;
                endPoint = boneVector[i][j] * zoomScale;
                painter.drawLine(startPoint, endPoint);
            }
            painter.setPen(QPen(color.fromHsv(hue, 255, 255), pointWidth,
                                Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPoint(endPoint);
            rad = (pointWidth / 2) + 2;
        }

    // Alpha not functioning
    color = color.fromHsv(hue, 255, 255);
    color.setAlpha(50);
    painter.setPen(QPen(color, lineWidth,
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    qInfo() << "endPoint: " << endPoint << "tempBonePoint: " << tempBonePoint;
    if (!tempBonePoint.isNull())
        painter.drawLine(endPoint, tempBonePoint);

    update();
}



///
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
