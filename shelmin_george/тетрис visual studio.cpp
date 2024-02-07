//#include <Windows.h>
//#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>
#include <conio.h>
//#include <mutex>

using namespace std;
using namespace chrono;

//mutex th;

short int const n = 8, m = 16;
//vector<vector<int> > a(m+6, vector<int>(n+2));
int a[m+6][n+2];
short int x[]={ 0,0,0,0}, y[] = { 0,0,0,0 };
short int d = 0;
short int pos = 1;
int p = 450, p1 = 0;
short int r = 0;
int simb = 254;//206
char LRS;
int score = 0;
bool random1 = 0;
bool s = 0;
short int s1 = 2;

void cls()
{
	//Sleep(p);
	this_thread::sleep_for(chrono::milliseconds(p));
	system("cls");

	for (short int i = 1; i < m + 5; i += 1)
	{
		
		for (short int j = 0; j < n + 2; j += 1)
		{
			if (a[i][j] >= 3) a[i][j] = 0;
			if ((j != 0) && (j != n + 1)) {
				switch (a[i][j])
				{
				case 0:
					cout << " " << " ";
					break;
				case 1:
					cout << " " << char(simb);
					break;
				case 2:
					cout << " " << char(254);
					break;
				}
			}
			else { cout << "|"; }
		}
		
		cout << endl;
	}
	cout << "\n   score:   " << score << endl;
}
void cls1()
{
	//Sleep(p1);
	this_thread::sleep_for(chrono::milliseconds(p1));
	system("cls");

	for (short int i = 1; i < m + 5; i += 1)
	{
		
		for (short int j = 0; j < n + 2; j += 1)
		{
			if (a[i][j] >= 3) a[i][j] = 0;
			if ((j != 0) && (j != n + 1)) {
			switch (a[i][j])
			{
			case 0:
				cout << " " << " ";
				break;
			case 1:
				cout << " " << char(simb);
				break;
			case 2:
				cout << " " << char(254);
				break;
			}
			}
			else { cout << "|"; }
			
		}
		
		cout << endl;
	}
	cout << "\n   score:   " << score << endl;
}


class object
{
public:
	void setkubik()
	{
		

		x[0] = 4+d; x[1] = 5+d; x[2] = 4+d; x[3] = 5+d;
		y[0] = 1+d; y[1] = 1+d; y[2] = 2+d; y[3] = 2+d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
	}
	void setPalka()
	{
		
		x[0] = 5 + d; x[1] = 5 + d; x[2] = 5 + d; x[3] = 5 + d;
		y[0] = 1 + d; y[1] = 2 + d; y[2] = 3 + d; y[3] = 4 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
	}
	void setUgolok1()
	{
		

		x[0] = 5 + d; x[1] = 5 + d; x[2] = 5 + d; x[3] = 4 + d;
		y[0] = 1 + d; y[1] = 2 + d; y[2] = 3 + d; y[3] = 3 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
		
	}
	void setUgolok2()
	{
		

		x[0] = 4 + d; x[1] = 4 + d; x[2] = 4 + d; x[3] = 5 + d;
		y[0] = 1 + d; y[1] = 2 + d; y[2] = 3 + d; y[3] = 3 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
	
	}
	void setTavr()
	{
		

		x[0] = 5 + d; x[1] = 4 + d; x[2] = 5 + d; x[3] = 6 + d;
		y[0] = 1 + d; y[1] = 2 + d; y[2] = 2 + d; y[3] = 2 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
	}
	void setZagogulina1()
	{
	

		x[0] = 3 + d; x[1] = 4 + d; x[2] = 4 + d; x[3] = 5 + d;
		y[0] = 1 + d; y[1] = 1 + d; y[2] = 2 + d; y[3] = 2 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
	}
	void setZagogulina2()
	{
		

		x[0] = 6 + d; x[1] = 5 + d; x[2] = 5 + d; x[3] = 4 + d;
		y[0] = 1 + d; y[1] = 1 + d; y[2] = 2 + d; y[3] = 2 + d;
		a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
		
	}
	void random()
	{
		if (!random1)
		{
			r = rand() % 7;
			random1 = 1;
			//диапазон от 0 до 6
			switch (r)
			{
			case 0:
				setkubik();
				break;
			case 1:
				setPalka();
				break;
			case 2:
				setUgolok1();
				break;
			case 3:
				setUgolok2();
				break;
			case 4:
				setTavr();
				break;
			case 5:
				setZagogulina1();
				break;
			case 6:
				setZagogulina2();
				break;
			
			}

			cls();
			pos = 1;
		}
	}
}setup;

