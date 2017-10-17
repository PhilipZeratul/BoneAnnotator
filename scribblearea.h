#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QImage>

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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::ScribbleArea *ui;

    QImage image;
    bool isModified;
    bool isScribbling;
};

#endif // SCRIBBLEAREA_H
