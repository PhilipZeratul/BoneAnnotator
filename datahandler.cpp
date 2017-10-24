#include "datahandler.h"

using namespace rapidjson;
using namespace std;

DataHandler::DataHandler()
{}

void DataHandler::Serialize(vector<vector<QPoint>> *boneVector, QString savePath)
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
            writer.Key("Point");
            writer.StartArray();
            writer.Uint(point.x());
            writer.Uint(point.y());
            writer.EndArray();
        }
        writer.EndArray();
    }
    writer.EndObject();

    qInfo() << s.GetString();
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
