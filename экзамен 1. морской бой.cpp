#include <iostream>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

const int size = 11; //размер массивов игоровых полей
unsigned char PlayerMap[::size][::size], ComputerMap[::size][::size], VisibleComputerMap[::size][::size]; //массивы (игровые поля)
unsigned char Empty = 176, Ship = 219, Hit = 'X', Missed = 249; //данные для заполнения ячеек в массивах
int CoordX, CoordY, CoordX2, CoordY2, CoordX3, CoordY3; //переменные для записи координат подбитых кораблей до полного уничтожения (для выстрела компьютера (врага))
int Coord2X, Coord2Y, Coord2X2, Coord2Y2, Coord2X3, Coord2Y3, CountFirst1, CountFirst2; //переменные для записи координат подбитых кораблей до полного уничтожения (для выстрела компьютера (игорока))
int EnemyChoise, PlayerChoise; //переменные выбора сложностей компьютера
bool TestDestroy = false, TestDestroy2 = false;//переменные проверки полного уничтожения корабля

//прототипы функций
void EndGame(unsigned char PlayerMap[][::size], unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size]);
void ManualShip4Parts(unsigned char mass1[][::size]);
void ManualShip3Parts(unsigned char mass1[][::size]);
void ManualShip2Parts(unsigned char mass1[][::size]);
void ManualShip1Parts(unsigned char mass1[][::size]);
void PlayerShot(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size]);
void ShotCompEasyAsPlayer(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size]);
void ShotCompHardAsPlayer(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size]);

//ф-ция отображающая название игры при запуске
void LogoAnimated()
{
	cout << "  __  " << endl;
	cout << "/' _/  " << endl;
	cout << "`._`.  " << endl;
	cout << "|___/ " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___  " << endl;
	cout << "/' _/ | __| " << endl;
	cout << "`._`. | _|  " << endl;
	cout << "|___/ |___| " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __         " << endl;
	cout << "/' _/ | __|  /  \\         " << endl;
	cout << "`._`. | _|  | /\\ |           " << endl;
	cout << "|___/ |___| |_||_|          " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __      " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  " << endl;
	cout << "`._`. | _|  | /\\ |           | -<   " << endl;
	cout << "|___/ |___| |_||_|           |__/ " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __    __     " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |    " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __    __    _____   " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  |_   _|  " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |   | |      " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   |_|     " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __    __    _____   _____    " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  |_   _| |_   _| " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |   | |     | |    " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   |_|     |_|   " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __    __    _____   _____   _       " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  |_   _| |_   _| | |    " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |   | |     | |   | |_   " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   |_|     |_|   |___| " << endl;
	Sleep(50);
	system("cls");
	cout << "  __   ___    __              __    __    _____   _____   _     ___  " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  |_   _| |_   _| | |   | __| " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |   | |     | |   | |_  | _|  " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   |_|     |_|   |___| |___| " << endl;
}

//фун-ция отбражения логотип игры (без анимации)
void LogoStatic()
{
	cout << "  __   ___    __              __    __    _____   _____   _     ___  " << endl;
	cout << "/' _/ | __|  /  \\            |  \\  /  \\  |_   _| |_   _| | |   | __| " << endl;
	cout << "`._`. | _|  | /\\ |           | -< | /\\ |   | |     | |   | |_  | _|  " << endl;
	cout << "|___/ |___| |_||_|           |__/ |_||_|   |_|     |_|   |___| |___| " << endl;
}

//ф-ция для отображеения массива в консоль
void ShowScreen(unsigned char mass1[][::size])
{
	for (int i = 0; i < ::size; i++)
	{
		i < ::size - 1? cout << " ": cout << "1"; //для отображения пробелов в конце каждой строки и "1" в конце последней строки, т.к. последняя строка имеет номер 10, а мы не можем записать в массив 2 символа.
		for (int j = 0; j < ::size; j++)
			cout <<mass1[i][j];
		cout << endl;
	}
}

//ф-ция для заполнения массива пустыми клетками
void EmptyField(unsigned char mass1[][::size])
{
	for (int i = 0; i < ::size; i++)
	{
		for (int j = 0; j < ::size; j++)
		{
			if (i == 0)
				j == 0 ? mass1[i][j] = ' ' : mass1[i][j] = char(j + 64); //в первой строке выводим буквы от А до J и пробел в 1 ячейке.
			else if (j == 0)
				i < ::size-1 ? mass1[i][j] = i + 48 : mass1[i][j] = '0'; //в 1 столбце выводим цифры от 1 до 9, в конце выводим 0. ф-ция showscreen дополняет последний столбец до числа 10.
			else
				mass1[i][j] = ::Empty; //само заполнение массива
		}
	}
}

//ф-ция описания управления при расстановке кораблей
void Controls()
{
	cout << endl;
	cout << "Press " << char(17) << " for move left" << endl;
	cout << "Press " << char(16) << " for move right" << endl;
	cout << "Press " << char(30) << " for move up" << endl;
	cout << "Press " << char(31) << " for move down" << endl;
	cout << "Press space for place the ship" << endl;
	cout << "Press backspace for delete the current ship" << endl;
	cout << "Press ESC for return to main menu" << endl;
}

//ф-ция описания управления при битве
void ControlsForFight()
{
	if (::PlayerChoise == 0 || ::PlayerChoise == 1) //при игре комп/комп
		cout << endl << "Press space for pause/unpause" << endl << "Press double tap ESC for return to main menu" << endl;
	else //при игре игрока/комп
		cout << endl << "Use arrows " << char(17) << " " << char(16) << " " << char(30) << " " << char(31) << " to move around the field" << endl << "Press space for shoot" << endl << "Press ESC for return to main menu" << endl;
}

//ф-ция показывает результат после выстрела
void ShowAfterShot(unsigned char PlayerMap[][::size], unsigned char VisibleComputerMap[][::size])
{
	system("cls");
	cout << " Player map." << endl;
	ShowScreen(PlayerMap);
	cout << " Computer map." << endl;
	ShowScreen(VisibleComputerMap);
}

//ф-ция автоматической расстановки 4х клеточного корабля
void AutoShip4Parts(unsigned char mass1[][::size])
{
	srand(time(NULL)); //для генератора случайных чисел
	COORD c;
	int count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 }, Ytemp2{ 0 }, Xtemp2{ 0 }, Ytemp3{ 0 }, Xtemp3{ 0 };
	while (count < 4)
	{
		c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };//генерация координат от 2 до 11 по обоим осям
		if (mass1[c.Y - 1][c.X - 1] == ::Empty)
		{
			if (count == 0)
			{
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp1 = c.Y - 1, Xtemp1 = c.X - 1;
				count++;
				continue;
			}
			if (count == 1)
			{
				if ((mass1[c.Y - 1 + 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) ||
					(mass1[c.Y - 1][c.X - 1 + 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 - 1] == ::Ship))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					Ytemp2 = c.Y - 1, Xtemp2 = c.X - 1;
					count++;
					continue;
				}
			}
			if (count == 2)
			{
				if (Xtemp1 == Xtemp2 && ((mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 + 1][c.X - 1] == ::Ship)))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					Ytemp3 = c.Y - 1, Xtemp3 = c.X - 1;
					count++;
					continue;
				}
				if (Ytemp1 == Ytemp2 && ((mass1[c.Y - 1][c.X - 1 - 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 + 1] == ::Ship)))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					Ytemp3 = c.Y - 1, Xtemp3 = c.X - 1;
					count++;
					continue;
				}
			}
			if (count == 3)
			{
				if (Xtemp1 == Xtemp3 && ((mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 + 1][c.X - 1] == ::Ship)))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					count++;
					continue;
				}
				if (Ytemp1 == Ytemp3 && ((mass1[c.Y - 1][c.X - 1 - 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 + 1] == ::Ship)))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					count++;
					continue;
				}
			}
		}
	}
}

//ф-ция автоматической расстановки 3х клеточного корабля
void AutoShip3Parts(unsigned char mass1[][::size])
{
	srand(time(NULL));
	COORD c;
	int countSCCP = 0, count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 }, Ytemp2{ 0 }, Xtemp2{ 0 };
	while (count < 3)
	{
		if (countSCCP>10000)
			//условие создано для того, чтобы проверять возможность установки корабля, 
			//исключить случай когда кораблю не хватает места и цикл станет бесконечным (например 3х палубный корабль можен не поместиться в углу между другим 3х палубным и 4х палубным кораблем)
			//переменная countSCCP возрастает на 1 каждый раз когда программа пытается установить координату для размещения палубы
			//в этом случае, если корабль где-то не помещается, программа обнуляет результат после 10 тысяч попыток, и начинает размещать корабль с первой палубы в другом месте
		{
			mass1[Ytemp1][Xtemp1]= ::Empty;
			mass1[Ytemp2][Xtemp2] = ::Empty;
			countSCCP = 0;
			count = 0;
		}
		if (count == 0)
		{
			c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };
			countSCCP++;
			if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp1 = c.Y - 1;
				Xtemp1 = c.X - 1;
				mass1[Ytemp1][Xtemp1] = 'T';
				count++;
				continue;
			}
		}
		if (count == 1)
		{
			c = { (short)(rand() % 4 + (Xtemp1 - 1)), (short)(rand() % 4 + (Ytemp1 - 1)) };
			countSCCP++;
			if ((mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //если курсор над первой палубой
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp2 = c.Y - 1;
				Xtemp2 = c.X - 1;
				mass1[Ytemp2][Xtemp2] = 'T';
				mass1[Ytemp1][Xtemp1] = 't';
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && 
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) && //если курсор слева от первой палубы
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp2 = c.Y - 1;
				Xtemp2 = c.X - 1;
				mass1[Ytemp2][Xtemp2] = 'T';
				mass1[Ytemp1][Xtemp1] = 't';
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) && //если курсор справа от первой палубы
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp2 = c.Y - 1;
				Xtemp2 = c.X - 1;
				mass1[Ytemp2][Xtemp2] = 'T';
				mass1[Ytemp1][Xtemp1] = 't';
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //если курсор под первой палубой
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp2 = c.Y - 1;
				Xtemp2 = c.X - 1;
				mass1[Ytemp2][Xtemp2] = 'T';
				mass1[Ytemp1][Xtemp1] = 't';
				count++;
				continue;
			}
		}
		if (count == 2)
		{
			c = { (short)(rand() % 5 + (Xtemp1 - 1)), (short)(rand() % 5 + (Ytemp1 - 1)) };
			countSCCP++;
			if (Xtemp1 == Xtemp2) //если корабль располагается вертикально
			{
				if (mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) //если курсор над обоими палубами
				{
					if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
					{
						mass1[Ytemp2][Xtemp2] = ::Ship;
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						count++;
						continue;
					}
				}
				else if (mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) //если курсор под обоими палубами
				{
					if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
					{
						mass1[Ytemp2][Xtemp2] = ::Ship;
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						count++;
						continue;
					}
				}
			}
			if (Ytemp1 == Ytemp2) //если корабль располагается горизонтально
			{
				if (mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) //если курсор справа от обоих палуб
				{
					if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
					{
						mass1[Ytemp2][Xtemp2] = ::Ship;
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						count++;
						continue;
					}
				}
				else if (mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) //если курсор слева от обоих палуб
				{
					if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship)&& (mass1[c.Y - 1][c.X - 1] == ::Empty))
					{
						mass1[Ytemp2][Xtemp2] = ::Ship;
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						count++;
						continue;
					}
				}
			}
		}
	}
}

