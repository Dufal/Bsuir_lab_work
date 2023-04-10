#include <iostream>
#include <windows.h>



int main() {
	int n;
	bool Flag;
	DWORD dr = GetLogicalDrives(); // функция возвращает битовую маску

	for (int x = 0; x < 26; x++) // проходимся циклом по битам
	{
		n = ((dr >> x) & 1); // узнаём значение текущего бита
		if (n) // если единица - диск с номером x есть
		{
			AnsiString dl = AnsiString((char)(65 + x)); // получаем литеру диска
			AnsiString path = AnsiString((char)(65 + x)) + ":\\";

			AnsiString out = "Диск " + dl + "\n";

			// здесь узнаём готово ли устройство
			WORD OldErrorMode;
			OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS); // убираем показ ошибок
			bool ready = DirectoryExists(path); // пытаемcя открыть корневую директорию
			SetErrorMode(OldErrorMode); // восстанавливаем старый режим показа ошибок

			if (ready)
			{
				UINT drive_type = GetDriveType((dl + ":\\").c_str()); // узнаём тип диска
				out += "\nТип диска: ";
				if (drive_type == DRIVE_REMOVABLE) out += "REMOVABLE";
				else if (drive_type == DRIVE_FIXED)	 out += "FIXED";
				else if (drive_type == DRIVE_REMOTE)	out += "REMOTE";
				else if (drive_type == DRIVE_CDROM)	 out += "CD-ROM";
				else if (drive_type == DRIVE_RAMDISK)   out += "RAMDISK";
				else out += "НЕИЗВЕСТНЫЙ_ТИП\n";

				// если это HDD - заприашиваем информацию о нем
				if (drive_type == DRIVE_FIXED)
				{
					unsigned __int64 FreeBytesAvailable;
					unsigned __int64 TotalNumberOfBytes;
					unsigned __int64 TotalNumberOfFreeBytes;
					char drive_label[30];
					char drive_fat[30];
					DWORD drive_sn;
					DWORD drive_name_size = sizeof(drive_label);

					// получаем данные о размерах
					Flag = ::GetDiskFreeSpaceEx(path.c_str(),
						(PULARGE_INTEGER)&FreeBytesAvailable,
						(PULARGE_INTEGER)&TotalNumberOfBytes,
						(PULARGE_INTEGER)&TotalNumberOfFreeBytes
					);
					if (Flag)
					{
						out += "\nСвободно на диске: " + AnsiString(TotalNumberOfFreeBytes) + "\n";
						out += "Всего на диске: " + AnsiString(TotalNumberOfBytes) + "\n";
					}
					else
					{
						out += "Ошибка в GetDiskFreeSpaceEx\n";
					}

					// получаем метку, серинийный номер и пр.
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
						out += "\nМетка тома: " + AnsiString(drive_label) + "\n";
						out += "Сер.номер: " + AnsiString(drive_sn) + "\n";
						out += "Файловая система: " + AnsiString(drive_fat) + "\n";
					}
					else
					{
						out += "Ошибка в GetVolumeInformation\n";
					}
				}
			}
			else
			{
				out += "НЕ ГОТОВ";
			}
			ShowMessage(out);
		}
	}
}