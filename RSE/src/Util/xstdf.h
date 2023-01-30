#ifndef XSTDF_H
#define XSTDF_H

#include <sdafx.h>

namespace xstdf
{
	struct file
	{
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

	struct crypto
	{
		static uint64_t FNV1aHash(const std::string& str)
		{
			static const uint64_t FNV_offset_basis = 14695981039346656037ull;
			static const uint64_t FNV_prime = 1099511628211ull;
			uint64_t hash = FNV_offset_basis;
			for (char c : str)
			{
				hash ^= c;
				hash *= FNV_prime;
			}
			return hash;
		}
	};
}

#endif // XSTDF_H