//ф-ция автоматической расстановки 2х клеточного корабля
void AutoShip2Parts(unsigned char mass1[][::size])
{
	srand(time(NULL));
	COORD c;
	int countSCCP=0,count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 };
	while (count < 2)
	{
		if (countSCCP > 10000)
		{
			mass1[Ytemp1][Xtemp1] = ::Empty;
			countSCCP = 0;
			count = 0;
		}
		if (count == 0)
		{
			c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };
			countSCCP++;
			if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship) && (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				Ytemp1 = c.Y - 1;
				Xtemp1 = c.X - 1;
				mass1[Ytemp1][Xtemp1] = 'T';
				count++;
				continue;
			}
		}
		if (count == 1)
		{
			c = { (short)(rand() % 4 + (Xtemp1 - 1)), (short)(rand() % 4 + (Ytemp1 - 1)) };
			countSCCP++;
			if ((mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба снизу курсора
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship) && (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && 
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба справа курсора
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship) && (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && 
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба слева курсора
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship) && (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				count++;
				continue;
			}
			else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && 
				(mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба сверху курсора
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship) && (mass1[c.Y - 1][c.X - 1] == ::Empty))
			{
				mass1[Ytemp1][Xtemp1] = ::Ship;
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				count++;
				continue;
			}
		}
	}
}

//ф-ция автоматической расстановки 1 клеточного корабля
void AutoShip1Parts(unsigned char mass1[][::size])
{
	srand(time(NULL));
	COORD c;
	int k = 0, count{ 0 };
	while (count < 1)
	{
		c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };
		if (mass1[c.Y - 1][c.X - 1] == ::Empty)
		{
			if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
				(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
			{
				mass1[c.Y - 1][c.X - 1] = ::Ship;
				count++;
				continue;
			}
		}
	}
}

