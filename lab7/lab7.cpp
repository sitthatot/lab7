
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}


void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void erase_ship(int x, int y) {
	gotoxy(x, y);
	printf("       ");
}

void erase_bg(int x, int y) {
	gotoxy(x, y);
	setcolor(0, 0);
	printf("       ");
}

void draw_ship(int x, int y)
{
	setcolor(2, 4);
	gotoxy(x, y);
	printf(" <-0-> ");
}

void draw_bullet(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y);
	printf("^");
	Beep(700, 30);
}
void draw_stars(int x, int y) {
		gotoxy(x, y);
		printf("*");
}
void erase(int x, int y) {
	gotoxy(x, y);
	setcolor(7, 0);
	printf(" ");
}
struct Bullet {
	int active = 0;
	int x = 0, y = 0;
};

void board(int score) {
	gotoxy(70,1);
	setcolor(7, 0);
	printf("score: %d",score);
}

char cursor(int x, int y) {//เช็ก Char ที่ตำแหน่ง x,y
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read)) return '\0';
	else
		return buf[0];
}

int main()
{
	int score = 0;
	srand(time(NULL));
	for (int i = 0; i < 20; i++) {
		draw_stars(10+rand() % 61, 2 + rand() % 5);
	}
	Bullet bullets[5];
	//c.speed = 10;
	char ch = ' ';
	int x = 38, y = 29;
	int press = 0;
	setcursor(0);
	draw_stars(x, y);
	draw_ship(x, y);
	board(score);
	do {
		if (_kbhit()) {
			ch = _getch();

			if (ch == 'a' && x > 0) {
				press = 1;
			}
			if (ch == 'd' && x < 73) {
				press = 2;
			}
			if (ch == 's') {
				press = 3;
			}
			if (ch == ' ') {
				for (int i = 0; i < 5; i++) {
					if (bullets[i].active == 0) {
						bullets[i].active = 1;
						bullets[i].x = x + 3;
						bullets[i].y = y;
						break;
					}

				}
			}
			//if (ch == 'w' && y > 0) { erase_ship(x, y); erase_bg(x, y); draw_ship(x, --y); }
			fflush(stdin);
		}
		for (int i = 0; i < 5; i++) {
			if (bullets[i].active == 1) {
				erase(bullets[i].x, bullets[i].y);//erase bullet
				if (cursor(bullets[i].x, bullets[i].y - 1) == '*') {
					bullets[i].active = 0;//status close
					Beep(2000, 100);
					erase(bullets[i].x, bullets[i].y - 1);//erase star
					draw_stars(10 + rand() % 61, 2 + rand() % 5);//new star create
					board(++score);
				}
				else if (bullets[i].y > 0) {//ขอบบน
					draw_bullet(bullets[i].x, --bullets[i].y);
				}
				
				else {
					bullets[i].active = 0;
				}

			}

		}
		if (press == 1 && x > 0) {
			erase_ship(x, y);
			erase_bg(x, y);
			draw_ship(--x, y);
		}
		else if (press == 2 && x < 73) {
			erase_ship(x, y);
			erase_bg(x, y);
			draw_ship(++x, y);
		}
		else{
			erase_ship(x, y);
			erase_bg(x, y);
			draw_ship(x, y);
		}
		Sleep(100);
	} while (ch != 'x');
	return 0;
}
