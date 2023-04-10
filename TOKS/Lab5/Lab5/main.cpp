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
			throw "НЕПРАВИЛЬНО!";
		}
	}
	auto x = std::stoi(index);
	if (x < 1 || x > nrOfComp)
	{
		throw "НЕПРАВИЛЬНО!";
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

	std::cout << "Прочитать информацию из файла или хотите её ввести самостоятельно? (1-файл/2-своё) ";
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
		std::cout << "Количество компьютеров: "; std::cin >> nrOfComputers;
		std::cout << "\n";
		for (int i = 0; i < nrOfComputers; i++)
		{
			std::cout << "Компьютер " << i + 1 << ": ";
			std::string ip;
			std::cin >> ip;
			computers.push_back(ip);
		}
	}
	PrintComputers(computers);
	while (true)
	{
		std::cout << "Индекс ip-адреса источника компьютера: ";
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
		std::cout << "Индекс IP-адреса компьютера назначения: ";
		std::cin >> indexOfCompDestination;
		try
		{
			CheckExistingComputers(indexOfCompDestination, nrOfComputers);
			if (indexOfCompDestination.compare(indexOfCompSource) == 0)
			{
				throw "IНЕПРАВИЛЬНО!";
			}
			break;
		}
		catch (const char* message)
		{
			std::cout << message << "\n";
		}
	}
	std::cout << "По часовой стрелке: (1-да/2-нет) "; std::cin >> cronologic;
	std::cout << "Сообщение: "; std::cin >> message;

	Token token(computers[std::stoi(indexOfCompSource) - 1], computers[std::stoi(indexOfCompDestination) - 1], cronologic, message);
	std::cout << "\n";
	while (true)
	{
		setlocale(LC_ALL, "rus");

		token.SendMessage(computers);
		std::cout << "\n\nХотите отправить другое сообщение? (1-да/0-нет)";
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
				std::cout << "\n\nИндекс ip-адреса источника компьютера: ";
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
				std::cout << "Индекс IP-адреса компьютера назначения: ";
				std::cin >> indexOfCompDestination;
				try
				{
					CheckExistingComputers(indexOfCompDestination, nrOfComputers);
					if (indexOfCompDestination.compare(indexOfCompSource) == 0)
					{
						throw "Неправильно!";
					}
					break;
				}
				catch (const char* message)
				{
					std::cout << message << "\n";
				}
			}

			std::cout << "Сообщение: "; std::cin >> message;

			token.SetIpSource(computers[std::stoi(indexOfCompSource) - 1]);
			token.SetIpDestination(computers[std::stoi(indexOfCompDestination) - 1]);
			token.SetMessage(message);
		}
	}

	std::cout << "\n";
}