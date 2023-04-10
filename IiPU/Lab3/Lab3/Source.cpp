#include <iostream>
#include <windows.h>



int main() {
	int n;
	bool Flag;
	DWORD dr = GetLogicalDrives(); // ������� ���������� ������� �����

	for (int x = 0; x < 26; x++) // ���������� ������ �� �����
	{
		n = ((dr >> x) & 1); // ����� �������� �������� ����
		if (n) // ���� ������� - ���� � ������� x ����
		{
			AnsiString dl = AnsiString((char)(65 + x)); // �������� ������ �����
			AnsiString path = AnsiString((char)(65 + x)) + ":\\";

			AnsiString out = "���� " + dl + "\n";

			// ����� ����� ������ �� ����������
			WORD OldErrorMode;
			OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS); // ������� ����� ������
			bool ready = DirectoryExists(path); // ������c� ������� �������� ����������
			SetErrorMode(OldErrorMode); // ��������������� ������ ����� ������ ������

			if (ready)
			{
				UINT drive_type = GetDriveType((dl + ":\\").c_str()); // ����� ��� �����
				out += "\n��� �����: ";
				if (drive_type == DRIVE_REMOVABLE) out += "REMOVABLE";
				else if (drive_type == DRIVE_FIXED)	 out += "FIXED";
				else if (drive_type == DRIVE_REMOTE)	out += "REMOTE";
				else if (drive_type == DRIVE_CDROM)	 out += "CD-ROM";
				else if (drive_type == DRIVE_RAMDISK)   out += "RAMDISK";
				else out += "�����������_���\n";

				// ���� ��� HDD - ������������ ���������� � ���
				if (drive_type == DRIVE_FIXED)
				{
					unsigned __int64 FreeBytesAvailable;
					unsigned __int64 TotalNumberOfBytes;
					unsigned __int64 TotalNumberOfFreeBytes;
					char drive_label[30];
					char drive_fat[30];
					DWORD drive_sn;
					DWORD drive_name_size = sizeof(drive_label);

					// �������� ������ � ��������
					Flag = ::GetDiskFreeSpaceEx(path.c_str(),
						(PULARGE_INTEGER)&FreeBytesAvailable,
						(PULARGE_INTEGER)&TotalNumberOfBytes,
						(PULARGE_INTEGER)&TotalNumberOfFreeBytes
					);
					if (Flag)
					{
						out += "\n�������� �� �����: " + AnsiString(TotalNumberOfFreeBytes) + "\n";
						out += "����� �� �����: " + AnsiString(TotalNumberOfBytes) + "\n";
					}
					else
					{
						out += "������ � GetDiskFreeSpaceEx\n";
					}

					// �������� �����, ���������� ����� � ��.
					Flag = GetVolumeInformation(path.c_str(),
						drive_label,
						sizeof(drive_label),
						&drive_sn,
						&drive_name_size,
						NULL,
						drive_fat,
						sizeof(drive_fat)
					);
					if (Flag)
					{
						out += "\n����� ����: " + AnsiString(drive_label) + "\n";
						out += "���.�����: " + AnsiString(drive_sn) + "\n";
						out += "�������� �������: " + AnsiString(drive_fat) + "\n";
					}
					else
					{
						out += "������ � GetVolumeInformation\n";
					}
				}
			}
			else
			{
				out += "�� �����";
			}
			ShowMessage(out);
		}
	}
}