#pragma once

#include <future>
#include <fstream>

namespace Common
{
	namespace Helpers
	{
		bool FileExists(const std::wstring& filename);
		std::wstring GetFilename(const std::wstring& filename);
		std::wstring GetFileExtension(const std::wstring& filename);
		std::wstring GetFilePath(const std::wstring& filename);

		std::wstring StringToWString(const std::string& str);
		std::string WStringToString(const std::wstring& wstr);

		template<typename ContainerType>
		void ReadData(const std::wstring& filename, ContainerType& buffer)
		{
			using namespace std;

			using ValueType = typename ContainerType::value_type;

			// Open file for reading in binary mode, and seek to the end of file immediately:
			basic_ifstream<ValueType> file(filename, ios::in | ios::binary | ios::ate);
			if (!file.good())
				throw runtime_error("Couldn't open file " + Helpers::WStringToString(filename));

			// Get size of file and seek to the begin of file:
			auto size = file.tellg();
			file.seekg(0, ios::beg);

			// Read content of file:
			buffer.resize(static_cast<uint32_t>(size));
			file.read(buffer.data(), size);
			if (!file.good())
				throw runtime_error("Error while reading file " + Helpers::WStringToString(filename));
		}
		template<typename ContainerType>
		void WriteData(const std::wstring& filename, const ContainerType& buffer)
		{
			using namespace std;

			// Open file for writing in binary mode:
			ofstream file(WStringToString(filename), ios::out | ios::binary);
			if (!file.good())
				throw runtime_error("Couldn't open file " + Helpers::WStringToString(filename));

			// Write contents to file:
			file.write(buffer.data(), buffer.size());
			if (!file.good())
				throw runtime_error("Error while writing file " + Helpers::WStringToString(filename));
		}

		template<typename FunctionType, typename... ArgumentsType>
		std::future<typename std::result_of<FunctionType(ArgumentsType...)>::type> RunAsync(FunctionType&& function, ArgumentsType&&... arguments)
		{
			return std::async(std::launch::async, std::forward<FunctionType>(function), std::forward<ArgumentsType>(arguments)...);
		}
	}
}
