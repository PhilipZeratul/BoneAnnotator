#ifndef BONEANNOTATOR_H
#define BONEANNOTATOR_H

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QDirIterator>
#include <QStringList>
#include <QListWidgetItem>

#include "scribblearea.h"

namespace Ui {
class BoneAnnotator;
}

class BoneAnnotator : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoneAnnotator(QWidget *parent = 0);
    ~BoneAnnotator();

private:
    Ui::BoneAnnotator *ui;

    void createActions();
    void openDirectory();
    void constructImageList(QDirIterator& dirIter);
    bool checkIsImage(QFileInfo fileInfo);
    void constructListWidget();
    void onImageListItemSelectionChanged();
    void openImageInScribbleArea(const QString filePath);

    ScribbleArea *scribbleArea;
    QString directory;
    QStringList fileNameList;
};

#endif // BONEANNOTATOR_H
