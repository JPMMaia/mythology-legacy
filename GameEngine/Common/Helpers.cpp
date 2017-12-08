export module Common.Helpers;

import Common.Exceptions;
import std.core;
import std.threading;

using namespace std;

namespace Common
{
	namespace Helpers
	{
		wstring StringToWString(const string& str)
		{
			wstringstream ss;
			ss << str.c_str();
			return ss.str();
		}
		string WStringToString(const wstring& wstr)
		{
			stringstream ss;
			ss << wstr.c_str();
			return ss.str();
		}

		bool FileExists(const wstring& filename)
		{
			/*fstream fileStream(WStringToString(filename));
			return fileStream.good();*/
			return false;
		}
		wstring GetFilename(const wstring& filename)
		{
			wstring searchString(L"/");

			auto begin = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
			if (begin == filename.end())
				begin = filename.begin();
			else
				++begin;

			wstring dot(L".");
			auto end = std::find_end(begin, filename.end(), dot.begin(), dot.end());

			return wstring(begin, end);
		}
		wstring GetFileExtension(const wstring& filename)
		{
			wstring dot(L".");
			auto end = std::find_end(filename.begin(), filename.end(), dot.begin(), dot.end());
			if (end == filename.end())
				throw EngineException("Filename has no extension.");

			return wstring(end + 1, filename.end());
		}
		wstring GetFilePath(const wstring& filename)
		{
			wstring searchString(L"/");

			auto location = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
			if (location == filename.end())
				return L"";

			return wstring(filename.begin(), location + 1);
		}

		template<typename ContainerType>
		void ReadData(const wstring& filename, ContainerType& buffer)
		{
			using namespace std;

			using ValueType = typename ContainerType::value_type;

			// Open file for reading in binary mode, and seek to the end of file immediately:
			basic_ifstream<ValueType> file(filename, ios::in | ios::binary | ios::ate);
			if (!file.good())
				throw runtime_error("Couldn't open file " + WStringToString(filename));

			// Get size of file and seek to the begin of file:
			auto size = file.tellg();
			file.seekg(0, ios::beg);

			// Read content of file:
			buffer.resize(static_cast<uint32_t>(size));
			file.read(buffer.data(), size);
			if (!file.good())
				throw runtime_error("Error while reading file " + WStringToString(filename));
		}
		template<typename ContainerType>
		void WriteData(const wstring& filename, const ContainerType& buffer)
		{
			using namespace std;

			// Open file for writing in binary mode:
			ofstream file(WStringToString(filename), ios::out | ios::binary);
			if (!file.good())
				throw runtime_error("Couldn't open file " + WStringToString(filename));

			// Write contents to file:
			file.write(buffer.data(), buffer.size());
			if (!file.good())
				throw runtime_error("Error while writing file " + WStringToString(filename));
		}

		template<typename FunctionType, typename... ArgumentsType>
		std::future<typename std::result_of<FunctionType(ArgumentsType...)>::type> RunAsync(FunctionType&& function, ArgumentsType&&... arguments)
		{
			return std::async(std::launch::async, std::forward<FunctionType>(function), std::forward<ArgumentsType>(arguments)...);
		}
	}
}
