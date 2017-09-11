#include "pch.h"
#include "Common/Helpers.h"
#include "Common/EngineException.h"

#include <algorithm>

#ifndef ANDROID_NDK
#include <codecvt>
#else
#include <sstream>
#endif

using namespace Common;

bool Helpers::FileExists(const std::wstring& filename)
{
	std::fstream fileStream(WStringToString(filename));
	return fileStream.good();
}

std::wstring Helpers::GetFilename(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto begin = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (begin == filename.end())
		begin = filename.begin();
	else
		++begin;

	std::wstring dot(L".");
	auto end = std::find_end(begin, filename.end(), dot.begin(), dot.end());

	return std::wstring(begin, end);
}
std::wstring Helpers::GetFileExtension(const std::wstring& filename)
{
	std::wstring dot(L".");
	auto end = std::find_end(filename.begin(), filename.end(), dot.begin(), dot.end());
	if (end == filename.end())
		throw EngineException("Filename has no extension.");

	return std::wstring(end + 1, filename.end());
}
std::wstring Helpers::GetFilePath(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto location = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (location == filename.end())
		return L"";

	return std::wstring(filename.begin(), location + 1);
}

std::wstring Helpers::StringToWString(const std::string& str)
{
#ifdef ANDROID_NDK
	{
		std::wstringstream ss;
		ss << str.c_str();
		return ss.str();
	}
#else
	{
		using convertType = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convertType, wchar_t> converter;

		return converter.from_bytes(str);
	}
#endif
}

std::string Helpers::WStringToString(const std::wstring& wstr)
{
#ifdef ANDROID_NDK
	{
		std::stringstream ss;
		ss << wstr.c_str();
		return ss.str();
	}
#else
	{
		using convertType = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convertType, wchar_t> converter;

		return converter.to_bytes(wstr);
	}
#endif
}

