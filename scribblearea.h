#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QImage>
#include <QLabel>

#include <opencv2/imgproc.hpp>
#include <vector>

namespace Ui {
class ScribbleArea;
}

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = 0);
    ~ScribbleArea();
    bool openImage(const QString &fileName);
    void setZoomScale(int scale);
    void setIsImageOpened(bool value);
    void resizeDisplayImage();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:    
    void resizeImage(QImage *sourceImage, QImage *resizedImage, int scale);
    cv::Mat qImageToMat(QImage *qImage);
    QImage matToQImage(cv::Mat *mat);
    bool isClickedOnImage(QPoint pos);
    void drawBone();



    ///
    void test_ShowBoneList();

    Ui::ScribbleArea *ui;
    QImage originalImage;
    QImage displayImage;
    cv::Mat originalMat;
    cv::Mat displayMat;
    bool isModified;
    bool isScribbling;
    int zoomScale;
    bool isImageOpend;
    std::vector<std::vector<QPoint>> boneVector;
};

#endif // SCRIBBLEAREA_H
