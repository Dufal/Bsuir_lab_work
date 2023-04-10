#pragma once
#include <iostream>
#include <vector>
#include <string>

class Token
{
public:
	Token(std::string ipSource, std::string ipDestination, bool cronologic, std::string message);

	void SetIpSource(const std::string& ipSource);

	void SetIpDestination(const std::string& ipDestination);

	void SetMessage(const std::string& message);
	std::string GetMessage() const;

	void SetFree(const bool& free);
	bool GetFree() const;

	void AddInHistory(const std::string& message);

	void Print() const;

	void ClearData();

	void SendMessage(const std::vector<std::string>& computers);

private:
	std::string m_ipSource;
	std::string m_ipDestination;
	bool m_cronologic;
	std::string m_message;
	bool m_atDestination;
	bool m_free;
	std::vector<std::string> m_history;
};
