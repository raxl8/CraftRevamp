#include "pch.h"

#include "UUID.h"

UUID::UUID(std::string&& content)
	: m_Content(std::move(content))
{
}

UUID::UUID()
	: m_Content("")
{
}

std::string UUID::WithHyphens()
{
	std::string result;
	result.reserve(m_Content.size() + 4);

	std::string_view contentView(m_Content);
	result.append(contentView.substr(0, 8));
	result.append("-");

	for (int i = 0; i < 3; i++)
	{
		result.append(contentView.substr(8 + 4 * i, 4));
		result.append("-");
	}
	
	result.append(contentView.substr(8 + 4 * 3, 12));

	return result;
}