class object1
{	
public:
	void moveDown()
	{
		short int string = -1;
		short int count = 0;
		if (random1)
		{
			//while ((clock() / CLK_TCK) % p / 1000 > 30) { Sleep(0.1); }//криво но работает - наладить уже в ардуино

			if (x[0] > 0)
			{
				a[y[0] - d][x[0] - d] = 0;	a[y[1] - d][x[1] - d] = 0;	a[y[2] - d][x[2] - d] = 0;	a[y[3] - d][x[3] - d] = 0;
			}
			if ((a[y[0] + 1 - d][x[0] - d] == 1) || (a[y[1] + 1 - d][x[1] - d] == 1) || (a[y[2] + 1 - d][x[2] - d] == 1) || (a[y[3] + 1 - d][x[3] - d] == 1))

			{
				a[y[0] - d][x[0] - d] = 1;	a[y[1] - d][x[1] - d] = 1;	a[y[2] - d][x[2] - d] = 1;	a[y[3] - d][x[3] - d] = 1;
				random1 = 0;

			}
			else
			{
				y[0] += 1; y[1] += 1; y[2] += 1; y[3] += 1;
				a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
			}
		}
		//s = 0;
			cls();
			//s = 1;
			if (!random1)
			{
				string = -1;
				for (int i = 1; i <= m + 4; i++)
				{
					for (int j = 1; j <= n+1; j++)
					{
						if ((a[i][j] != a[i][0]))
						{
							count = 1;
							break;
						}
						else
						{
							count = 0; 
						}
					}
					if (!count)
					{
						string += 1;
						score += 1+string;
						count = 1;
						for (int j = 1; j <= n; j++)
						{
							a[i][j] = 0;
						}
						//сдвиг поля
						for (int k = i; k >= 2; k--)
						{
							for (int j = 1; j <= n; j++)
							{
								a[k][j] = a[k-1][j];
							}
						}
					}
					
					
				}
				cls1();
			}
		
	}
	void moveRight(bool f)
	{
		if (f)
		{
			if ((a[y[0] - d][x[0] - d] == 1))
			{
				a[y[0] - d][x[0] - d] = 1;	a[y[1] - d][x[1] - d] = 1;	a[y[2] - d][x[2] - d] = 1;	a[y[3] - d][x[3] - d] = 1;
			}
			else
			{
				if (x[0] > 0)
				{//зачистка поля перед сдвигом фигуры
					a[y[0] - d][x[0] - d] = 0;	a[y[1] - d][x[1] - d] = 0;	a[y[2] - d][x[2] - d] = 0;	a[y[3] - d][x[3] - d] = 0;
				}
				if ((a[y[0] - d][x[0] + 1 - d] == 1) || (a[y[1] - d][x[1] + 1 - d] == 1) || (a[y[2] - d][x[2] + 1 - d] == 1)
					|| (a[y[3] - d][x[3] + 1 - d] == 1))
				{//если касается стены то поставить где была
					a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
				}
				else
				{
					x[0] += 1; x[1] += 1; x[2] += 1; x[3] += 1;
					a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
				}
			//	if(!s)
				//	this_thread::sleep_for(chrono::milliseconds(s1));
				cls1();
			}
		}
	}
	void moveLeft(bool f)
	{
		//какая то логическая переменная в мэйне для проверки нажатия кнопки etc мб функция
		if (f)
		{
			if ((a[y[0] - d][x[0] - d] == 1))
			{
				a[y[0] - d][x[0] - d] = 1;	a[y[1] - d][x[1] - d] = 1;	a[y[2] - d][x[2] - d] = 1;	a[y[3] - d][x[3] - d] = 1;
			}
			else
			{
				if (x[0] > 0)
				{//зачистка поля перед сдвигом фигуры
					a[y[0] - d][x[0] - d] = 0;	a[y[1] - d][x[1] - d] = 0;	a[y[2] - d][x[2] - d] = 0;	a[y[3] - d][x[3] - d] = 0;
				}
				if ((a[y[0] - d][x[0] - 1 - d] == 1) || (a[y[1] - d][x[1] - 1 - d] == 1) || (a[y[2] - d][x[2] - 1 - d] == 1)
					|| (a[y[3] - d][x[3] - 1 - d] == 1))
				{//если касается стены то поставить где была
					a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
				}
				else
				{
					x[0] -= 1; x[1] -= 1; x[2] -= 1; x[3] -= 1;
					a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
				}
			//	if (!s)
			//		this_thread::sleep_for(chrono::milliseconds(s1));
				cls1();
			}
		}
	}
	void spin(bool f)
	{
		short int x1[] = { 0,0,0,0 }, y1[] = { 0,0,0,0 };
		if (f)
		{
			if ((a[y[0] - d][x[0] - d] == 1))
			{
				a[y[0] - d][x[0] - d] = 1;	a[y[1] - d][x[1] - d] = 1;	a[y[2] - d][x[2] - d] = 1;	a[y[3] - d][x[3] - d] = 1;
			}
			else
			{
				if (x[0] > 0)
				{//зачистка поля перед поворотом фигуры
					a[y[0] - d][x[0] - d] = 0;	a[y[1] - d][x[1] - d] = 0;	a[y[2] - d][x[2] - d] = 0;	a[y[3] - d][x[3] - d] = 0;
				}
				x1[0] = x[0]; x1[1] = x[1]; x1[2] = x[2]; x1[3] = x[3];
				y1[0] = y[0]; y1[1] = y[1]; y1[2] = y[2]; y1[3] = y[3];
				
				switch (r)
				{
				case 0://кубик
					a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
					break;
				case 1://палка
					switch (pos)
					{
					case 1:
						x[0] -= 1;  x[2] += 1; x[3] += 2;
						y[0] += 2;  y[1] += 1; y[3] -= 1;
						break;
					case 2:
						x[0] += 1;  x[2] -= 1; x[3] -= 2;
						y[0] -= 1;  y[2] += 1; y[3] += 2;
						if (x[3]==-1)
						{
							x[0] += 1;  x[1] += 1; x[2] += 1; x[3] += 1;
						}
						break;
					case 3:
						x[0] -= 2;  x[1] -= 1; x[3] += 1;
						y[0] += 1;  y[2] -= 1; y[3] -= 2;
						if (x[0] == -1)
						{
							x[0] += 1;  x[1] += 1; x[2] += 1; x[3] += 1;
						}
						break;
					case 4:
						x[0] += 2;  x[1] += 1; x[3] -= 1;
						y[0] -= 2;  y[1] -= 1; y[3] += 1;
						break;
					}
					break;
				case 2://уголок1
					switch (pos)
					{
					case 1:
						x[0] += 2;  x[1] += 1; x[3] += 1;
						y[0] += 2;  y[1] += 1; y[3] -= 1;
						break;
					case 2:
						x[0] -= 2;  x[1] -= 1; x[3] += 1;
						y[0] += 2;  y[1] += 1; y[3] += 1;
						if (x[0] == -1)
						{
							x[0] += 1;  x[1] += 1; x[2] += 1; x[3] += 1;
						}
						break;
					case 3:
						x[0] -= 2;  x[1] -= 1; x[3] -= 1;
						y[0] -= 2;  y[1] -= 1; y[3] += 1;
						if (x[0] == -1)
						{
							x[0] += 1;  x[1] += 1; x[2] += 1; x[3] += 1;
						}
						break;
					case 4:
						x[0] += 2;  x[1] += 1; x[3] -= 1;
						y[0] -= 2;  y[1] -= 1; y[3] -= 1;
						break;
					}
					break;
				case 3://уголок2
					switch (pos)
					{
					case 1:
						x[0] += 2;  x[1] += 1; x[3] -= 1;
						y[0] += 2;  y[1] += 1; y[3] += 1;
						break;
					case 2:
						x[0] -= 2;  x[1] -= 1; x[3] -= 1;
						y[0] += 2;  y[1] += 1; y[3] -= 1;
						break;
					case 3:
						x[0] -= 2;  x[1] -= 1; x[3] += 1;
						y[0] -= 2;  y[1] -= 1; y[3] -= 1;
						break;
					case 4:
						x[0] += 2;  x[1] += 1; x[3] += 1;
						y[0] -= 2;  y[1] -= 1; y[3] += 1;
						break;
					}
					break;
				case 4://тавр
					switch (pos)
					{
					case 1:
						x[0] += 1;  x[1] += 1; x[3] -= 1;
						y[0] += 1;  y[1] -= 1; y[3] += 1;
						break;
					case 2:
						x[0] -= 1;  x[1] += 1; x[3] -= 1;
						y[0] += 1;  y[1] += 1; y[3] -= 1;
						break;
					case 3:
						x[0] -= 1;  x[1] -= 1; x[3] += 1;
						y[0] -= 1;  y[1] += 1; y[3] -= 1;
						break;
					case 4:
						x[0] += 1;  x[1] -= 1; x[3] += 1;
						y[0] -= 1;  y[1] -= 1; y[3] += 1;
						break;
					}
					break;
				case 5://загогулина1
					switch (pos)
					{
					case 1:
						x[0] += 2;  x[1] += 1; x[3] -= 1;
						y[1] += 1; y[3] += 1;
						break;
					case 2:
						x[0] -= 2;  x[1] -= 1; x[3] += 1;
						y[1] -= 1; y[3] -= 1;
						break;
					case 3:
						x[0] += 2;  x[1] += 1; x[3] -= 1;
						y[1] += 1; y[3] += 1;
						break;
					case 4:
						x[0] -= 2;  x[1] -= 1; x[3] += 1;
						y[1] -= 1; y[3] -= 1;
						break;
					}
					break;
				case 6://загогулина2
					switch (pos)
					{
					case 1:
						x[1] += 1; x[3] += 1;
						y[0] += 2;  y[1] += 1; y[3] -= 1;
						break;
					case 2:
						x[1] -= 1; x[3] -= 1;
						y[0] -= 2;  y[1] -= 1; y[3] += 1;
						break;
					case 3:
						x[1] += 1; x[3] += 1;
						y[0] += 2;  y[1] += 1; y[3] -= 1;
						break;
					case 4:
						x[1] -= 1; x[3] -= 1;
						y[0] -= 2;  y[1] -= 1; y[3] += 1;
						break;
					}
					break;
				}
				
				if ((x[0] == 0) || (x[1] ==0) || (x[2] ==0) || (x[3] ==0))
				{
					x[0] += 1; x[1] += 1; x[2] += 1; x[3] += 1;
				}
				if ((y[0] == 0) || (y[1] == 0) || (y[2] == 0) || (y[3] == 0))
				{
					y[0] += 1; y[1] += 1; y[2] += 1; y[3] += 1;
				}
				if ((x[0] == n+2) || (x[1] == n+2) || (x[2] == n+2) || (x[3] == n+2))
				{
					x[0] -= 1; x[1] -= 1; x[2] -= 1; x[3] -= 1;
				}
				

			/*	if ((x[0] == -1) || (x[1] == -1) || (x[2] == -1) || (x[3] == -1))
				{
					x[0] += 2; x[1] += 2; x[2] += 2; x[3] += 2;
				}
				if ((x[0] == n + 3) || (x[1] == n + 3) || (x[2] == n + 3) || (x[3] == n + 3))
				{
					x[0] -= 2; x[1] -= 2; x[2] -= 2; x[3] -= 2;
				}
				*/
				if ((a[y[0] - d][x[0] - d] == 1) || (a[y[1] - d][x[1] - d] == 1) || (a[y[2] - d][x[2] - d] == 1)
					|| (a[y[3] - d][x[3] - d] == 1))
				{
					if ((a[y[0] - 1 - d][x[0] - d] == 0) && (a[y[1] - 1 - d][x[1] - d] == 0) && (a[y[2] - 1 - d][x[2] - d] == 0)
						&& (a[y[3] - 1 - d][x[3] - d] == 0))
					{
						y[0] -= 1; y[1] -= 1; y[2] -= 1; y[3] -= 1;
					}
					else {
						if ((a[y[0] - d][x[0] - 1 - d] == 0) && (a[y[1] - d][x[1] - 1 - d] == 0) && (a[y[2] - d][x[2] - 1 - d] == 0)
							&& (a[y[3] - d][x[3] - 1 - d] == 0))
						{
							x[0] -= 1; x[1] -= 1; x[2] -= 1; x[3] -= 1;
						}
						else {
							if ((a[y[0] - d][x[0] + 1 - d] == 0) && (a[y[1] - d][x[1] + 1 - d] == 0) && (a[y[2] - d][x[2] + 1 - d] == 0)
								&& (a[y[3] - d][x[3] + 1 - d] == 0))
							{
								x[0] += 1; x[1] += 1; x[2] += 1; x[3] += 1;
							}
							else {
								if ((a[y[0] + 1 - d][x[0] - d] == 0) && (a[y[1] + 1 - d][x[1] - d] == 0) && (a[y[2] + 1 - d][x[2] - d] == 0)
									&& (a[y[3] + 1 - d][x[3] - d] == 0)&&((y[0]==m+5)|| (y[1] == m + 5)||(y[2] == m + 5)||(y[3] == m + 5)))
								{
									y[0] += 1; y[1] += 1; y[2] += 1; y[3] += 1;
								}
								else {//отсюда сдвиг на  1 по диагонали
									//нужно для него границу массива прописать
									if ((a[y[0] - 1 - d][x[0] -1 - d] == 0) && (a[y[1] - 1 - d][x[1] -1 - d] == 0) && 
									(a[y[2] - 1 - d][x[2] -1 - d] == 0)
										&& (a[y[3] - 1 - d][x[3] -1 - d] == 0))
									{
										y[0] -= 1; y[1] -= 1; y[2] -= 1; y[3] -= 1;
										x[0] -= 1; x[1] -= 1; x[2] -= 1; x[3] -= 1;
									}
									else {
										if ((a[y[0] -1 - d][x[0] + 1 - d] == 0) && (a[y[1] -1 - d][x[1] + 1 - d] == 0) 
										&& (a[y[2] -1 - d][x[2] +1 - d] == 0)
											&& (a[y[3] -1 - d][x[3] + 1 - d] == 0))
										{
											y[0] -= 1; y[1] -= 1; y[2] -= 1; y[3] -= 1;
											x[0] += 1; x[1] += 1; x[2] += 1; x[3] += 1;
										}
										else {
											if ((a[y[0]+1 - d][x[0]-1 - d] == 0) && (a[y[1]+1  - d][x[1] -1 - d] == 0) &&
												(a[y[2] +1- d][x[2] -1 - d] == 0) && (a[y[3]+1 - d][x[3] -1 - d] == 0))
											{
												y[0] += 1; y[1] += 1; y[2] += 1; y[3] += 1;
												x[0] -= 1; x[1] -= 1; x[2] -= 1; x[3] -= 1;
											}
											else {
												if ((a[y[0] + 1 - d][x[0]+1  - d] == 0) && (a[y[1] + 1 - d][x[1]+1  - d] == 0) &&
													(a[y[2] + 1 - d][x[2] +1- d] == 0) && (a[y[3] + 1 - d][x[3]+1  - d] == 0))
												{
													y[0] += 1; y[1] += 1; y[2] += 1; y[3] += 1;
													x[0] += 1; x[1] += 1; x[2] += 1; x[3] += 1;
												}
												else {
													x[0] = x1[0]; x[1] = x1[1]; x[2] = x1[2]; x[3] = x1[3];
													y[0] = y1[0]; y[1] = y1[1]; y[2] = y1[2]; y[3] = y1[3];
												}
											}
										}
									}
								}
							}
						}
					}
				}

				a[y[0] - d][x[0] - d] = 2;	a[y[1] - d][x[1] - d] = 2;	a[y[2] - d][x[2] - d] = 2;	a[y[3] - d][x[3] - d] = 2;
				
				if (pos < 4)
				{
					pos += 1;
				}
				else
				{
					pos = 1;
				}
				//if (!s)
				//	this_thread::sleep_for(chrono::milliseconds(s1));
				cls1();
			}
		}
	}
}m0ve;

