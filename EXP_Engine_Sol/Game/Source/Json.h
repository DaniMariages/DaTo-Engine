#pragma once
#include "Globals.h"

#include "../External/Parson/parson.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

#include <string>

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class Color;
class ArrayConfig;

class JsonConfig {
private:
	std::string path;
	JSON_Object* node = nullptr;
	JSON_Value* value = nullptr;

public:

	JsonConfig();
	JsonConfig(const char* buffer);
	JsonConfig(const std::string path, JSON_Object* object, JSON_Value* value);
	JsonConfig(JSON_Object* jsonObject);
	~JsonConfig();

	uint SerializeConfig(char** buffer);

	double GetNumber(const std::string name);
	void SetNumber(const std::string name, const double number);

	std::string GetString(const std::string name) const;
	void SetString(std::string name, std::string destination);

	bool GetBool(const std::string name);
	void SetBool(const std::string name, const bool boolean);

	Color GetColor(const std::string name);
	void SetColor(const std::string name, const Color color);

	float3 GetFloat3(const std::string name);
	void SetFloat3(const std::string name, const float3 float3);

	Quat GetQuat(const std::string name);
	void SetQuat(const std::string name, const Quat quat);

	ArrayConfig GetArray(const std::string name) const;
	ArrayConfig SetArray(const std::string name);
};

class ArrayConfig {
private:

	JSON_Array* jArray;
	uint index = 0;

public:

	ArrayConfig();
	ArrayConfig(JSON_Array* jArray);
	~ArrayConfig();

	JsonConfig AddNode();
	JsonConfig GetNode(uint index) const;
	uint GetSize() const;
};