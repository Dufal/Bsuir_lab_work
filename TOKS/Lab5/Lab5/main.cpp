#include <iostream>
#include <vector>
#include <fstream>

#include "Token.h"

void PrintComputers(const std::vector<std::string> computers)
{
	setlocale(LC_ALL, "rus");

	for (int i = 0; i < computers.size(); i++)
	{
		std::cout << i + 1 << ". " << computers[i] << "\n";
	}
}

void CheckExistingComputers(std::string index, int nrOfComp)
{
	setlocale(LC_ALL, "rus");

	for (auto c : index)
	{
		if (!std::isdigit(c))
		{
			throw "�����������!";
		}
	}
	auto x = std::stoi(index);
	if (x < 1 || x > nrOfComp)
	{
		throw "�����������!";
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	std::ifstream f("computers.txt");

	int nrOfComputers;
	std::vector<std::string> computers;
	std::string indexOfCompSource;
	std::string indexOfCompDestination;
	bool cronologic;
	std::string message;

	std::cout << "��������� ���������� �� ����� ��� ������ � ������ ��������������? (1-����/2-���) ";
	int answer;
	std::cin >> answer;
	if (answer == 1)
	{
		f >> nrOfComputers;
		for (int i = 0; i < nrOfComputers; i++)
		{
			std::string ip;
			f >> ip;
			computers.push_back(ip);
		}
	}
	else
	{
		std::cout << "���������� �����������: "; std::cin >> nrOfComputers;
		std::cout << "\n";
		for (int i = 0; i < nrOfComputers; i++)
		{
			std::cout << "��������� " << i + 1 << ": ";
			std::string ip;
			std::cin >> ip;
			computers.push_back(ip);
		}
	}
	PrintComputers(computers);
	while (true)
	{
		std::cout << "������ ip-������ ��������� ����������: ";
		std::cin >> indexOfCompSource;
		try
		{
			CheckExistingComputers(indexOfCompSource, nrOfComputers);
			break;
		}
		catch (const char* message)
		{
			std::cout << message << "\n";
		}
	}
	while (true)
	{
		std::cout << "������ IP-������ ���������� ����������: ";
		std::cin >> indexOfCompDestination;
		try
		{
			CheckExistingComputers(indexOfCompDestination, nrOfComputers);
			if (indexOfCompDestination.compare(indexOfCompSource) == 0)
			{
				throw "I�����������!";
			}
			break;
		}
		catch (const char* message)
		{
			std::cout << message << "\n";
		}
	}
	std::cout << "�� ������� �������: (1-��/2-���) "; std::cin >> cronologic;
	std::cout << "���������: "; std::cin >> message;

	Token token(computers[std::stoi(indexOfCompSource) - 1], computers[std::stoi(indexOfCompDestination) - 1], cronologic, message);
	std::cout << "\n";
	while (true)
	{
		setlocale(LC_ALL, "rus");

		token.SendMessage(computers);
		std::cout << "\n\n������ ��������� ������ ���������? (1-��/0-���)";
		int answer;
		std::cin >> answer;
		if (answer == 0)
		{
			break;
		}
		else
		{
			PrintComputers(computers);
			while (true)
			{
				std::cout << "\n\n������ ip-������ ��������� ����������: ";
				std::cin >> indexOfCompSource;
				try
				{
					CheckExistingComputers(indexOfCompSource, nrOfComputers);
					break;
				}
				catch (const char* message)
				{
					std::cout << message << "\n";
				}
			}

			while (true)
			{
				std::cout << "������ IP-������ ���������� ����������: ";
				std::cin >> indexOfCompDestination;
				try
				{
					CheckExistingComputers(indexOfCompDestination, nrOfComputers);
					if (indexOfCompDestination.compare(indexOfCompSource) == 0)
					{
						throw "�����������!";
					}
					break;
				}
				catch (const char* message)
				{
					std::cout << message << "\n";
				}
			}

			std::cout << "���������: "; std::cin >> message;

			token.SetIpSource(computers[std::stoi(indexOfCompSource) - 1]);
			token.SetIpDestination(computers[std::stoi(indexOfCompDestination) - 1]);
			token.SetMessage(message);
		}
	}

	std::cout << "\n";
}