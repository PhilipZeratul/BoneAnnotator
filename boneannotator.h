#ifndef BONEANNOTATOR_H
#define BONEANNOTATOR_H

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QDirIterator>
#include <QStringList>
#include <QListWidgetItem>
#include <QtGlobal>

#include "scribblearea.h"
#include "datahandler.h"

namespace Ui {
class BoneAnnotator;
}

class BoneAnnotator : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoneAnnotator(QWidget *parent = 0);
    ~BoneAnnotator();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void on_spinBox_Zoom_valueChanged(int arg1);

    void on_listWidget_ImageList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::BoneAnnotator *ui;

    void createActions();
    void openDirectory();
    void createBoneResultDir();
    void constructImageList(QDirIterator& dirIter);
    bool checkIsImage(QFileInfo fileInfo);
    void constructListWidget();
    void openImageInScribbleArea(const QString filePath);

    ScribbleArea *scribbleArea;
    DataHandler dataHandler;
    QString imageDirectory;
    QString resultDirectory;
    QStringList fileNameList;
    QString previousImageName;
    QString pathSeperator;
};

#endif // BONEANNOTATOR_H
