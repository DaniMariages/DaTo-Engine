#include "Json.h"
#include "Color.h"

JsonConfig::JsonConfig()
{
    value = json_value_init_object();
    node = json_value_get_object(value);
}

JsonConfig::JsonConfig(const char* buffer)
{
    value = json_parse_string(buffer);
    if (value)
    {
        node = json_value_get_object(value);
    }
}

JsonConfig::JsonConfig(std::string _path, JSON_Object* _object, JSON_Value* _value)
{
    path = _path;
    node = _object;
    value = _value;
}

JsonConfig::JsonConfig(JSON_Object* jsonObject) : node(jsonObject) {}

JsonConfig::~JsonConfig()
{
    if (value != nullptr)
    {
        json_value_free(value);
    }
}

uint JsonConfig::SerializeConfig(char** buffer)
{
    uint size = (uint)json_serialization_size_pretty(value);
    *buffer = new char[size];
    JSON_Status status = json_serialize_to_buffer_pretty(value, *buffer, size);

    if (status == JSONFailure)
    {
        LOG("ERROR: Not able to serialize.");
        RELEASE_ARRAY(buffer);
        size = 0;
    }

    return size;
}

double JsonConfig::GetNumber(std::string name)
{
    return json_object_get_number(node, name.c_str());
}

void JsonConfig::SetNumber(std::string name, const double number)
{
    json_object_set_number(node, name.c_str(), number);
}

std::string JsonConfig::GetString(const std::string name) const
{
    return json_object_get_string(node, name.c_str());
}

void JsonConfig::SetString(std::string name, std::string destination)
{
    json_object_set_string(node, name.c_str(), destination.c_str());
}

bool JsonConfig::GetBool(const std::string name)
{
    return json_object_get_boolean(node, name.c_str());
}

void JsonConfig::SetBool(std::string name, bool boolean)
{
    json_object_set_boolean(node, name.c_str(), boolean);
}

Color JsonConfig::GetColor(const std::string name)
{
    JSON_Array* arr = json_object_get_array(json_value_get_object(value), name.data());

    Color color;
    color.r = json_array_get_number(arr, 0);
    color.g = json_array_get_number(arr, 1);
    color.b = json_array_get_number(arr, 2);
    color.a = json_array_get_number(arr, 3);

    return color;
}

void JsonConfig::SetColor(const std::string name, const Color color)
{
    json_object_set_number(node, name.c_str(), color.r);
    json_object_set_number(node, name.c_str(), color.g);
    json_object_set_number(node, name.c_str(), color.b);
    json_object_set_number(node, name.c_str(), color.a);
}


float3 JsonConfig::GetFloat3(const std::string name)
{
    JSON_Array* tempArray = json_object_get_array(node, name.data());
    float3 floats;
    floats.x = json_array_get_number(tempArray, 0);
    floats.y = json_array_get_number(tempArray, 1);
    floats.z = json_array_get_number(tempArray, 2);

    return floats;
}

void JsonConfig::SetFloat3(const std::string name, const float3 float3)
{
    JSON_Array* tempArray = json_object_get_array(node, name.data());

    if (tempArray == nullptr) 
    {
        JSON_Value* val = json_value_init_array();
        tempArray = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }
    else json_array_clear(tempArray);

    json_array_append_number(tempArray, float3.x);
    json_array_append_number(tempArray, float3.y);
    json_array_append_number(tempArray, float3.z);
}

Quat JsonConfig::GetQuat(const std::string name)
{
    JSON_Array* tempArray = json_object_get_array(node, name.data());
    Quat quat;
    quat.x = json_array_get_number(tempArray, 0);
    quat.y = json_array_get_number(tempArray, 1);
    quat.z = json_array_get_number(tempArray, 2);
    quat.w = json_array_get_number(tempArray, 3);

    return quat;
}

void JsonConfig::SetQuat(const std::string name, const Quat quat)
{
    JSON_Array* tempArray = json_object_get_array(node, name.data());

    if (tempArray == nullptr) 
    {
        JSON_Value* val = json_value_init_array();
        tempArray = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }
    else json_array_clear(tempArray);

    json_array_append_number(tempArray, quat.x);
    json_array_append_number(tempArray, quat.y);
    json_array_append_number(tempArray, quat.z);
    json_array_append_number(tempArray, quat.w);
}


ArrayConfig JsonConfig::GetArray(std::string name) const
{
    if (json_object_has_value_of_type(node, name.c_str(), JSONArray))
    {
        return ArrayConfig(json_object_get_array(node, name.c_str()));
    }
}

ArrayConfig JsonConfig::SetArray(const std::string name)
{
    json_object_set_value(node, name.c_str(), json_value_init_array());
    return ArrayConfig(json_object_get_array(node, name.c_str()));
}

ArrayConfig::ArrayConfig()
{
    jArray = json_value_get_array(json_value_init_array());
}

ArrayConfig::ArrayConfig(JSON_Array* jArray) : jArray(jArray)
{
    index = json_array_get_count(jArray);
}

ArrayConfig::~ArrayConfig() {}

JsonConfig ArrayConfig::AddNode()
{
    json_array_append_value(jArray, json_value_init_object());
    index++;
    return JsonConfig(json_array_get_object(jArray, index - 1));
}

JsonConfig ArrayConfig::GetNode(uint index) const
{
    return JsonConfig(json_array_get_object(jArray, index));
}

uint ArrayConfig::GetSize() const
{
    return index;
}
