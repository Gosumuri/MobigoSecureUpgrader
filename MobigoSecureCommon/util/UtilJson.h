#pragma once


#include <string>
#include "JsonBox.h"

class AFX_EXT_CLASS CUtilJson
{
public:
	CUtilJson(void);
	~CUtilJson(void);

private:
	static JsonBox::Value get(const JsonBox::Object object, const std::string& key);	//throw
public:

	static std::string		get_string(const JsonBox::Object object, const std::string& key);
	static std::wstring		get_stringW(const JsonBox::Object object, const std::string& key);
	static std::string		opt_string(const JsonBox::Object object, const std::string& key);
	static std::wstring		opt_stringW(const JsonBox::Object object, const std::string& key);

	static CStringA			get_cstring(const JsonBox::Object object, const std::string& key);
	static CStringW			get_cstringW(const JsonBox::Object object, const std::string& key);
	static CStringA			opt_cstring(const JsonBox::Object object, const std::string& key);
	static CStringW			opt_cstringW(const JsonBox::Object object, const std::string& key);

	static bool				get_bool(const JsonBox::Object object, const std::string& key);
	static int				get_int(const JsonBox::Object object, const std::string& key);
	static long				get_long(const JsonBox::Object object, const std::string& key);

	static JsonBox::Object	get_object(const JsonBox::Object object, const std::string& key);
	static JsonBox::Object	opt_object(const JsonBox::Object object, const std::string& key);

	static JsonBox::Array	get_array(const JsonBox::Object, const std::string& key);
	static JsonBox::Array	opt_array(const JsonBox::Object, const std::string& key);

};
