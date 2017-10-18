#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QImage>

#include <opencv2/imgproc.hpp>

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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void resizeImage(cv::Mat *originalMat, cv::Mat *resizedMat, int scale);
    cv::Mat qImageToMat(QImage *qImage);
    QImage matToQImage(cv::Mat *mat);

    Ui::ScribbleArea *ui;
    QImage originalImage;
    QImage displayImage;
    cv::Mat originalMat;
    cv::Mat displayMat;
    bool isModified;
    bool isScribbling;
    int zoomScale;
};

#endif // SCRIBBLEAREA_H
