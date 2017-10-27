#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPoint>
#include <QDebug>
#include <QtGlobal>

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "opencv2/opencv.hpp"

#include "rapidjsonhandler.h"

class DataHandler
{
public:
    DataHandler();
    void SerializeToJson(std::vector<std::vector<QPoint>> *boneVector, QString imagePath);
    void SaveResultImage(cv::Mat resultImage, QString imagePath);
    void ReadFromJson(QString imagePath, std::vector<std::vector<QPoint>> *boneVector);

private:
    QString GetJsonFilePath(QString &imagePath);
    QString GetResultImagePath(QString &imagePath);
    void SaveJsonToDisk(const rapidjson::MemoryStream::Ch *content, QString savePath);
    void JsonToBoneVector(QString json, std::vector<std::vector<QPoint>> *boneVector);
    RapidJsonHandler rapidJsonHandler;



    ///
    void test_SaveJson();
};

#endif // DATAHANDLER_H
