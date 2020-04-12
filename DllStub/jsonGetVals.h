#pragma once

void GetVal(Json::Value& config, unsigned int* setting);
void GetVal(Json::Value& config, int* setting);
void GetVal(Json::Value& config, bool* setting);
void GetVal(Json::Value& config, float* setting);
void GetVal(Json::Value& config, double* setting);
void GetVal(Json::Value& config, char* setting, int bufferSize);