//ф-ция выстрела легкого компьютера по игроку
void EasyComputerShot(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size])
{
	srand(time(NULL));
	COORD c;
	bool status = true, test = false;
	while (status == true)
	{
		c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };
		if (PlayerMap[c.Y - 1][c.X - 1] != ::Missed && PlayerMap[c.Y - 1][c.X - 1] != ::Hit && PlayerMap[c.Y - 1][c.X - 1] != ::Empty && PlayerMap[c.Y - 1][c.X - 1] != ::Ship)
			continue;
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Missed || PlayerMap[c.Y - 1][c.X - 1] == ::Hit)
			continue;
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Empty)
		{
			PlayerMap[c.Y - 1][c.X - 1] = ::Missed;
			status = false;
			Sleep(1000); //задержка 1 сек перед завершением выстрела (удобнее отследить выстрел, ибо компьютер стреляет моментально)
			PlaySound(L"miss", NULL, SND_RESOURCE | SND_ASYNC);
			break;
		}
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Ship)
		{
			PlayerMap[c.Y - 1][c.X - 1] = ::Hit;
			Sleep(1000);
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (i == 0 && j == 0)
						continue;
					if (PlayerMap[c.Y - 1 + i][c.X - 1 + j] == ::Hit || PlayerMap[c.Y - 1 + i][c.X - 1 + j] == ::Ship)
					{
						for (int q = -1; q < 2; q++)
						{
							for (int r = -1; r < 2; r++)
							{
								if (PlayerMap[c.Y - 1 + i+q][c.X - 1 + j+r] == ::Hit || PlayerMap[c.Y - 1 + i+q][c.X - 1 + j+r] == ::Ship)
								{
									for (int y = -1; y < 2; y++)
									{
										for (int u = -1; u < 2; u++)
										{
											if (PlayerMap[c.Y - 1 + i + q+y][c.X - 1 + j + r+u] == ::Ship)
											{
												test = true; //проверка на полностью подбитый корабль
												PlaySound(L"hit", NULL, SND_RESOURCE | SND_ASYNC);
												goto mark1; //для ускоренного выхода из циклов
											}
										}
									}
								}
							}
						}
					}
				}
			}
			mark1:
			if (test == false)
			{
				PlaySound(L"damage", NULL, SND_RESOURCE | SND_ASYNC);
				for (int n = -1; n < 2; n++)
				{
					for (int m = -1; m < 2; m++)
					{
						if (PlayerMap[c.Y - 1 + n][c.X - 1 + m] == ::Empty)
							PlayerMap[c.Y - 1 + n][c.X - 1 + m] = ::Missed; //вокруг 1-палубного корабля ставятся промахи
						if (PlayerMap[c.Y - 1 + n][c.X - 1 + m] == ::Hit)
						{
							for (int z = -1; z < 2; z++)
							{
								for (int x = -1; x < 2; x++)
								{
									if (PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] == ::Empty)
										PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] = ::Missed;//вокруг 2-палубного корабля ставятся промахи
									if (PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] == ::Hit)
									{
										for (int r = -1; r < 2; r++)
										{
											for (int t = -1; t < 2; t++)
											{
												if (PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] == ::Empty)
													PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] = ::Missed;//вокруг 3-палубного корабля ставятся промахи
												if (PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] == ::Hit)
												{
													for (int q = -1; q < 2; q++)
													{
														for (int w = -1; w < 2; w++)
															if (PlayerMap[c.Y - 1 + n + z + r + q][c.X - 1 + m + x + t + w] == ::Empty)
																PlayerMap[c.Y - 1 + n + z + r + q][c.X - 1 + m + x + t + w] = ::Missed;//вокруг 4-палубного корабля ставятся промахи
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			ShowAfterShot(PlayerMap, VisibleComputerMap);
			cout << " computer is strike your ship!\n";
			ControlsForFight();
			test = false;
			EndGame(PlayerMap, ComputerMap, VisibleComputerMap);
			continue;
		}
	}
	ShowAfterShot(PlayerMap, VisibleComputerMap);
	cout << " computer is missing! your turn.\n";
	ControlsForFight();
}

//ф-ция выстрела сложного компьютера по игроку
void HardComputerShot(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size])
{
	srand(time(NULL));
	COORD c;
	bool status = true;
	while (status == true)
	{
		if (::CountFirst1 == 0) //при первом выстреле (до поражения нового корабля (первый ход и каждый последующий ход после полного уничтожения корабля))
		{
			int countpass{ 0 },dots{0};
			//компьютер находит ячейку, вокруг которой минимум подбитых ячеек. (более логичный выбор при игре в морской бой)
			while (countpass<20)
			{
				dots=0; //считаем кол-во уже простерленных мест вокруг предполагаемого выстрела
				c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 2) };
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (i == 0 && j == 0)
							continue;
						if (PlayerMap[c.Y - 1 + i][c.X - 1 + j] == ::Missed) //если вокруг выбранной ячейки есть уже простреленные места
							dots++; //то увеличиваем перенную
					}
				}
				countpass++;// попытка поиска для выстрела увеличилась на 1.(после 20 попытки цикл прекратиться)
				if (dots == 0) //в приориетете выстрелить в ту ячейку, вокруг которой еще ничего неизвестно
				{
					countpass = 0;//обнуление кол-во попыток поиска области для выстрела
					break; //цикл прерываем, т.к. ячейка подходящая
				}
				else if (countpass == 19 && dots == 1) //если на поле отсутсвуют ячейки вокург которых нет неивестных областей, то логичнее стрелять в то место, вокруг которого только одна простреленная область
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 2) //и т.д. по образу предыдущего выстрела
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 3)
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 4)
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 5)
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 6)
				{
					countpass = 0;
					break;
				}
				else if (countpass == 19 && dots == 7)
				{
					countpass = 0;
					break;
				}
				else
					continue;// если найденная координата не подходящая, ищем заного
			}
			::CoordX = c.X; //записываем коодинаты первой подбитой палубы
			::CoordY = c.Y;
		}
		else
		{
			if (::CountFirst1 == 1) //компьютер подбил первую палубу корабля
			{
				//это для того чтобы, компьютер выстрелил по горизонали или вертикали от предыдущего выстрела
				c.Y = (rand() % 3 + (::CoordY - 1)); //координата У в диапазоне 3 от предыдущей (пример: коорд У была 5. сейчас она может быть в диапазоне от 4 до 6.)
				c.X = (rand() % 3 + (::CoordX - 1)); //координата Х в диапазоне 3 от предыдущей
				if (c.Y != ::CoordY) //если коодината У изменилась
					c.X = ::CoordX; //то координата Х не меняется
				if (c.X != ::CoordX)//если коодината Х изменилась
					c.Y = ::CoordY; //то координата У не меняется
				::CoordX2 = c.X;//записываем коодинаты 2 подбитой палубы
				::CoordY2 = c.Y;
			}
			if (::CountFirst1 == 2) //компьютер подбил вторую палубу корабля
			{
				if (::CoordY == ::CoordY2) //если корабль расположен горизонтально
				{
					c.Y = ::CoordY; //выстрел будет слева или справа
					if (::CoordX2 > ::CoordX) //если 2 палуба правее первой
						c.X = (rand() % 4 + (::CoordX2 -2)); //координата Х в диапазоне 4 от правой координаты (пример: коорд Х была 5. сейчас она может быть в диапазоне от 3 до 6.)
					else
						c.X = (rand() % 4 + (::CoordX - 2));//если 1 палуба правее второй то координата Х в диапазоне 4 от правой координаты
				}
				else
				{
					c.X = ::CoordX; //выстрел будет снизу или сверху
					if (::CoordY2 > ::CoordY) //если 2 палуба ниже первой
						c.Y = (rand() % 4 + (::CoordY2 - 2));//координата У в диапазоне 4 от нижней координаты
					else
						c.Y = (rand() % 4 + (::CoordY - 2));//если 1 палуба ниже второй то координата У в диапазоне 4 от нижней координаты
				}
				::CoordX3 = c.X;//записываем коодинаты 3 подбитой палубы
				::CoordY3 = c.Y;
			}
			if (::CountFirst1 == 3)//компьютер подбил 3 палубу корабля
			{
				if (::CoordY == ::CoordY2)//если корабль расположен горизонтально
				{
					c.Y = ::CoordY; //выстрел будет слева или справа
					//если 2 подбитая палуба посередине то координата У в диапазоне 5 от координаты второй подбитой палубы
					if ((::CoordX2 > ::CoordX && ::CoordX3 > ::CoordX2 && ::CoordX3 > ::CoordX) || (::CoordX2 < ::CoordX && ::CoordX3 < ::CoordX2 && ::CoordX3 < ::CoordX))
						c.X = (rand() % 5 + (::CoordX2 - 2));
					//если 1 подбитая палуба посередине то координата У в диапазоне 5 от координаты первой подбитой палубы
					if ((::CoordX2 < ::CoordX && ::CoordX3 > ::CoordX2 && ::CoordX3 > ::CoordX) || (::CoordX2 > ::CoordX && ::CoordX3 < ::CoordX2 && ::CoordX3 < ::CoordX))
						c.X = (rand() % 5 + (::CoordX - 2));
				}
				else
				{
					c.X = ::CoordX; //выстрел будет снизу или сверху
					//если 2 подбитая палуба посередине то координата У в диапазоне 5 от координаты второй подбитой палубы
					if ((::CoordY2 > ::CoordY && ::CoordY3 > ::CoordY2 && ::CoordY3 > ::CoordY) || (::CoordY2 < ::CoordY && ::CoordY3 < ::CoordY2 && ::CoordY3 < ::CoordY))
						c.Y = (rand() % 5 + (::CoordY2 - 2));
					//если 1 подбитая палуба посередине то координата У в диапазоне 5 от координаты первой подбитой палубы
					if ((::CoordY2 < ::CoordY && ::CoordY3 > ::CoordY2 && ::CoordY3 > ::CoordY) || (::CoordY2 > ::CoordY && ::CoordY3 < ::CoordY2 && ::CoordY3 < ::CoordY))
						c.Y = (rand() % 5 + (::CoordY - 2));
				}
			}
		}
		if (PlayerMap[c.Y - 1][c.X - 1] != ::Missed && PlayerMap[c.Y - 1][c.X - 1] != ::Hit && PlayerMap[c.Y - 1][c.X - 1] != ::Empty && PlayerMap[c.Y - 1][c.X - 1] != ::Ship)
			continue;
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Missed || PlayerMap[c.Y - 1][c.X - 1] == ::Hit)
			continue;
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Empty)
		{
			PlayerMap[c.Y - 1][c.X - 1] = ::Missed;
			Sleep(1000);
			PlaySound(L"miss", NULL, SND_RESOURCE | SND_ASYNC);
			status = false;
			break;
		}
		if (PlayerMap[c.Y - 1][c.X - 1] == ::Ship)
		{
			PlayerMap[c.Y - 1][c.X - 1] = ::Hit;
			::CountFirst1++; //счетчик ранений корабля увеличился
			Sleep(1000);
			EndGame(PlayerMap, ComputerMap, VisibleComputerMap);
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (i == 0 && j == 0)
						continue;
					if (PlayerMap[c.Y - 1 + i][c.X - 1 + j] == ::Hit || PlayerMap[c.Y - 1 + i][c.X - 1 + j] == ::Ship)
					{
						for (int q = -1; q < 2; q++)
						{
							for (int r = -1; r < 2; r++)
							{
								if (PlayerMap[c.Y - 1 + i + q][c.X - 1 + j + r] == ::Hit || PlayerMap[c.Y - 1 + i + q][c.X - 1 + j + r] == ::Ship)
								{
									for (int y = -1; y < 2; y++)
									{
										for (int u = -1; u < 2; u++)
										{
											if (PlayerMap[c.Y - 1 + i + q + y][c.X - 1 + j + r + u] == ::Ship)
											{
												::TestDestroy = true;
												PlaySound(L"hit", NULL, SND_RESOURCE | SND_ASYNC);
												goto mark2;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			mark2:
			if (::TestDestroy == false)
			{
				PlaySound(L"damage", NULL, SND_RESOURCE | SND_ASYNC);
				::CountFirst1 = 0; //при полном убийсве корабля счетчик ранений обнуляется
				for (int n = -1; n < 2; n++)
				{
					for (int m = -1; m < 2; m++)
					{
						if (PlayerMap[c.Y - 1 + n][c.X - 1 + m] == ::Empty)
							PlayerMap[c.Y - 1 + n][c.X - 1 + m] = ::Missed;
						if (PlayerMap[c.Y - 1 + n][c.X - 1 + m] == ::Hit)
						{
							for (int z = -1; z < 2; z++)
							{
								for (int x = -1; x < 2; x++)
								{
									if (PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] == ::Empty)
										PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] = ::Missed;
									if (PlayerMap[c.Y - 1 + n + z][c.X - 1 + m + x] == ::Hit)
									{
										for (int r = -1; r < 2; r++)
										{
											for (int t = -1; t < 2; t++)
											{
												if (PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] == ::Empty)
													PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] = ::Missed;
												if (PlayerMap[c.Y - 1 + n + z + r][c.X - 1 + m + x + t] == ::Hit)
												{
													for (int q = -1; q < 2; q++)
													{
														for (int w = -1; w < 2; w++)
														{
															if (PlayerMap[c.Y - 1 + n + z + r + q][c.X - 1 + m + x + t + w] == ::Empty)
																PlayerMap[c.Y - 1 + n + z + r + q][c.X - 1 + m + x + t + w] = ::Missed;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			ShowAfterShot(PlayerMap, VisibleComputerMap);
			cout << " computer is strike your ship!\n";
			ControlsForFight();
			::TestDestroy = false;
		}
	}
	ShowAfterShot(PlayerMap, VisibleComputerMap);
	cout << " computer is missing! your turn.\n";
	ControlsForFight();
}

//ф-ция отображения меню
void Menu(unsigned char PlayerMap[][::size])
{
	system("cls");
	LogoAnimated(); //показывается анимированное название игры
	start1:
	system("cls");
	LogoStatic(); //показывается статичное название игры
	cout << "\n\t\t    " << char(16) << " player vs computer"; //стандартно в меню выбор всегда на первой опции
	cout << "\n\t\t      computer vs computer";
	cout << "\n\t\t      exit";
	cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option"; //помощь по управлению
	COORD c = { 20, 5 };
	int menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0;
	while (menu11 != 32) //стартовый выбор в меню.
	{
		menu11 = _getch();
		switch (menu11)
		{
		case 72:
			PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
			system("cls");
			LogoStatic();
			cout << "\n\t\t    " << char(16) << " player vs computer";
			cout << "\n\t\t      computer vs computer";
			cout << "\n\t\t      exit";
			cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
			if (c.Y ==6) //при переходе с 2 опции на 1.
			{
				c.Y--;
				system("cls");
				LogoStatic();
				cout << "\n\t\t    " << char(16) << " player vs computer";
				cout << "\n\t\t      computer vs computer";
				cout << "\n\t\t      exit";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
				break;
			}
			if (c.Y ==7)//при переходе с 3 опции на 2.
			{
				c.Y--;
				system("cls");
				LogoStatic();
				cout << "\n\t\t      player vs computer";
				cout << "\n\t\t    " << char(16) << " computer vs computer";
				cout << "\n\t\t      exit";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
			}
			break;
		case 80:
			PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
			if (c.Y ==5)//при переходе с 1 опции на 2.
			{
				c.Y++;
				system("cls");
				LogoStatic();
				cout << "\n\t\t      player vs computer";
				cout << "\n\t\t    " << char(16) << " computer vs computer";
				cout << "\n\t\t      exit";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
				break;
			}
			if (c.Y ==6)//при переходе с 2 опции на 3.
			{
				c.Y++;
				system("cls");
				LogoStatic();
				cout << "\n\t\t      player vs computer";
				cout << "\n\t\t      computer vs computer";
				cout << "\n\t\t    " << char(16) << " exit";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
			}
			break;
		case 32:
			PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
			if (c.Y == 5)//2 экран (выбрана игра против комьюетра). выбор сложности
			{
				::PlayerChoise = 2; //игрок играет сам.
				start2:
				system("cls");
				LogoStatic();
				cout << "\n\t\t    " << char(16) << " enemy easy computer (shoots randomly)";
				cout << "\n\t\t      enemy hard computer (shoots wisely)";
				cout << "\n\t\t      back";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
				while (menu12 != 32)
				{
					menu12 = _getch();
					switch (menu12)
					{
					case 72:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y ==6) //при переходе с 2 опции на 1.
						{
							c.Y--;
							system("cls");
							LogoStatic();
							cout << "\n\t\t    " << char(16) << " enemy easy computer (shoots randomly)";
							cout << "\n\t\t      enemy hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
							break;
						}
						if (c.Y ==7) //при переходе с 3 опции на 2.
						{
							c.Y--;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      enemy easy computer (shoots randomly)";
							cout << "\n\t\t    " << char(16) << " enemy hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						}
						break;
					case 80:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y ==5) //при переходе с 1 опции на 2.
						{
							c.Y++;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      enemy easy computer (shoots randomly)";
							cout << "\n\t\t    " << char(16) << " enemy hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
							break;
						}
						if (c.Y ==6) //при переходе с 2 опции на 3.
						{
							c.Y++;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      enemy easy computer (shoots randomly)";
							cout << "\n\t\t      enemy hard computer (shoots wisely)";
							cout << "\n\t\t    " << char(16) << " back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						}
						break;
					case 32:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y == 5) //выбрана игра против лекого компьютера
							::EnemyChoise = 0;
						if (c.Y == 6) //выбрана игра против сложного компьютера
							::EnemyChoise = 1;
						if (c.Y == 7) //возврат на предыдущий экран меню
						{
							menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0; //обнуляем переменные выбора
							c = { 20, 5 };
							goto start1; //переносимся на метку. она в начале стартового выбора
							break;
						}
						system("cls");
						LogoStatic();
						c.Y = 5;
						cout << "\n\t\t    " << char(16) << " manual placement of ships";
						cout << "\n\t\t      automatic placement of ships";
						cout << "\n\t\t      back";
						cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						while (menu13 != 32) //3 экран. выбор расстановки
						{
							menu13 = _getch();
							switch (menu13)
							{
							case 72:
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y ==6) //при переходе с 2 опции на 1.
								{
									c.Y--;
									system("cls");
									LogoStatic();
									cout << "\n\t\t    " << char(16) << " manual placement of ships";
									cout << "\n\t\t      automatic placement of ships";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
									break;
								}
								if (c.Y ==7) //при переходе с 3 опции на 2.
								{
									c.Y--;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      manual placement of ships";
									cout << "\n\t\t    " << char(16) << " automatic placement of ships";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
								}
								break;
							case 80:
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y ==5) //при переходе с 1 опции на 2.
								{
									c.Y++;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      manual placement of ships";
									cout << "\n\t\t    " << char(16) << " automatic placement of ships";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
									break;
								}
								if (c.Y == 6) //при переходе с 2 опции на 3.
								{
									c.Y++;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      manual placement of ships";
									cout << "\n\t\t      automatic placement of ships";
									cout << "\n\t\t    " << char(16) << " back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
								}
								break;
							case 32: 
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y == 5) //выбрана ручная расстановка
								{
									system("cls");
									cout << " Player map." << endl;
									ShowScreen(PlayerMap);
									Controls();
									ManualShip4Parts(PlayerMap);
									ManualShip3Parts(PlayerMap);
									ManualShip3Parts(PlayerMap);
									ManualShip2Parts(PlayerMap);
									ManualShip2Parts(PlayerMap);
									ManualShip2Parts(PlayerMap);
									ManualShip1Parts(PlayerMap);
									ManualShip1Parts(PlayerMap);
									ManualShip1Parts(PlayerMap);
									ManualShip1Parts(PlayerMap);
								}
								if (c.Y == 6) //выбрана авторасстановка
								{
									AutoShip4Parts(PlayerMap);
									AutoShip3Parts(PlayerMap);
									AutoShip3Parts(PlayerMap);
									AutoShip2Parts(PlayerMap);
									AutoShip2Parts(PlayerMap);
									AutoShip2Parts(PlayerMap);
									AutoShip1Parts(PlayerMap);
									AutoShip1Parts(PlayerMap);
									AutoShip1Parts(PlayerMap);
									AutoShip1Parts(PlayerMap);
								}
								if (c.Y == 7) //возврат на предыдущий экран (где был выбор сложности)
								{
									menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0;
									c = { 20, 5 };
									goto start2; //возват к метке
									break;
								}
								break;
							}
						}
						break;
					}
				}
				break;
			}
			if (c.Y == 6)
			{
			start4:
				system("cls");
				LogoStatic();
				c.Y = 5;
				cout << "\n\t\t    " << char(16) << " Computer 1 - easy computer (shoots randomly)";
				cout << "\n\t\t      Computer 1 - hard computer (shoots wisely)";
				cout << "\n\t\t      back";
				cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
				while (menu21 != 32)//2 экран (выбрана игра двух компьютеров)
				{
					menu21 = _getch();
					switch (menu21)
					{
					case 72:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y ==6) //при переходе с 2 опции на 1.
						{
							c.Y--;
							system("cls");
							LogoStatic();
							cout << "\n\t\t    " << char(16) << " Computer 1 - easy computer (shoots randomly)";
							cout << "\n\t\t      Computer 1 - hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
							break;
						}
						if (c.Y ==7)//при переходе с 3 опции на 2.
						{
							c.Y--;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      Computer 1 - easy computer (shoots randomly)";
							cout << "\n\t\t    " << char(16) << " Computer 1 - hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						}
						break;
					case 80:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y ==5) //при переходе с 1 опции на 2.
						{
							c.Y++;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      Computer 1 - easy computer (shoots randomly)";
							cout << "\n\t\t    " << char(16) << " Computer 1 - hard computer (shoots wisely)";
							cout << "\n\t\t      back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
							break;
						}
						if (c.Y ==6) //при переходе с 2 опции на 3.
						{
							c.Y++;
							system("cls");
							LogoStatic();
							cout << "\n\t\t      Computer 1 - easy computer (shoots randomly)";
							cout << "\n\t\t      Computer 1 - hard computer (shoots wisely)";
							cout << "\n\t\t    " << char(16) << " back";
							cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						}
						break;
					case 32:
						PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
						if (c.Y == 5)
							::PlayerChoise = 0; //выбран легкий компьютер на место игрока
						if (c.Y == 6)
							::PlayerChoise = 1; //выбран сложный компьютер на место игрока
						if (c.Y == 7) //возврат в предыдущее меню
						{
							menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0;
							c = { 20, 5 };
							goto start1;
							break;
						}
					start3:
						system("cls");
						LogoStatic();
						c.Y = 5;
						cout << "\n\t\t    " << char(16) << " enemy easy computer (shoots randomly)";
						cout << "\n\t\t      enemy hard computer (shoots wisely)";
						cout << "\n\t\t      back";
						cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
						while (menu22 != 32) //3 экран выбор сложности врага
						{
							menu22 = _getch();
							switch (menu22)
							{
							case 72:
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y ==6) //при переходе с 2 опции на 1.
								{
									c.Y--;
									system("cls");
									LogoStatic();
									cout << "\n\t\t    " << char(16) << " enemy easy computer (shoots randomly)";
									cout << "\n\t\t      enemy hard computer (shoots wisely)";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
									break;
								}
								if (c.Y ==7) //при переходе с 3 опции на 2.
								{
									c.Y--;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      enemy easy computer (shoots randomly)";
									cout << "\n\t\t    " << char(16) << " enemy hard computer (shoots wisely)";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
								}
								break;
							case 80:
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y ==5) //при переходе с 1 опции на 2.
								{
									c.Y++;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      enemy easy computer (shoots randomly)";
									cout << "\n\t\t    " << char(16) << " enemy hard computer (shoots wisely)";
									cout << "\n\t\t      back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
									break;
								}
								if (c.Y ==6) //при переходе с 2 опции на 3.
								{
									c.Y++;
									system("cls");
									LogoStatic();
									cout << "\n\t\t      enemy easy computer (shoots randomly)";
									cout << "\n\t\t      enemy hard computer (shoots wisely)";
									cout << "\n\t\t    " << char(16) << " back";
									cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
								}
								break;
							case 32:
								PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
								if (c.Y == 5) //выбрана игра против легкого компьютера
									::EnemyChoise = 0;
								if (c.Y == 6) //выбрана игра против сложного компьютера
									::EnemyChoise = 1;
								if (c.Y == 7) //возврат в предыдущее меню
								{
									menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0;
									c = { 20, 5 };
									goto start4;
									break;
								}
								system("cls");
								LogoStatic();
								c.Y = 5;
								cout << "\n\t\t    " << char(16) << " manual placement of ships";
								cout << "\n\t\t      automatic placement of ships";
								cout << "\n\t\t      back";
								cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
								while (menu23 != 32) //4 экран выбор расстановки
								{
									menu23 = _getch();
									switch (menu23)
									{
									case 72:
										PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
										if (c.Y ==6) //при переходе с 2 опции на 1.
										{
											c.Y--;
											system("cls");
											LogoStatic();
											cout << "\n\t\t    " << char(16) << " manual placement of ships";
											cout << "\n\t\t      automatic placement of ships";
											cout << "\n\t\t      back";
											cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
											break;
										}
										if (c.Y ==7) //при переходе с 3 опции на 2.
										{
											c.Y--;
											system("cls");
											LogoStatic();
											cout << "\n\t\t      manual placement of ships";
											cout << "\n\t\t    " << char(16) << " automatic placement of ships";
											cout << "\n\t\t      back";
											cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
										}
										break;
									case 80:
										PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
										if (c.Y ==5) //при переходе с 1 опции на 2.
										{
											c.Y++;
											system("cls");
											LogoStatic();
											cout << "\n\t\t      manual placement of ships";
											cout << "\n\t\t    " << char(16) << " automatic placement of ships";
											cout << "\n\t\t      back";
											cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
											break;
										}
										if (c.Y ==6) //при переходе с 2 опции на 3.
										{
											c.Y++;
											system("cls");
											LogoStatic();
											cout << "\n\t\t      manual placement of ships";
											cout << "\n\t\t      automatic placement of ships";
											cout << "\n\t\t    " << char(16) << " back";
											cout << "\n\n\n\n\tPress " << char(30) << " for move up / Press " << char(31) << " for move down / Press space to select an option";
										}
										break;
									case 32:
										PlaySound(L"menu", NULL, SND_RESOURCE | SND_ASYNC);
										if (c.Y == 5) //выбрана ручная расстановка
										{
											system("cls");
											cout << " Player map." << endl;
											ShowScreen(PlayerMap);
											Controls();
											ManualShip4Parts(PlayerMap);
											ManualShip3Parts(PlayerMap);
											ManualShip3Parts(PlayerMap);
											ManualShip2Parts(PlayerMap);
											ManualShip2Parts(PlayerMap);
											ManualShip2Parts(PlayerMap);
											ManualShip1Parts(PlayerMap);
											ManualShip1Parts(PlayerMap);
											ManualShip1Parts(PlayerMap);
											ManualShip1Parts(PlayerMap);
										}
										if (c.Y == 6) //выбрана авторасстановка
										{
											AutoShip4Parts(PlayerMap);
											AutoShip3Parts(PlayerMap);
											AutoShip3Parts(PlayerMap);
											AutoShip2Parts(PlayerMap);
											AutoShip2Parts(PlayerMap);
											AutoShip2Parts(PlayerMap);
											AutoShip1Parts(PlayerMap);
											AutoShip1Parts(PlayerMap);
											AutoShip1Parts(PlayerMap);
											AutoShip1Parts(PlayerMap);
										}
										if (c.Y == 7)
										{
											menu11 = 0, menu12 = 0, menu13 = 0, menu21 = 0, menu22 = 0, menu23 = 0;
											c = { 20, 5 };
											goto start3;
											break;
										}
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
			}
			if (c.Y == 7) //выбрана 3 опция на начальном экране выбора. выход из игры
				exit(0);
			break;
		}
	}
}

int main()
{
	EmptyField(VisibleComputerMap);//заполняем поля пустыми ячейками
	EmptyField(PlayerMap);
	EmptyField(ComputerMap);
	AutoShip4Parts(ComputerMap); //заполняем поле комьюьера
	AutoShip3Parts(ComputerMap);
	AutoShip3Parts(ComputerMap);
	AutoShip2Parts(ComputerMap);
	AutoShip2Parts(ComputerMap);
	AutoShip2Parts(ComputerMap);
	AutoShip1Parts(ComputerMap);
	AutoShip1Parts(ComputerMap);
	AutoShip1Parts(ComputerMap);
	AutoShip1Parts(ComputerMap);
	Menu(PlayerMap); //попадем в меню
	system("cls");
	cout << " Player map." << endl;
	ShowScreen(PlayerMap);
	cout << " Computer map." << endl;
	ShowScreen(VisibleComputerMap);
	ControlsForFight();
	while(1)
	{
		if (::PlayerChoise == 0) //вместо игрока играет слабый компьютер
			ShotCompEasyAsPlayer(ComputerMap, VisibleComputerMap, PlayerMap);
		else if (::PlayerChoise == 1)//вместо игрока играет сильный компьютер
			ShotCompHardAsPlayer(ComputerMap, VisibleComputerMap, PlayerMap);
		else//играет игрок
			PlayerShot(ComputerMap, VisibleComputerMap, PlayerMap);
		::EnemyChoise==0? EasyComputerShot(ComputerMap, VisibleComputerMap, PlayerMap): HardComputerShot(ComputerMap, VisibleComputerMap, PlayerMap); //выбор противника. слабый или сильный компьютер
	}
}

//ф-ция проверяет окончена ли игра после очередного выстрела
void EndGame(unsigned char PlayerMap[][::size], unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size])
{
	int countlose{ 0 }, countvic{ 0 };
	for (size_t i = 0; i < ::size; i++)
	{
		for (size_t j = 0; j < ::size; j++)
		{
			if (PlayerMap[i][j] == ::Hit) //подсчет подбитых палуб у кораблей игрока/компьюера1
				countlose++;
			if (VisibleComputerMap[i][j] == ::Hit) //подсчет подбитых палуб у кораблей комьютера
				countvic++;
		}
	}
	if (countlose == 20) //при 20 подбитых палуб у игрока, игра заканчивается
	{
		PlaySound(L"lose", NULL, SND_RESOURCE | SND_ASYNC);
		system("cls");
		cout << "you lose!\n";
		system("pause");
		for (size_t i = 0; i < ::size; i++)
		{
			for (size_t j = 0; j < ::size; j++)
			{
				if (VisibleComputerMap[i][j] == ::Empty)
					VisibleComputerMap[i][j] = ComputerMap[i][j]; //замена скрытых ячеек компьюетра на открытые игроку
			}
		}
		ShowAfterShot(PlayerMap, VisibleComputerMap); //показ полностью открытых полей после окончания игры
		system("pause");
		::CountFirst1 = 0, ::CountFirst2 = 0; //обнуление глобальных переменных
		::TestDestroy = false, ::TestDestroy2 = false;
		main(); //возврат в меню. обнуление полей
	}
	if (countvic == 20) //при 20 подбитых палуб у компьюьера, игра заканчивается
	{
		PlaySound(L"victory", NULL, SND_RESOURCE | SND_ASYNC);
		system("cls");
		cout << "you win!\n";
		system("pause");
		for (size_t i = 0; i < ::size; i++)
		{
			for (size_t j = 0; j < ::size; j++)
			{
				if (VisibleComputerMap[i][j] == ::Empty)
					VisibleComputerMap[i][j] = ComputerMap[i][j];//замена скрытых ячеек компьюетра на открытые игроку
			}
		}
		ShowAfterShot(PlayerMap, VisibleComputerMap);//показ полностью открытых полей после окончания игры
		system("pause");
		::CountFirst1 = 0, ::CountFirst2 = 0;
		::TestDestroy = false, ::TestDestroy2 = false;
		main(); //возврат в меню. обнуление полей
	}
}

//ф-ция ручной расстановки 4х клеточного корабля
void ManualShip4Parts(unsigned char mass1[][::size])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //захват курсора в консоли
	COORD c = { 2, 2 }; //изначальное положение курсора (2,2 т.к. это начальная точка игорового поля игрока. т.е. ячейка массива [1][1])
	SetConsoleCursorPosition(h, c); //установка позиции курсора. в данном случае на (2,2)
	int k = 0, count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 }, Ytemp2{ 0 }, Xtemp2{ 0 }, Ytemp3{ 0 }, Xtemp3{ 0 };
	while (count < 4)
	{
		k = _getch(); //переменная примает значение при нажатии заданных клавиш.
		switch (k)
		{
		case 27:
			main();
		case 72: //при нажатии стрелки вверх координата по оси Y уменьшается.
			if (c.Y > 2)//условие которое не дает курсору выйти за пределы массива т.е. поля игрока
				c.Y--;
			break;
		case 75://при нажатии стрелки влево координата по оси Х уменьшается.
			if (c.X > 2)
				c.X--;
			break;
		case 77://при нажатии стрелки вправо координата по оси Х увеличивается.
			if (c.X < 11)
				c.X++;
			break;
		case 80://при нажатии стрелки вниз координата по оси У увеличивается.
			if (c.Y < 11)
				c.Y++;
			break;
		case 32://при нажатии пробела
			if (mass1[c.Y - 1][c.X - 1] == ::Empty) //действие выполнится только при условии что клеточка пустая
			{
				if (count == 0)//первая палуба
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;//заменям данные в массиве на символ - заполненный квадратик
					PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
					Ytemp1 = c.Y - 1, Xtemp1 = c.X - 1; //записываем координаты первой палубы во временные переменные
					system("cls");//очищаем консоль
					cout << " Player map." << endl;
					ShowScreen(mass1);//и выводим поле игрока с внесенными изменениями
					Controls();
					count++;//увеличиваем счетчик выставленных палуб на 1
					break;
				}
				if (count == 1)//вторая палуба
				{
					if ((mass1[c.Y - 1 + 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) || //действие выполнится только при условии что мы ставим палубу по горизонтали или вертикали от 1 палубы.
						(mass1[c.Y - 1][c.X - 1 + 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 - 1] == ::Ship))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp2 = c.Y - 1, Xtemp2 = c.X - 1; //записываем координаты 2 палубы во временные переменные
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
				}
				if (count == 2) //3 палуба
				{
					//1 условие - если мы ставим корабль вертикально. 2 условие - возможность поставить палубу только если корабль НАД или ПОД курсором
					if (Xtemp1 == Xtemp2 && ((mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 + 1][c.X - 1] == ::Ship)))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp3 = c.Y - 1, Xtemp3 = c.X - 1; //записываем координаты 3 палубы во временные переменные
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
					//1 условие - если мы ставим корабль горизонтально. 2 условие - возможность поставить палубу только если корабль СЛЕВА или СПРАВА от курсора
					if (Ytemp1 == Ytemp2 && ((mass1[c.Y - 1][c.X - 1 - 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 + 1] == ::Ship)))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp3 = c.Y - 1, Xtemp3 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
				}
				if (count == 3) //4 палуба
				{
					if (Xtemp1 == Xtemp3 && ((mass1[c.Y - 1 - 1][c.X - 1] == ::Ship) || (mass1[c.Y - 1 + 1][c.X - 1] == ::Ship)))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
					if (Ytemp1 == Ytemp3 && ((mass1[c.Y - 1][c.X - 1 - 1] == ::Ship) || (mass1[c.Y - 1][c.X - 1 + 1] == ::Ship)))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
				}
			}
			break;
		case 8://при нажатии backspace
			if (count >= 1)
				mass1[Ytemp1][Xtemp1] = ::Empty;//удаляет первую палубу корабля
			if (count >= 2)
				mass1[Ytemp2][Xtemp2] = ::Empty;//удаляет первую и вторую палубу корабля
			if (count == 3)
				mass1[Ytemp3][Xtemp3] = ::Empty;//удаляет первую, вторую и третью палубу корабля
			count = 0; //счетчик поставленных палую обнуляет
			system("cls");
			cout << " Player map." << endl;
			ShowScreen(mass1);
			Controls();
			break;
		}
		SetConsoleCursorPosition(h, c);//установка позиции курсора после установки каждой палубы сохраняется
	}
}

//ф-ция ручной расстановки 3х клеточного корабля
void ManualShip3Parts(unsigned char mass1[][::size])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 2, 2 };
	SetConsoleCursorPosition(h, c);
	int k = 0, count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 }, Ytemp2{ 0 }, Xtemp2{ 0 };
	while (count < 3)
	{
		k = _getch();
		switch (k)
		{
		case 27:
			main();
		case 72:
			if (c.Y > 2)
				c.Y--;
			break;
		case 75:
			if (c.X > 2)
				c.X--;
			break;
		case 77:
			if (c.X < 11)
				c.X++;
			break;
		case 80:
			if (c.Y < 11)
				c.Y++;
			break;
		case 32:
			if (mass1[c.Y - 1][c.X - 1] == ::Empty)
			{
				if (count == 0)
				{
					//условие которое позволяет ставить корабль, только если вокруг уже нет кораблей.
					if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp1 = c.Y - 1;
						Xtemp1 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp1][Xtemp1] = 'T'; //временно переназначаем уже поставленную первую палубу корабля на символ, отличающийся от ::ship
						//если не делать эту операцию, то в итоге при установке второго корабля, мы сможем присоединить вторую палубу к первой палубе первого корабля, что неверно.
						count++;
						break;
					}
				}

				if (count == 1)
				{
					if ((mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //при условии что первая палуба стоит снизу от курсора
						//если остальные ячейки вокруг не являются кораблем
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship; //возвращем первой палубе значение ::ship
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp2 = c.Y - 1;
						Xtemp2 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp2][Xtemp2] = 'T';//временно переназначаем уже поставленную первую палубу корабля на символ, отличающийся от ::ship
						mass1[Ytemp1][Xtemp1] = 't';//временно переназначаем уже поставленную вторую палубу корабля на символ, отличающийся от ::ship
						//если не делать эту операцию, то в итоге при установке второго корабля, мы сможем присоединить третью палубу ко второй палубе первого корабля, что неверно.
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) && //при условии что первая палуба стоит справа от курсора. остальные вокруг ячейки не корабли
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем первой палубе значение ::ship
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp2 = c.Y - 1;
						Xtemp2 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp2][Xtemp2] = 'T';//временно переназначаем уже поставленную первую палубу корабля на символ, отличающийся от ::ship
						mass1[Ytemp1][Xtemp1] = 't';//временно переназначаем уже поставленную вторую палубу корабля на символ, отличающийся от ::ship
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) && //при условии что первая палуба стоит слева от курсора. остальные вокруг ячейки не корабли
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем первой палубе значение ::ship
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp2 = c.Y - 1;
						Xtemp2 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp2][Xtemp2] = 'T';//временно переназначаем уже поставленную первую палубу корабля на символ, отличающийся от ::ship
						mass1[Ytemp1][Xtemp1] = 't';//временно переназначаем уже поставленную вторую палубу корабля на символ, отличающийся от ::ship
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //при условии что первая палуба стоит сверху от курсора. остальные вокруг ячейки не корабли
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем первой палубе значение ::ship
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp2 = c.Y - 1;
						Xtemp2 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp2][Xtemp2] = 'T';//временно переназначаем уже поставленную первую палубу корабля на символ, отличающийся от ::ship
						mass1[Ytemp1][Xtemp1] = 't';//временно переназначаем уже поставленную вторую палубу корабля на символ, отличающийся от ::ship
						count++;
						break;
					}
				}
				if (count == 2)
				{
					if (Xtemp1 == Xtemp2) //если корабль расставляется вертикально
					{
						if (mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) //при условии что курсор стоит над двумя расставлеными палубами
						{
							if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) && //при условии что вокург курсора нет кораблей, кроме нижней ячейки.
								(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
							{
								mass1[Ytemp2][Xtemp2] = ::Ship;//возвращем первой палубе значение ::ship
								mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем второй палубе значение ::ship
								mass1[c.Y - 1][c.X - 1] = ::Ship;
								PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
								system("cls");
								cout << " Player map." << endl;
								ShowScreen(mass1);
								Controls();
								count++;
								break;
							}
						}
						else if (mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) //при условии что курсор стоит под двумя расставлеными палубами
						{
							if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && //при условии что вокург курсора нет кораблей, кроме верхней ячейки.
								(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
							{
								mass1[Ytemp2][Xtemp2] = ::Ship;//возвращем первой палубе значение ::ship
								mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем второй палубе значение ::ship
								mass1[c.Y - 1][c.X - 1] = ::Ship;
								PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
								system("cls");
								cout << " Player map." << endl;
								ShowScreen(mass1);
								Controls();
								count++;
								break;
							}
						}
					}
					if (Ytemp1 == Ytemp2) //если корабль расставляется горизонтально
					{
						if (mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) //при условии что курсор стоит справа от двух расставленных палуб
						{
							if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) && //при условии что вокург курсора нет кораблей, кроме левой ячейки.
								(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
							{
								mass1[Ytemp2][Xtemp2] = ::Ship;//возвращем первой палубе значение ::ship
								mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем второй палубе значение ::ship
								mass1[c.Y - 1][c.X - 1] = ::Ship;
								PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
								system("cls");
								cout << " Player map." << endl;
								ShowScreen(mass1);
								Controls();
								count++;
								break;
							}
						}
						else if (mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp2][Xtemp2] || mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) //при условии что курсор стоит слева от двух расставленных палуб
						{
							if ((mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) && //при условии что вокург курсора нет кораблей, кроме правой ячейки.
								(mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
								(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
								(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
							{
								mass1[Ytemp2][Xtemp2] = ::Ship;//возвращем первой палубе значение ::ship
								mass1[Ytemp1][Xtemp1] = ::Ship;//возвращем второй палубе значение ::ship
								mass1[c.Y - 1][c.X - 1] = ::Ship;
								PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
								system("cls");
								cout << " Player map." << endl;
								ShowScreen(mass1);
								Controls();
								count++;
								break;
							}
						}
					}
				}
			}
			break;
		case 8:
			if (count >= 1)
				mass1[Ytemp1][Xtemp1] = ::Empty;
			if (count == 2)
				mass1[Ytemp2][Xtemp2] = ::Empty;
			count = 0;
			system("cls");
			cout << " Player map." << endl;
			ShowScreen(mass1);
			Controls();
			break;
		}
		SetConsoleCursorPosition(h, c);
	}
}

//ф-ция ручной расстановки 2х клеточного корабля
void ManualShip2Parts(unsigned char mass1[][::size])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 2, 2 };
	SetConsoleCursorPosition(h, c);
	int k = 0, count{ 0 }, Ytemp1{ 0 }, Xtemp1{ 0 };
	while (count < 2)
	{
		k = _getch();
		switch (k)
		{
		case 27:
			main();
		case 72:
			if (c.Y > 2)
				c.Y--;
			break;
		case 75:
			if (c.X > 2)
				c.X--;
			break;
		case 77:
			if (c.X < 11)
				c.X++;
			break;
		case 80:
			if (c.Y < 11)
				c.Y++;
			break;
		case 32:
			if (mass1[c.Y - 1][c.X - 1] == ::Empty)
			{
				if (count == 0)
				{
					if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && //если вокруг нет кораблей
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						Ytemp1 = c.Y - 1;
						Xtemp1 = c.X - 1;
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						mass1[Ytemp1][Xtemp1] = 'T';
						count++;
						break;
					}
				}
				if (count == 1)
				{
					if ((mass1[c.Y - 1 + 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба выше курсора
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба справа от курсора
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] == mass1[Ytemp1][Xtemp1]) && //если первая палуба слева от курсора
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
					else if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && //если первая палуба ниже курсора
						(mass1[c.Y - 1 - 1][c.X - 1] == mass1[Ytemp1][Xtemp1]) &&
						(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
						(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
					{
						mass1[Ytemp1][Xtemp1] = ::Ship;
						mass1[c.Y - 1][c.X - 1] = ::Ship;
						PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
						system("cls");
						cout << " Player map." << endl;
						ShowScreen(mass1);
						Controls();
						count++;
						break;
					}
				}
			}
			break;
		case 8:
			if (count == 1)
				mass1[Ytemp1][Xtemp1] = ::Empty;
			count = 0;
			system("cls");
			cout << " Player map." << endl;
			ShowScreen(mass1);
			Controls();
			break;
		}
		SetConsoleCursorPosition(h, c);
	}
}

//ф-ция ручной расстановки 1 клеточного корабля
void ManualShip1Parts(unsigned char mass1[][::size])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 2, 2 };
	SetConsoleCursorPosition(h, c);
	int k = 0, count{ 0 };
	while (count < 1)
	{
		k = _getch();
		switch (k)
		{
		case 27:
			main();
		case 72:
			if (c.Y > 2)
				c.Y--;
			break;
		case 75:
			if (c.X > 2)
				c.X--;
			break;
		case 77:
			if (c.X < 11)
				c.X++;
			break;
		case 80:
			if (c.Y < 11)
				c.Y++;
			break;
		case 32:
			if (mass1[c.Y - 1][c.X - 1] == ::Empty)
			{
				if ((mass1[c.Y - 1 + 1][c.X - 1] != ::Ship) && //если вокруг нет кораблей
					(mass1[c.Y - 1 - 1][c.X - 1] != ::Ship) &&
					(mass1[c.Y - 1][c.X - 1 + 1] != ::Ship) &&
					(mass1[c.Y - 1][c.X - 1 - 1] != ::Ship) &&
					(mass1[c.Y - 1 + 1][c.X - 1 + 1] != ::Ship) &&
					(mass1[c.Y - 1 + 1][c.X - 1 - 1] != ::Ship) &&
					(mass1[c.Y - 1 - 1][c.X - 1 + 1] != ::Ship) &&
					(mass1[c.Y - 1 - 1][c.X - 1 - 1] != ::Ship))
				{
					mass1[c.Y - 1][c.X - 1] = ::Ship;
					PlaySound(L"place", NULL, SND_RESOURCE | SND_ASYNC);
					system("cls");
					cout << " Player map." << endl;
					ShowScreen(mass1);
					Controls();
					count++;
					break;
				}
			}
		}
		SetConsoleCursorPosition(h, c);
	}
}

