#include<iostream>
#include<bit>
#include<Bits.h>
#include<errno.h>
#define _POSIX_C_SOURCE 199309L
#include<time.h>
#include<ctime>
#include <Windows.h>

#define _UNISTD_H    1


using namespace std;

void my_sleep(unsigned msec)
{
	setlocale(LC_ALL, "rus");
	struct timespec req, rem;
	int err;                                                              
	rem.tv_sec = 0;
	rem.tv_nsec = 0;
	req.tv_sec = msec / 1000;
	req.tv_nsec = (msec % 1000) * 1000000;
	while ((req.tv_sec != 0) || (req.tv_nsec != 0))
	{
	
		err = errno;
		// Interrupted;  continue
		if (err == EINTR)
		{
			req.tv_sec = rem.tv_sec;
			req.tv_nsec = rem.tv_nsec;
		}
		// Unhandleable error (EFAULT (bad pointer), EINVAL (bad timeval in tv_nsec), or ENOSYS (function not supported))
		break;
	}
}

void purealoha()
{
	setlocale(LC_ALL, "rus");

	int i, n, k = 1, time[10], current_time = 1, sent[10], ready[10], in[10], i1 = 0, count = 0, coll = 0, s = 0;
	system("cls");
	cout << "����� ����� ���������� 2 ���\n\n";
	cout << "������� ���������� ������������:\t";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		time[i] = (rand() % 10 + 1);
		sent[i] = 0;
		ready[i] = 0;
		cout << "����������� " << i + 1 << " �������� ���� ������� " << time[i] << "\n";
		my_sleep(2);
	}
	while (s == 0)
	{
		for (i = 0; i < n; i++)
		{
			if (current_time == time[i] && sent[i] == 0)
			{
				ready[i] = 1;
			}
		}
		for (i = 0; i < n; i++)
		{
			if (ready[i] == 1 && count == 0)
			{
				count = 1;
				in[i1] = i;
				i1++;
			}
			else if (ready[i] == 1 && count == 1)
			{
				coll = 1;
				in[i1] = i;
				i1++;
			}
		}
		for (i = 0; i < n; i++)
		{
			if (time[i] == current_time + 1 && sent[i] == 0 && count == 1)
			{
				in[i1] = i;
				i1++;
				coll = 1;
			}
		}
		if (coll == 1)
		{
			cout << "�������� ��������� � ������������\n";
			for (i = 0; i <= i1 - 1; i++)
			{
				time[in[i]] = time[in[i]] + k;
				cout << "\t" << in[i] + 1;
				k = k * 2;
			}
			if (i1 > 1)
				cout << "\n����� ����� ��� ������������ � ���������:\n";
			for (i = 0; i <= i1 - 1; i++)
			{
				cout << "Sender " << in[i] + 1 << " -> " << time[in[i]] << "\n";
				my_sleep(1);
			}
		}
		if (coll != 1 && count == 1)
		{
			sent[in[i1 - 1]] = 1;
			cout << "����������� " << in[i1 - 1] + 1 << " �������� �����\n\n";
			my_sleep(2);
			s = 1;
		}
		for (i = 0; i < n; i++)
		{
			if (sent[i] == 0)
				s = 0;
		}
		current_time++;
		for (i = 0; i < n; i++)
		{
			ready[i] = 0;
		}
		count = 0;
		coll = 0;
		i1 = 0;
	}
}

void slottedaloha()
{
	setlocale(LC_ALL, "rus");

	int i, n, k = 1, time[10], current_time = 1, sent[10], ready[10], in[10], i1 = 0, count = 0, coll = 0, s = 0;
	system("cls");
	cout << "����� ����� ���������� 1 ���\n\n";
	cout << "������� ���������� ������������:\t";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		time[i] = (rand() % 10 + 1);
		sent[i] = 0;   ready[i] = 0;
		cout << "����������� " << i + 1 << " �������� ���� ������� " << time[i] << "\n";
		my_sleep(2);
	}
	while (s == 0)
	{
		for (i = 0; i < n; i++)
		{
			if (current_time == time[i] && sent[i] == 0)
			{
				ready[i] = 1;
			}
		}
		for (i = 0; i < n; i++)
		{
			if (ready[i] == 1 && count == 0)
			{
				count = 1;
				in[i1] = i;
				i1++;
			}
			else if (ready[i] == 1 && count == 1)
			{
				coll = 1;
				in[i1] = i;
				i1++;
			}
		}
		if (coll == 1)
		{
			cout << "\n�������� ��������� � �����������\n";
			for (i = 0; i <= i1 - 1; i++)
			{
				time[in[i]] = time[in[i]] + k;
				cout << "\t" << in[i] + 1;
				k = k * 2;
			}
			if (i1 > 1)
				cout << "\n����� ����� ��� ������������ � ���������:\t";
			for (i = 0; i <= i1 - 1; i++)
			{
				cout << "\nSender " << in[i] + 1 << " -> " << time[in[i]];
				my_sleep(1);
			}
		}
		if (coll != 1 && count == 1)
		{
			sent[in[i1 - 1]] = 1;
			cout << "\n����������� " << in[i1 - 1] + 1 << " �������� �����";
			my_sleep(2);
			s = 1;
		}
		for (i = 0; i < n; i++)
		{
			if (sent[i] == 0)
				s = 0;
		}
		current_time++;
		for (i = 0; i < n; i++)
		{
			ready[i] = 0;
		}
		count = 0;
		coll = 0;
		i1 = 0;
	}
}

