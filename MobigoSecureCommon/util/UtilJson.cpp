////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utiljson.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilJson::CUtilJson(void)
{
}

CUtilJson::~CUtilJson(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// static member function 
////////////////////////////////////////////////////////////////////////////////

JsonBox::Value CUtilJson::get(const JsonBox::Object object, const std::string& key)
{
	auto it = object.find(key);
	if (it == object.end())
	{
        return NULL;
		//throw std::exception(("key가 존재하지않음 [" + key + "]").c_str());
	}

	return (*it).second;
}

std::string CUtilJson::get_string(const JsonBox::Object object, const std::string& key)
{
	return get(object, key).getString();
}

std::wstring CUtilJson::get_stringW(const JsonBox::Object object, const std::string& key)
{
	return (std::wstring)CA2W(get_string(object, key).c_str(), CP_UTF8);
}

std::string CUtilJson::opt_string(const JsonBox::Object object, const std::string& key)
{
	try
	{
		return get(object, key).getString().c_str();
	}
	catch (std::exception& e)
	{
		return "";
	}
}

std::wstring CUtilJson::opt_stringW(const JsonBox::Object object, const std::string& key)
{
	return (std::wstring)CA2W(opt_string(object, key).c_str(), CP_UTF8);
}

CStringA CUtilJson::get_cstring(const JsonBox::Object object, const std::string& key)
{
	return get_string(object, key).c_str();
}

CStringW CUtilJson::get_cstringW(const JsonBox::Object object, const std::string& key)
{
	return (CStringW)CA2W(get_cstring(object, key), CP_UTF8);
}

CStringA CUtilJson::opt_cstring(const JsonBox::Object object, const std::string& key)
{
	try
	{
		return get(object, key).getString().c_str();
	}
	catch (std::exception& e)
	{
		return "";
	}
}

CStringW CUtilJson::opt_cstringW(const JsonBox::Object object, const std::string& key)
{
	return (CStringW)CA2W(opt_cstring(object, key), CP_UTF8);
}

JsonBox::Object CUtilJson::get_object(const JsonBox::Object object, const std::string& key)
{
	return get(object, key).getObject();
}

JsonBox::Object CUtilJson::opt_object(const JsonBox::Object object, const std::string& key)
{
	try
	{
		return get(object, key).getObject();
	}
	catch (std::exception& e)
	{
		return JsonBox::Object();
	}
}

JsonBox::Array CUtilJson::get_array(const JsonBox::Object object, const std::string& key)
{
	return get(object, key).getArray();
}

JsonBox::Array CUtilJson::opt_array(const JsonBox::Object object, const std::string& key)
{
	try
	{
		return get(object, key).getArray();
	}
	catch (std::exception& e)
	{
		return JsonBox::Array();
	}
}

bool CUtilJson::get_bool(const JsonBox::Object object, const std::string& key)
{
	return get(object, key).getBoolean();
}

int CUtilJson::get_int(const JsonBox::Object object, const std::string& key)
{
	return get(object, key).getInteger();
}

long CUtilJson::get_long(const JsonBox::Object object, const std::string& key)
{
	return static_cast<long>(get(object, key).getDouble());
}