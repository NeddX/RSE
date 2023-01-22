#ifndef REXCEPTION_H
#define REXCEPTION_H

#include <chrono>
#include <string>
#include <format>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

namespace RSE
{
	enum class RExceptionType
	{
		
	};

	class RException
	{
	private:
		std::string m_ErrorMessage;
		int m_ErrorCode;

	public:
		RException(const char* message)
		{
			m_ErrorCode = -1;
			auto now = std::chrono::system_clock::now();
			auto timeNow = std::chrono::system_clock::to_time_t(now);
			std::string dateStr = std::ctime(&timeNow);
			m_ErrorMessage = std::format("[DATE] An Exception Occured: {}", message);
		}
		const char* Message() const
		{
			return m_ErrorMessage.c_str();
		}
		int ErrorCode() const
		{
			return m_ErrorCode;
		}
	};
}

#endif // REXCEPTION_H