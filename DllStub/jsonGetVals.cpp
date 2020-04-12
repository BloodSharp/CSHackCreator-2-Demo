#include "DllStub.h"

void GetVal(Json::Value& config, unsigned int* setting)
{
    if (config.isNull())
        return;

    *setting = config.asUInt();
}

void GetVal(Json::Value& config, int* setting)
{
    if (config.isNull())
        return;

    *setting = config.asInt();
}

void GetVal(Json::Value& config, bool* setting)
{
    if (config.isNull())
        return;

    *setting = config.asBool();
}

void GetVal(Json::Value& config, float* setting)
{
    if (config.isNull())
        return;

    *setting = config.asFloat();
}

void GetVal(Json::Value& config, double* setting)
{
    if (config.isNull())
        return;

    *setting = config.asDouble();
}

void GetVal(Json::Value& config, char* setting, int bufferSize)
{
    if (config.isNull())
        return;

    strcpy_s(setting, bufferSize, config.asCString());
}