void csmaca()
{
	setlocale(LC_ALL, "rus");

	int i, n, sent[10], c = -1, ready[10], exp = 1, f[10], current_time = 1, k = 1, s = 0, busy = 0;
	float time[10];
	system("cls");
	cout << "������� ������������:\t";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		time[i] = (rand() % 10 + 1);
		sent[i] = 0;  ready[i] = 0;   f[i] = 0;
		cout << "\n����������� " << i + 1 << " �������� ���� ������� " << time[i];
		my_sleep(3);
	}
	while (s == 0)
	{
		for (i = 0; i < n; i++)
		{
			if (current_time == time[i] && sent[i] == 0 && busy == 0 && ready[i] == 0)
			{
				ready[i] = 1;
				time[i] = time[i] + 0.96;
				cout << "\n����������� " << i + 1 << " ����� ����� � ������� IFS";
				cout << "\n����� �������, �����, � ������� �� �������� ����, ����� " << time[i];
				my_sleep(2);
				c = current_time;
			}
			else if (current_time == time[i] && sent[i] == 0 && ready[i] == 0)
			{
				time[i] = time[i] + pow(2, exp);
				exp++;
				cout << "\n��������� ����� ��� �����, ����� �������� ����������� " << i + 1 << " ���� �������� �� ����� " << time[i] << " ����� �������� ��������";
				my_sleep(3);
			}
			else if (current_time > time[i] && sent[i] == 0 && f[i] == 1)
			{
				cout << "\n����������� " << i + 1 << " �������� ���� ������� " << time[i];
				sent[i] = 1;
				busy = 1;
				s = 1;
				my_sleep(2);
			}
		}
		if (busy == 0 && c == current_time - 1)
		{
			for (i = 0; i < n; i++)
			{
				if (ready[i] == 1 && sent[i] == 0)
				{
					int r = pow(2, k) - 1;
					r = rand() % (r + 1);
					time[i] = time[i] + r;
					cout << "\n\n���������� ������, � ������� ������� ����������� " << i + 1 << " ����� �����, ����� " << r << "\n\n";
					my_sleep(2);
					f[i] = 1;
				}
			}
		}
		else if (c == current_time - 1)
		{
			time[i] = time[i] + pow(2, exp);
			exp++;
			cout << "\n��������� ����� ��� �����, ����� �������� ����������� " << i + 1 << " ���� �������� �� ����� " << time[i] << " ����� �������� ��������";
			my_sleep(3);
		}
		for (i = 0; i < n; i++)
		{
			if (sent[i] == 0)
				s = 0;
		}
		current_time++;
		busy = 0;
	}
}

void csmacd()
{
	setlocale(LC_ALL, "rus");

	int i, n, time[10], exp = 1, size[10], current_time = 1, sent[10], ready[10], in[10], i1 = 0, count = 0, coll = 0, busy = 0, c{};
	system("cls");
	cout << "����� ����� ���������� 1 �������\n";
	cout << "������� ���������� ������������:\t";
	cin >> n;
	for (int u = 0; u < n; u++) {
		cout << "������� ���������� ������ ��� ����������� " << u + 1 << "\t";
		cin >> size[u];
	}
	for (int i = 0; i < n; i++)
	{
		time[i] = (rand() % 10 + 1);
		sent[i] = 0;   ready[i] = 0;
		cout << "����������� " << i + 1 << " �������� ���� ������� " << time[i] << "\n";
		my_sleep(2);
	}
	while (current_time <= 20)
	{
		for (i = 0; i < n; i++)
		{
			if (current_time == time[i] && sent[i] == 0 && busy == 0)
			{
				ready[i] = 1;
				busy = 1;
				c = current_time + size[i];
				cout << "\n\n� ��� ����� " << current_time << " ����������� " << i + 1 << " ���������� ���� � ������� ��������� " << size[i] << " ������ �� ��� ���, ���� " << c;
				my_sleep(2);
			}
			else if (current_time == time[i])
			{
				time[i] = time[i] + pow(2, exp);
				exp++;
				cout << "\n��������� ����� ��� �����, ����� �������� ����������� " << i + 1 << " ���� �������� �� ����� " << time[i] << " �����	�������� ��������";
				my_sleep(2);
			}
		}
		if (coll != 1 && count == 1)
		{
			sent[in[i1 - 1]] = 1;
			cout << "\n����������� " << in[i1 - 1] + 1 << " �������� �����";
			my_sleep(2);
		}
		current_time++;
		if (c == current_time)
			busy = 0;
		for (i = 0; i < n; i++)
		{
			ready[i] = 0;
		}
		count = 0;
		coll = 0;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	int ch, a;
	do
	{
		system("cls");
		cout << "1. ������ Aloha\n";
		cout << "2. ������������� Aloha\n";
		cout << "3. CSMA CD\n";
		cout << "4. CSMA CA\n";
		cout << "5. �����\n";
		cout << "������� ��� �������:\t";
		cin >> ch;
		switch (ch)
		{
		case 1: purealoha();
			break;
		case 2: slottedaloha();
			break;
		case 3: csmacd();
			break;
		case 4: csmaca();
			break;
		case 5: exit(0);
		default: cout << "����������� �����";
			break;
		}
		cout << "\n\n������� ����������??(�� ����������:/) ����� 1 ��� ��:\t";
		cin >> a;
	} while (a == 1);

	return 0;
}