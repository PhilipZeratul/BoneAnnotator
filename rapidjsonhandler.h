#ifndef RAPIDJSONHANDLER_H
#define RAPIDJSONHANDLER_H

#include <QPoint>
#include <QDebug>

#include "rapidjson/memorystream.h"

class RapidJsonHandler
{
public:
    RapidJsonHandler();
    void Init(std::vector<std::vector<QPoint>> *_boneVector);

    bool Null();
    bool Bool(bool b);
    bool Int(int i);
    bool Int64(int64_t i);
    bool Uint64(uint64_t i);
    bool Double(double d);
    bool RawNumber(const rapidjson::MemoryStream::Ch* str, rapidjson::SizeType length, bool copy);
    bool String(const rapidjson::MemoryStream::Ch* str, rapidjson::SizeType length, bool copy);

    bool Uint(unsigned i);
    bool StartObject();
    bool Key(const rapidjson::MemoryStream::Ch* str, rapidjson::SizeType length, bool copy);
    bool EndObject(rapidjson::SizeType memberCount);
    bool StartArray();
    bool EndArray(rapidjson::SizeType elementCount);

private:
    void test_ShowBoneList();

    std::vector<std::vector<QPoint>> *boneVector;
    QPoint point;
    int point_x;
    int point_y;

    enum State {
        kExpectObjectStart,
        kExpectKeyOrObjectEnd,
        kExpectBoneStart,
        kExpectBoneEndOrPointStart,
        kExpectPointEnd,
        kExpectPoint_X,
        kExpectPoint_Y
    }state_;
};

#endif // RAPIDJSONHANDLER_H
