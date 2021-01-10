#include "shared/memory/procedure.hpp"

namespace shared::memory
{
	std::string ToLower(std::string data)
	{
		auto dest = ""s;

		dest.resize(data.size());
		std::transform(data.begin(), data.end(), dest.begin(), ::tolower);

		return dest;
	}

	std::wstring ToLower(std::wstring data)
	{
		auto dest = L""s;

		dest.resize(data.size());
		std::transform(data.begin(), data.end(), dest.begin(), ::towlower);

		return dest;
	}

	std::string ToUpper(std::string data)
	{
		auto dest = ""s;

		dest.resize(data.size());
		std::transform(data.begin(), data.end(), dest.begin(), ::toupper);

		return dest;
	}

	std::wstring ToUpper(std::wstring data)
	{
		auto dest = L""s;

		dest.resize(data.size());
		std::transform(data.begin(), data.end(), dest.begin(), ::towupper);

		return dest;
	}

	std::string ToString(std::wstring data)
	{
		if (data.empty())
			return std::string();

		int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &data[0], data.size(), NULL, 0, NULL, NULL);
		std::string ret = std::string(size, 0);
		WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &data[0], data.size(), &ret[0], size, NULL, NULL);

		return ret;
	}

	std::wstring ToWString(std::string data)
	{
		if(data.empty())
			return std::wstring();

		size_t len = data.length() + 1;
		std::wstring ret = std::wstring(len, 0);

		int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &data[0], data.size(), &ret[0], len);
		ret.resize(size);

		return ret;
	}
}