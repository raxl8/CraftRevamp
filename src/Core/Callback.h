#pragma once

template<typename... Args>
class Callback
{
public:
	void operator ()(Args... args)
	{
		for (auto subscription : m_Subscriptions)
		{
			std::invoke(subscription, args...);
		}
	}

	void Subscribe(std::function<void(Args...)> func)
	{
		m_Subscriptions.push_back(func);
	}

private:
	std::vector<std::function<void(Args...)>> m_Subscriptions;
};
