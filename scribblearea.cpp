#include "scribblearea.h"
#include "ui_scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScribbleArea)
{
    ui->setupUi(this);
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

    image = loadedImage;

    isModified = false;
    update();
    return true;
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);

    painter.drawImage(dirtyRect, image, dirtyRect);
}
