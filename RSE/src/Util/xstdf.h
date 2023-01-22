#ifndef XSTDF_H
#define XSTDF_H

#include <sdafx.h>

namespace xstd
{
	class File
	{
	public:
		static char* EncodeStrToBHS8(const char* str)
		{
			size_t size = strlen(str);
			char* encStr = new char[size + 1];
			memset(encStr, 0, size + 1);
			for (int i = 0; i < size; ++i) encStr[i] = str[i] ^ 255;
			return encStr;
		}
		static char* DecodeBHS8ToStr(const char* encStr)
		{
			size_t size = strlen(encStr);
			char* str = new char[size + 1];
			memset(str, 0, size + 1);
			for (int i = 0; i < size; ++i) str[i] = encStr[i] ^ 255;
			return str;
		}
		static size_t FileSize(const char* file)
		{
			std::ifstream fs(file, std::ios::binary);
			if (fs.is_open())
			{
				fs.seekg(0, std::ios_base::end);
				size_t size = fs.tellg();
				fs.close();
				return size;
			}
			else
			{
				std::cerr << "Bad file '" << file << "'." << std::endl;
				return 0;
			}
		}
	};
}

#endif // XSTDF_H