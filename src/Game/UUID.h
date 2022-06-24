#pragma once

class UUID
{
public:
	UUID();
	UUID(std::string&& content);

	std::string WithHyphens();

	operator bool() { return !m_Content.empty(); }
	inline std::string AsString() { return m_Content; }

private:
	std::string m_Content;
};
