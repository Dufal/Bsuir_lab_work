#include "Token.h"

Token::Token(std::string ipSource, std::string ipDestination, bool cronologic, std::string message) :
	m_ipSource(ipSource), m_ipDestination(ipDestination), m_cronologic(cronologic), m_message(message), m_atDestination(false), m_free(true) {}

void Token::SetIpSource(const std::string& ipSource)
{
	m_ipSource = ipSource;
}

void Token::SetIpDestination(const std::string& ipDestination)
{
	m_ipDestination = ipDestination;
}

void Token::SetMessage(const std::string& message)
{
	m_message = message;
}

std::string Token::GetMessage() const
{
	return m_message;
}

void Token::SetFree(const bool& free)
{
	m_free = free;
}

bool Token::GetFree() const
{
	return m_free;
}

void Token::AddInHistory(const std::string& message)
{
	m_history.push_back(message);
}

void Token::Print() const
{
	setlocale(LC_ALL, "rus");
	std::cout << "\n�������� IP ������: " << m_ipSource;
	std::cout << "\nIP ����� ����������: " << m_ipDestination;
	std::cout << "\n������������������: " << m_cronologic;
	std::cout << "\n���������: " << m_message;
	std::cout << "\n�� ����������: " << m_atDestination;
	std::cout << "\n��������: " << m_free;
	std::cout << "\n�������: ";
	for (std::string s : m_history)
	{
		std::cout << s << " ";
	}

	std::cout << "\n\n";
}

void Token::ClearData()
{
	m_ipSource.clear();
	m_ipDestination.clear();
	m_cronologic = true;
	m_message.clear();
	m_atDestination = false;
	m_free = true;
	m_history.clear();
}

void Token::SendMessage(const std::vector<std::string>& computers)
{
	int loop = 0;
	bool ok = true;
	int indexOfComputers = 0;
	while (ok)
	{
		if (computers[indexOfComputers].compare(m_ipSource) == 0)
		{
			m_free = false;
		}
		else
		{
			std::cout << "\n� ���������: " << indexOfComputers + 1 << ": " << computers[indexOfComputers];
			Print();
			indexOfComputers++;
		}

		while (!m_free)
		{
			if (indexOfComputers == computers.size())
			{
				indexOfComputers = 0;
				loop++;
			}

			std::cout << "\n� ���������: " << indexOfComputers + 1 << ": " << computers[indexOfComputers];


			if (computers[indexOfComputers].compare(m_ipDestination) == 0)
			{
				std::cout << " " << m_message;
				m_atDestination = true;
			}

			m_history.push_back(computers[indexOfComputers]);

			if (computers[indexOfComputers].compare(m_ipSource) == 0 && loop > 0 && m_atDestination == true)
			{
				ClearData();
				ok = false;
			}

			Print();
			indexOfComputers++;
		}
	}
}