void gameplay()
{
	while ((a[1 + d][4 + d] != 1) && (a[1 + d][5 + d] != 1) && (a[2 + d][4 + d] != 1) && (a[2 + d][5 + d] != 1))
	{
		while (random1 != 0)
		{
			this_thread::sleep_for(chrono::milliseconds(3));
			//cin >> LRS;
			LRS = _getch();
		//	th.lock();
			switch (LRS)
			{
			case 'a':
				m0ve.moveLeft(1);
				break;
			case 'd':
				m0ve.moveRight(1);
				break;
			case 'w':
				m0ve.spin(1);
				break;
			case 's':
				m0ve.moveDown();
				//m0ve.moveDown();
				break;
			case ']':
				exit(0);
				break;
			}
		//	th.unlock();
		}
	}
}

int main()
{
	srand(time(NULL));
	//setlocale(LC_ALL, "rus");

	//начальный вывод пустого поля  
		for (short int i = 0; i < m + 6; i += 1)
		{
		
			for (short int j = 0; j < n + 2; j += 1)
			{
				if ((i > 0) && (i < m + 5) && (j > 0) && (j < n + 1))
				{
					a[i][j] = 0;
				}
				else
				{
					a[i][j] = 1;
				}
				switch (a[i][j])
				{
				case 0:
					cout << " " << " ";
					break;
				case 1:
					cout << " " << char(simb);
					break;
				case 2:
					cout << " " << char(254);
					break;
				}
			}
			
			cout << endl;
			
		}
		cout << "\n   score:   " << score << endl;
		//Sleep(p);
		this_thread::sleep_for(chrono::milliseconds(p));
	//	clock();

		thread th(gameplay);
		th.detach();

		while ((a[1 + d][4 + d] != 1) && (a[1 + d][5 + d] != 1)&& (a[2 + d][4 + d] != 1) && (a[2 + d][5 + d] != 1))
		{
			setup.random();

			while (random1 != 0)
			{
			/*	cin >> LRS;
				switch (LRS)
				{
				case 'a':
					m0ve.moveLeft(1);
					break;
				case 'd':
					m0ve.moveRight(1);
					break;
				case 'w':
					m0ve.spin(1);
					break;
				case 's':
					m0ve.moveDown();
					m0ve.moveDown();
					break;
				case ']':
					exit(0);
					break;
				}*/
				if ((score > 50)&&(score<500))p -= score/2;
				m0ve.moveDown();
				//cls1();
				if ((score > 50) && (score < 500))p += score/2;
				if (score >= 500)
				{
					cout << "\n!!congratulations!!\n\n";
					exit(0);
				}
			} 
		} 
		cout << "\n game over" << endl;
}
