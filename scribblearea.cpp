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

    //QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, newSize);

    image = loadedImage;



    isModified = false;
    update();

    return true;
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{

    QRect dirtyRect = event->rect();
    QPainter painter(this);


    //QImage image("/Users/ZhangPengfei/Desktop/Annotator_2/IMG_5249.png");



    qInfo() << "paintEvent: image.size =" << image.size();
    qInfo() << "panitEvent: rect.size =" << dirtyRect.size();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
