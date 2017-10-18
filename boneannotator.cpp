#include "boneannotator.h"
#include "ui_boneannotator.h"

BoneAnnotator::BoneAnnotator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoneAnnotator)
{
    ui->setupUi(this);
    createActions();
    scribbleArea = ui->widget_ScribbleArea;
}

BoneAnnotator::~BoneAnnotator()
{
    delete ui;
}

void BoneAnnotator::createActions()
{
    connect(ui->action_OpenDirectory, &QAction::triggered, this, &BoneAnnotator::openDirectory);
    connect(ui->listWidget_ImageList, &QListWidget::itemSelectionChanged, this, &BoneAnnotator::onImageListItemSelectionChanged);
}

void BoneAnnotator::openDirectory()
{
    // Clear fileNameList to prepare for new image files.
    fileNameList.clear();

    directory  = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   QDir::currentPath(),
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    qInfo() << "Opened Directory: " << directory;

    QDirIterator dirIter(directory);
    constructImageList(dirIter);
    constructListWidget();
    scribbleArea->setIsImageOpened(false);
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

void BoneAnnotator::onImageListItemSelectionChanged()
{
    if (ui->listWidget_ImageList->selectedItems().count() > 0)
    {
        QString filePath = directory + "/" + ui->listWidget_ImageList->selectedItems()[0]->text();
        openImageInScribbleArea(filePath);
    }
}

void BoneAnnotator::openImageInScribbleArea(const QString filePath)
{
    scribbleArea->openImage(filePath);
}

void BoneAnnotator::on_spinBox_Zoom_valueChanged(int arg1)
{
    scribbleArea->setZoomScale(arg1);
    scribbleArea->resizeDisplayImage();
}
