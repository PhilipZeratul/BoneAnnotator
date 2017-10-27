#include "boneannotator.h"
#include "ui_boneannotator.h"

BoneAnnotator::BoneAnnotator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoneAnnotator)
{
    ui->setupUi(this);
    createActions();
    scribbleArea = ui->widget_ScribbleArea;
    scribbleArea->setZoomScale(ui->spinBox_Zoom->value());
    boneVector = scribbleArea->getBoneVector();

#ifdef Q_OS_WIN
    pathSeperator = "\\";
#else
    pathSeperator = "/";
#endif
}

BoneAnnotator::~BoneAnnotator()
{
    delete ui;
}

// TODO: press space to change to next image.
void BoneAnnotator::keyReleaseEvent(QKeyEvent *event)
{
    qInfo() << "Key pressed:" << event->text();
}

void BoneAnnotator::createActions()
{
    connect(ui->action_OpenDirectory, &QAction::triggered, this, &BoneAnnotator::openDirectory);
}

void BoneAnnotator::openDirectory()
{
    // Clear fileNameList to prepare for new image files.
    fileNameList.clear();

    imageDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       QDir::currentPath(),
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    qInfo() << "Opened Directory:" << imageDirectory;

    createBoneResultDir();

    QDirIterator dirIter(imageDirectory);
    constructImageList(dirIter);
    constructListWidget();
    scribbleArea->setIsImageOpened(false);
}

// TODO: Create Directory.
void BoneAnnotator::createBoneResultDir()
{
    QString parentPath = imageDirectory.left(imageDirectory.lastIndexOf(pathSeperator));
    resultDirectory = parentPath + pathSeperator + "BoneResult";
    qInfo() << "Result Directory:" << resultDirectory;
    QDir dir(resultDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void BoneAnnotator::constructImageList(QDirIterator& dirIter)
{
    while (dirIter.hasNext())
    {
        bool isImage = checkIsImage(dirIter.fileInfo());
        if (isImage)
            fileNameList.append(dirIter.fileName());

        dirIter.next();
    }

    bool isImage = checkIsImage(dirIter.fileInfo());
    if (isImage)
        fileNameList.append(dirIter.fileName());
}

bool BoneAnnotator::checkIsImage(QFileInfo fileInfo)
{
    QString extension = fileInfo.suffix();
    if (extension == "png" || extension == "jpg")
        return true;
    return false;
}

void BoneAnnotator::constructListWidget()
{
    ui->listWidget_ImageList->clear();
    ui->listWidget_ImageList->addItems(fileNameList);
}

void BoneAnnotator::on_spinBox_Zoom_valueChanged(int arg1)
{
    scribbleArea->setZoomScale(arg1);
    scribbleArea->resizeOriginalImage();
}

void BoneAnnotator::on_listWidget_ImageList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous != NULL)
    {
        QString previousImageResultPath = resultDirectory + pathSeperator + previous->text();
        qInfo() << "Previous Image Path:" << previousImageResultPath;
        dataHandler.SerializeToJson(boneVector, previousImageResultPath);
        dataHandler.SaveResultImage(scribbleArea->getResultImage(), previousImageResultPath);
    }
    if (current != NULL)
    {
        QString imagePath = imageDirectory + pathSeperator + current->text();
        QString imageResultPath = resultDirectory + pathSeperator + current->text();
        dataHandler.ReadFromJson(imageResultPath, boneVector);
        scribbleArea->openImage(imagePath);
    }
}
