#include "rapidjsonhandler.h"

using namespace rapidjson;

RapidJsonHandler::RapidJsonHandler()
{}

void RapidJsonHandler::Init(std::vector<std::vector<QPoint> > *_boneVector)
{
    boneVector = _boneVector;
    state_ = kExpectObjectStart;
}

bool RapidJsonHandler::Null(){ qDebug() << "Null"; return false; }
bool RapidJsonHandler::Bool(bool b){ qDebug() << "Bool" << b; return false; }
bool RapidJsonHandler::Int(int i){ qDebug() << "Int" << i; return false; }
bool RapidJsonHandler::Int64(int64_t i){ qDebug() << "Int64" << i; return false; }
bool RapidJsonHandler::Uint64(uint64_t i){ qDebug() << "UInt64" << i; return false; }
bool RapidJsonHandler::Double(double d){ qDebug() << "Double" << d; return false; }
bool RapidJsonHandler::String(const MemoryStream::Ch* str, SizeType length, bool copy){ qDebug() << "String" << str; return false; }

bool RapidJsonHandler::Uint(unsigned i)
{
    switch (state_)
    {
    case kExpectPoint_X:
        state_ = kExpectPoint_Y;
        point_x = i;
        return true;
    case kExpectPoint_Y:
        state_ = kExpectPointEnd;
        point_y = i;
        return true;
    default:
        return false;
    }
}

bool RapidJsonHandler::StartObject()
{
    switch (state_)
    {
    case kExpectObjectStart:
        boneVector->clear();
        state_ = kExpectKeyOrObjectEnd;
        return true;
    default:
        return false;
    }
}

bool RapidJsonHandler::Key(const MemoryStream::Ch* str, SizeType length, bool copy)
{
    switch (state_)
    {
    case kExpectKeyOrObjectEnd:
        state_ = kExpectBoneStart;
        return true;
    default:
        return false;
    }
}

bool RapidJsonHandler::EndObject(SizeType memberCount)
{
    state_ = kExpectObjectStart;
    return true;
}

bool RapidJsonHandler::StartArray()
{
    switch (state_)
    {
    case kExpectBoneStart:
        state_ = kExpectBoneEndOrPointStart;
        boneVector->push_back(std::vector<QPoint>());
        return true;
    case kExpectBoneEndOrPointStart:
        state_ = kExpectPoint_X;
        return true;
    default:
        return false;
    }
}

bool RapidJsonHandler::EndArray(SizeType elementCount)
{
    switch (state_)
    {
    case kExpectPointEnd:
        state_ = kExpectBoneEndOrPointStart;
        boneVector->back().push_back(QPoint(point_x, point_y));
        return true;
    case kExpectBoneEndOrPointStart:
        state_ = kExpectKeyOrObjectEnd;
        return true;
    default:
        return false;
    }
}



///
void RapidJsonHandler::test_ShowBoneList()
{
    for (int i = 0; i < (int)boneVector->size(); i++)
    {
        for (int j = 0; j < (int)boneVector->at(i).size(); j++)
        {
            qInfo() << "rapid json handler boneVector" << i << j << "=" << boneVector->at(i).at(j);
        }
    }
}
