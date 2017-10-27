#include "datahandler.h"

using namespace rapidjson;
using namespace std;

DataHandler::DataHandler()
{}

void DataHandler::SerializeToJson(vector<vector<QPoint>> *boneVector, QString imagePath)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    for (unsigned i = 0; i < boneVector->size(); i++)
    {
        writer.Key("Bone");
        writer.StartArray();
        for (unsigned j = 0; j < boneVector->at(i).size(); j++)
        {
            QPoint point = boneVector->at(i).at(j);
            writer.StartArray();
            writer.Uint(point.x());
            writer.Uint(point.y());
            writer.EndArray();
        }
        writer.EndArray();
    }
    writer.EndObject();

    QString jsonPath = GetJsonFilePath(imagePath);
    SaveJsonToDisk(s.GetString(), jsonPath);
}

void DataHandler::SaveResultImage(cv::Mat resultImage, QString imagePath)
{
    QString resultImagePath = GetResultImagePath(imagePath);
    cv::imwrite(resultImagePath.toStdString(), resultImage);
}

void DataHandler::ReadFromJson(QString imagePath, std::vector<std::vector<QPoint>> *boneVector)
{
    boneVector->clear();
    QString jsonFile = GetJsonFilePath(imagePath);
    QFile file(jsonFile);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line = in.readAll();
        JsonToBoneVector(line, boneVector);
    }
    file.close();
}

QString DataHandler::GetJsonFilePath(QString &imagePath)
{
    QString prefix = imagePath.left(imagePath.lastIndexOf("."));
    return prefix + ".json";
}

QString DataHandler::GetResultImagePath(QString &imagePath)
{
    QString prefix = imagePath.left(imagePath.lastIndexOf("."));
    return prefix + ".result.png";
}

void DataHandler::SaveJsonToDisk(const rapidjson::MemoryStream::Ch *content, QString savePath)
{
    QString filename = savePath;
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << content << endl;
    }
    file.close();
}

void DataHandler::JsonToBoneVector(QString json, std::vector<std::vector<QPoint>> *boneVector)
{
    QByteArray ba = json.toLatin1();
    const char *jsonCharArray = ba.data();

    rapidJsonHandler.Init(boneVector);
    Reader reader;
    StringStream ss(jsonCharArray);
    reader.Parse(ss, rapidJsonHandler);
}



///
void DataHandler::test_SaveJson()
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("hello");
    writer.String("world");
    writer.Key("t");
    writer.Bool(true);
    writer.Key("f");
    writer.Bool(false);
    writer.Key("n");
    writer.Null();
    writer.Key("i");
    writer.Uint(123);
    writer.Key("pi");
    writer.Double(3.1416);
    writer.Key("a");
    writer.StartArray();
    for (unsigned i = 0; i < 4; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();
    qInfo() << s.GetString();
}