//ф-ция выстрела игрока
void PlayerShot(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	static COORD c = { 2, 14 }; //изначальное положение 2,14 т.к. это первоначальная точка на поле компьютера и в массиве [1][1]. тип данных static для сохранения координат после выхода и повторном входе в ф-цию.
	SetConsoleCursorPosition(h, c);
	int k = 0;
	bool status = true, test = false;
	while (status == true) //цикл продолжается пока переменная status не поменяюся на 0, при промахе игрока
	{
		k = _getch();
		switch (k)
		{
		case 27:
			::CountFirst1 = 0, ::CountFirst2 = 0;
			test = false, ::TestDestroy2 = false;
			main();
		case 72:
			if (c.Y > 14)
				c.Y--;
			break;
		case 75:
			if (c.X > 2)
				c.X--;
			break;
		case 77:
			if (c.X < 11)
				c.X++;
			break;
		case 80:
			if (c.Y < 23)
				c.Y++;
			break;
		case 32:
			//далее везде будут проверяться одновременно два массива. один (mapC1) невидимый игроку с заполненными ячейками кораблями - массив компьютера
			//второй (mapTemp) - видимый игроку заполненный ячейками ::empty (нераскрытые).
			//он будет копировать данные из массива компьютера при выстрелах и убийстве кораблей. и меняться в процессе игры, создавая видимость "раскрытия карты".
			if (VisibleComputerMap[c.Y - 13][c.X - 1] == ::Missed || VisibleComputerMap[c.Y - 13][c.X - 1] == ::Hit) //при выстреле в уже раскрытыю ячейку результата не будет
				continue;
			if (ComputerMap[c.Y - 13][c.X - 1] == ::Empty) //при выстреле в ячейку без корабля
			{
				VisibleComputerMap[c.Y - 13][c.X - 1] = ::Missed; //скрытая ячейка санет ячейкой с символом "промах" (::dot)
				PlaySound(L"miss", NULL, SND_RESOURCE | SND_ASYNC);
				status = false; //цикл прекратиться, ход перейдет к компьютеру
				break;
			}
			if (ComputerMap[c.Y - 13][c.X - 1] == ::Ship) //при выстреле в ячейку с кораблем
			{
				VisibleComputerMap[c.Y - 13][c.X - 1] = ::Hit; //скрытая ячейка санет ячейкой с символом "попал" (Х)
				EndGame(PlayerMap, ComputerMap, VisibleComputerMap); //проверка на конец игры
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (i == 0 && j == 0)
							continue;
						if (ComputerMap[c.Y - 13 + i][c.X - 1 + j] == ::Ship) //проверка ячеек вокруг подбитой на наличие кораблей
						{
							for (int h = -1; h < 2; h++)
							{
								for (int b = -1; b < 2; b++)
								{
									if (ComputerMap[c.Y - 13 + i + h][c.X - 1 + j + b] == ::Ship) //проверка ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей
									{
										for (int n = -1; n < 2; n++)
										{
											for (int m = -1; m < 2; m++)
											{
												//проверка ячеек вокруг проверяемых ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей при этом не являющихся видимыми подбитыми
												if (ComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] == ::Ship && VisibleComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] != ::Hit)
												{
													PlaySound(L"hit", NULL, SND_RESOURCE | SND_ASYNC);
													test = true; //если проверка пройдена хотябы раз, то переменная test поменяет значение на 1.
													break;
													//вся эта проверка проверяет подбит ли полностью корабль. при test=true корабль не убить полностью.
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if (test==false)
					PlaySound(L"damage", NULL, SND_RESOURCE | SND_ASYNC);
				for (int n = -1; n < 2; n++)
				{
					for (int m = -1; m < 2; m++)
					{
						//проще говоря - вокруг подбитого 1-о палубного корабля ставятся промахи.
						if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Empty && test == false) //если корабль полностью подбит(test=false) и вокруг выбранной пустые ячейки
							VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] = ::Missed; //то все ячейки заполненные "пусто" вокруг становится видимыми и заполняюся "промахом" по правилам игры.
						if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Ship && VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Hit && test == false)
							//проверка ячеек вокруг на наличие подбитых палуб корабля при условии что корабль полностью подбит
						{
							for (int z = -1; z < 2; z++)
							{
								for (int x = -1; x < 2; x++)
								{
									//проще говоря - вокруг подбитого 2-х палубного корабля ставятся промахи.
									if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
										VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
									if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Hit)
										//проверка ячеек вокруг проверяемых на наличие подбитых палуб корабля
									{
										for (int r = -1; r < 2; r++)
										{
											for (int t = -1; t < 2; t++)
											{
												//проще говоря - вокруг подбитого 3-х палубного корабля ставятся промахи.
												if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
													VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] = ::Missed;//и последующее их раскрытие игроку с заменой на "промах"
												if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Hit)
													//проверка ячеек вокруг проверяемых вокруг проверяемых на наличие подбитых палуб корабля
												{
													for (int q = -1; q < 2; q++)
													{
														for (int w = -1; w < 2; w++)
														{
															//проще говоря - вокруг подбитого 4-х палубного корабля ставятся промахи.
															if (ComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
																VisibleComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				ShowAfterShot(PlayerMap, VisibleComputerMap); //вывод в консоль изменений в массивах после выстрела в корабль
				cout << " strike! shoot again.\n";
				ControlsForFight();
				test = false; //переменная проверки на полностью подбитый корабль присваивает первоначальное значение для последующей повторной проверки
				SetConsoleCursorPosition(h, c); //сохранение позиции курсора после выстрела в корабль
				continue;
			}
		}
		SetConsoleCursorPosition(h, c);//сохранение позиции курсора после выстрела
	}
	ShowAfterShot(PlayerMap, VisibleComputerMap);//вывод в консоль изменений в массивах после выстрела
	cout << " you miss!\n";
	ControlsForFight();
}

//ф-ция выстрела легкого компьютера против компьютера
void ShotCompEasyAsPlayer(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size])
{
	srand(time(NULL));
	COORD c;
	bool status = true, test = false;
	while (status == true)
	{
		if (_kbhit() && (_getch() == 32))
			system("pause");
		else if (_kbhit() && (_getch() == 27))
		{
			::CountFirst1 = 0, ::CountFirst2 = 0;
			test = false, ::TestDestroy2 = false;
			main();
		}
		c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 14) };
		if (VisibleComputerMap[c.Y - 13][c.X - 1] == ::Missed || VisibleComputerMap[c.Y - 13][c.X - 1] == ::Hit)
			continue;
		if (ComputerMap[c.Y - 13][c.X - 1] == ::Empty) //при выстреле в ячейку без корабля
		{
			VisibleComputerMap[c.Y - 13][c.X - 1] = ::Missed; //скрытая ячейка санет ячейкой с символом "промах" (::dot)
			Sleep(1000);
			PlaySound(L"miss", NULL, SND_RESOURCE | SND_ASYNC);
			status = false; //цикл прекратиться, ход перейдет к компьютеру
			break;
		}
		if (ComputerMap[c.Y - 13][c.X - 1] == ::Ship) //при выстреле в ячейку с кораблем
		{
			VisibleComputerMap[c.Y - 13][c.X - 1] = ::Hit; //скрытая ячейка санет ячейкой с символом "попал" (Х)
			Sleep(1000);
			EndGame(PlayerMap, ComputerMap, VisibleComputerMap); //проверка на конец игры
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (i == 0 && j == 0)
						continue;
					if (ComputerMap[c.Y - 13 + i][c.X - 1 + j] == ::Ship) //проверка ячеек вокруг подбитой на наличие кораблей
					{
						for (int h = -1; h < 2; h++)
						{
							for (int b = -1; b < 2; b++)
							{
								if (ComputerMap[c.Y - 13 + i + h][c.X - 1 + j + b] == ::Ship) //проверка ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей
								{
									for (int n = -1; n < 2; n++)
									{
										for (int m = -1; m < 2; m++)
										{
											//проверка ячеек вокруг проверяемых ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей при этом не являющихся видимыми подбитыми
											if (ComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] == ::Ship && VisibleComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] != ::Hit)
											{
												PlaySound(L"hit", NULL, SND_RESOURCE | SND_ASYNC);
												test = true; //если проверка пройдена хотябы раз, то переменная test поменяет значение на 1.
												break;
												//вся эта проверка проверяет подбит ли полностью корабль. при test=true корабль не убить полностью.
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if (test==false)
				PlaySound(L"damage", NULL, SND_RESOURCE | SND_ASYNC);
			for (int n = -1; n < 2; n++)
			{
				for (int m = -1; m < 2; m++)
				{
					//проще говоря - вокруг подбитого 1-о палубного корабля ставятся промахи.
					if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Empty && test == false) //если корабль полностью подбит(test=false) и вокруг выбранной пустые ячейки
						VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] = ::Missed; //то все ячейки заполненные "пусто" вокруг становится видимыми и заполняюся "промахом" по правилам игры.
					if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Ship && VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Hit && test == false)
						//проверка ячеек вокруг на наличие подбитых палуб корабля при условии что корабль полностью подбит
					{
						for (int z = -1; z < 2; z++)
						{
							for (int x = -1; x < 2; x++)
							{
								//проще говоря - вокруг подбитого 2-х палубного корабля ставятся промахи.
								if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
									VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
								if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Hit)
									//проверка ячеек вокруг проверяемых на наличие подбитых палуб корабля
								{
									for (int r = -1; r < 2; r++)
									{
										for (int t = -1; t < 2; t++)
										{
											//проще говоря - вокруг подбитого 3-х палубного корабля ставятся промахи.
											if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
												VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] = ::Missed;//и последующее их раскрытие игроку с заменой на "промах"
											if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Hit)
												//проверка ячеек вокруг проверяемых вокруг проверяемых на наличие подбитых палуб корабля
											{
												for (int q = -1; q < 2; q++)
												{
													for (int w = -1; w < 2; w++)
													{
														//проще говоря - вокруг подбитого 4-х палубного корабля ставятся промахи.
														if (ComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
															VisibleComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			ShowAfterShot(PlayerMap, VisibleComputerMap); //вывод в консоль изменений в массивах после выстрела в корабль
			cout << " strike! shoot again.\n";
			ControlsForFight();
			test = false; //переменная проверки на полностью подбитый корабль присваивает первоначальное значение для последующей повторной проверки
			continue;
		}
	}
	ShowAfterShot(PlayerMap, VisibleComputerMap);
	cout << " you miss!\n";
	ControlsForFight();
}

//ф-ция выстрела сложного компьютера против компьютера
void ShotCompHardAsPlayer(unsigned char ComputerMap[][::size], unsigned char VisibleComputerMap[][::size], unsigned char PlayerMap[][::size])
{
	srand(time(NULL));
	COORD c;
	bool status2 = true;
	while (status2 == true)
	{
		if (_kbhit() && (_getch() == 32))
			system("pause");
		else if (_kbhit() && (_getch() == 27))
		{
			::CountFirst1 = 0, ::CountFirst2 = 0;
			::TestDestroy = false, ::TestDestroy2 = false;
			main();
		}
		if (::CountFirst2 == 0) //при первом выстреле (до поражения нового корабля (первый ход и каждый последующий ход после полного уничтожения корабля))
		{
			int countpass2{ 0 }, dots2{0};
			//компьютер находит ячейку, вокруг которой уже нет подбитых ячеек. (более логичный выбор при игре в морской бой)
			while (countpass2 < 20) //если программа пыталась выбрать ячейку, вокруг которой нет уже подбитых, 35 раз. То таких на поле уже не осталось, и координаты принимаются.
			{
				dots2 = 0;
				c = { (short)(rand() % 10 + 2), (short)(rand() % 10 + 14) };
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (i == 0 && j == 0)
							continue;
						if (VisibleComputerMap[c.Y - 13 + i][c.X - 1 + j] == ::Missed) //если вокруг выбранной ячейки есть хоть одна ячейка в которую уже попадали
							dots2++;
					}
				}
				countpass2++;
				if (dots2 == 0) //в приориетете выстрелить в ту ячейку, вокруг которой еще ничего неизвестно
				{
					countpass2 = 0;//обнуление кол-во попыток поиска области для выстрела
					break; //цикл прерываем, т.к. ячейка подходящая
				}
				else if (countpass2 == 19 && dots2 == 1) //если на поле отсутсвуют ячейки вокург которых нет неивестных областей, то логичнее стрелять в то место, вокруг которого только одна простреленная область
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 2) //и т.д. по образу предыдущего выстрела
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 3)
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 4)
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 5)
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 6)
				{
					countpass2 = 0;
					break;
				}
				else if (countpass2 == 19 && dots2 == 7)
				{
					countpass2 = 0;
					break;
				}
				else
					continue;// если найденная координата не подходящая, ищем заного
			}
			::Coord2X = c.X; //записываем коодинаты первой подбитой палубы
			::Coord2Y = c.Y;
		}
		else
		{
			if (::CountFirst2 == 1) //компьютер подбил первую палубу корабля
			{
				//это для того чтобы, компьютер выстрелил по горизонали или вертикали от предыдущего выстрела
				c.Y = (rand() % 3 + (::Coord2Y - 1)); //координата У в диапазоне 3 от предыдущей (пример: коорд У была 5. сейчас она может быть в диапазоне от 4 до 6.)
				c.X = (rand() % 3 + (::Coord2X - 1)); //координата Х в диапазоне 3 от предыдущей
				if (c.Y != ::Coord2Y) //если коодината У изменилась
					c.X = ::Coord2X; //то координата Х не меняется
				if (c.X != ::Coord2X)//если коодината Х изменилась
					c.Y = ::Coord2Y; //то координата У не меняется
				::Coord2X2 = c.X;//записываем коодинаты 2 подбитой палубы
				::Coord2Y2 = c.Y;
			}
			if (::CountFirst2 == 2) //компьютер подбил вторую палубу корабля
			{
				if (::Coord2Y == ::Coord2Y2) //если корабль расположен горизонтально
				{
					c.Y = ::Coord2Y; //выстрел будет слева или справа
					if (::Coord2X2 > ::Coord2X) //если 2 палуба правее первой
						c.X = (rand() % 4 + (::Coord2X2 - 2)); //координата Х в диапазоне 4 от правой координаты (пример: коорд Х была 5. сейчас она может быть в диапазоне от 3 до 6.)
					else
						c.X = (rand() % 4 + (::Coord2X - 2));//если 1 палуба правее второй то координата Х в диапазоне 4 от правой координаты
				}
				else
				{
					c.X = ::Coord2X; //выстрел будет снизу или сверху
					if (::Coord2Y2 > ::Coord2Y) //если 2 палуба ниже первой
						c.Y = (rand() % 4 + (::Coord2Y2 - 2));//координата У в диапазоне 4 от нижней координаты
					else
						c.Y = (rand() % 4 + (::Coord2Y - 2));//если 1 палуба ниже второй то координата У в диапазоне 4 от нижней координаты
				}
				::Coord2X3 = c.X;//записываем коодинаты 3 подбитой палубы
				::Coord2Y3 = c.Y;
			}
			if (::CountFirst2 == 3)//компьютер подбил 3 палубу корабля
			{
				if (::Coord2Y == ::Coord2Y2)//если корабль расположен горизонтально
				{
					c.Y = ::Coord2Y; //выстрел будет слева или справа
					//если 2 подбитая палуба посередине то координата У в диапазоне 5 от координаты второй подбитой палубы
					if ((::Coord2X2 > ::Coord2X && ::Coord2X3 > ::Coord2X2 && ::Coord2X3 > ::Coord2X) || (::Coord2X2 < ::Coord2X && ::Coord2X3 < ::Coord2X2 && ::Coord2X3 < ::Coord2X))
						c.X = (rand() % 5 + (::Coord2X2 - 2));
					//если 1 подбитая палуба посередине то координата У в диапазоне 5 от координаты первой подбитой палубы
					if ((::Coord2X2 < ::Coord2X && ::Coord2X3 > ::Coord2X2 && ::Coord2X3 > ::Coord2X) || (::Coord2X2 > ::Coord2X && ::Coord2X3 < ::Coord2X2 && ::Coord2X3 < ::Coord2X))
						c.X = (rand() % 5 + (::Coord2X - 2));
				}
				else
				{
					c.X = ::Coord2X; //выстрел будет снизу или сверху
					//если 2 подбитая палуба посередине то координата У в диапазоне 5 от координаты второй подбитой палубы
					if ((::Coord2Y2 > ::Coord2Y && ::Coord2Y3 > ::Coord2Y2 && ::Coord2Y3 > ::Coord2Y) || (::Coord2Y2 < ::Coord2Y && ::Coord2Y3 < ::Coord2Y2 && ::Coord2Y3 < ::Coord2Y))
						c.Y = (rand() % 5 + (::Coord2Y2 - 2));
					//если 1 подбитая палуба посередине то координата У в диапазоне 5 от координаты первой подбитой палубы
					if ((::Coord2Y2 < ::Coord2Y && ::Coord2Y3 > ::Coord2Y2 && ::Coord2Y3 > ::Coord2Y) || (::Coord2Y2 > ::Coord2Y && ::Coord2Y3 < ::Coord2Y2 && ::Coord2Y3 < ::Coord2Y))
						c.Y = (rand() % 5 + (::Coord2Y - 2));
				}
			}
		}
		if (VisibleComputerMap[c.Y - 13][c.X - 1] == ::Missed || VisibleComputerMap[c.Y - 13][c.X - 1] == ::Hit)
			continue;
		if (ComputerMap[c.Y - 13][c.X - 1] == ::Empty) //при выстреле в ячейку без корабля
		{
			VisibleComputerMap[c.Y - 13][c.X - 1] = ::Missed; //скрытая ячейка санет ячейкой с символом "промах" (::dot)
			Sleep(1000);
			PlaySound(L"miss", NULL, SND_RESOURCE | SND_ASYNC);
			status2 = false; //цикл прекратиться, ход перейдет к компьютеру
			break;
		}
		if (ComputerMap[c.Y - 13][c.X - 1] == ::Ship)
		{
			VisibleComputerMap[c.Y - 13][c.X - 1] = ::Hit;
			::CountFirst2++; //счетчик ранений корабля увеличился
			Sleep(1000);
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (i == 0 && j == 0)
						continue;
					if (ComputerMap[c.Y - 13 + i][c.X - 1 + j] == ::Ship) //проверка ячеек вокруг подбитой на наличие кораблей
					{
						for (int h = -1; h < 2; h++)
						{
							for (int b = -1; b < 2; b++)
							{
								if (ComputerMap[c.Y - 13 + i + h][c.X - 1 + j + b] == ::Ship) //проверка ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей
								{
									for (int n = -1; n < 2; n++)
									{
										for (int m = -1; m < 2; m++)
										{
											//проверка ячеек вокруг проверяемых ячеек вокруг проверяемых ячеек вокруг подбитой на наличие кораблей при этом не являющихся видимыми подбитыми
											if (ComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] == ::Ship && VisibleComputerMap[c.Y - 13 + i + h + n][c.X - 1 + j + b + m] != ::Hit)
											{
												PlaySound(L"hit", NULL, SND_RESOURCE | SND_ASYNC);
												::TestDestroy2 = true; //если проверка пройдена хотябы раз, то переменная test поменяет значение на 1.
												break;
												//вся эта проверка проверяет подбит ли полностью корабль. при test=true корабль не убить полностью.
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if (::TestDestroy2 == false)
			{
				PlaySound(L"damage", NULL, SND_RESOURCE | SND_ASYNC);
				::CountFirst2 = 0;
			}
			for (int n = -1; n < 2; n++)
			{
				for (int m = -1; m < 2; m++)
				{
					//проще говоря - вокруг подбитого 1-о палубного корабля ставятся промахи.
					if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Empty && ::TestDestroy2 == false) //если корабль полностью подбит(test=false) и вокруг выбранной пустые ячейки
						VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] = ::Missed; //то все ячейки заполненные "пусто" вокруг становится видимыми и заполняюся "промахом" по правилам игры.
					if (ComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Ship && VisibleComputerMap[c.Y - 13 + n][c.X - 1 + m] == ::Hit && ::TestDestroy2 == false)
						//проверка ячеек вокруг на наличие подбитых палуб корабля при условии что корабль полностью подбит
					{
						for (int z = -1; z < 2; z++)
						{
							for (int x = -1; x < 2; x++)
							{
								//проще говоря - вокруг подбитого 2-х палубного корабля ставятся промахи.
								if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
									VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
								if (ComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z][c.X - 1 + m + x] == ::Hit)
									//проверка ячеек вокруг проверяемых на наличие подбитых палуб корабля
								{
									for (int r = -1; r < 2; r++)
									{
										for (int t = -1; t < 2; t++)
										{
											//проще говоря - вокруг подбитого 3-х палубного корабля ставятся промахи.
											if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
												VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] = ::Missed;//и последующее их раскрытие игроку с заменой на "промах"
											if (ComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Ship && VisibleComputerMap[c.Y - 13 + n + z + r][c.X - 1 + m + x + t] == ::Hit)
												//проверка ячеек вокруг проверяемых вокруг проверяемых на наличие подбитых палуб корабля
											{
												for (int q = -1; q < 2; q++)
												{
													for (int w = -1; w < 2; w++)
													{
														//проще говоря - вокруг подбитого 4-х палубного корабля ставятся промахи.
														if (ComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] == ::Empty) //проверка ячеек на наличие ячеек заполненные "пусто"
															VisibleComputerMap[c.Y - 13 + n + z + r + q][c.X - 1 + m + x + t + w] = ::Missed; //и последующее их раскрытие игроку с заменой на "промах"
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			ShowAfterShot(PlayerMap, VisibleComputerMap);
			cout << " strike! shoot again.\n";
			ControlsForFight();
			::TestDestroy2 = false;
			EndGame(PlayerMap, ComputerMap, VisibleComputerMap);
			continue;
		}
	}
	ShowAfterShot(PlayerMap, VisibleComputerMap);
	cout << " miss!\n";
	ControlsForFight();
}

