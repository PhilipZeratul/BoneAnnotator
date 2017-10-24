#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QPoint>
#include <QDebug>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class DataHandler
{
public:
    DataHandler();

    rapidjson::Document document;

    void Serialize(std::vector<std::vector<QPoint> > *boneVector, QString savePath);



    ///
    void test_SaveJson();
};

#endif // DATAHANDLER_H
