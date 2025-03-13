 #define _CRT_SECURE_NO_WARNINGS
#define MAX_X 52 // MAX_X�� �ִ��� 52 -> ����� ���� ������ ���� �� ����
#define MAX_Y 52 //	MAX_Y�� �ִ��� 52 -> ����� ���� ������ ���� �� ����
#define LEFT 75 // ��������Ű: 75
#define RIGHT 77 // ��������Ű: 77
#define UP 72 // ���ʹ���Ű: 72
#define DOWN 80 // �Ʒ�����Ű: 80
#define SPACEBAR 32 // �����̽���
#define ESC 27 // ESC

#include <stdio.h> // ǥ�� ����� �۾��� ���� �������
#include <conio.h> // Ű �Է��� �۾��� �����ֱ� ���� �������
#include <time.h> // ����ã���� �� �ð��� �����ֱ� ���� �������
#include <windows.h> // ������ ���� ���α׷��� ����� ���� �������


struct Node // Node ����ü
{
	BOOL mine; // ����
	BOOL strip; // ��� ����
	BOOL flag; // ���
	BOOL wall; // ����
	int mineCnt; // �ֺ� ���� ����
};

struct APoint // APoint ����ü
{
	int x; // �÷��̾��� x ��ǥ
	int y; // �÷��̾��� y ��ǥ
};

void mainMenu(); // ����ã���� ù ����ȭ���� �����ִ� �Լ� ����
void levelSelectMenu();  // ���̵��� �����ϴ� ȭ���� �����ִ� �Լ� ����

void initMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // ����ã�� �迭�� �ʱ�ȭ ��Ű�� ���� �Լ� ���� (�迭, ���̵�, ���ڰ���)
void randMine(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // ���� ���� �ֱ�, ���� ��ġ �����ϱ� ���� �Լ� ���� (�迭, ���̵�, ���ڰ���)
int gameStart(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // ����ã�⸦ �������� �� ������ �����ִ� �Լ� ���� (�迭, ���̵�, ���ڰ���)
void displayMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // Map ���¸� ��Ÿ���� �Լ� (�迭, ���̵�, ���ڰ���)
void mineRecursive(struct Node(*gameMap)[MAX_Y], int tmpX, int tmpY); // ���ڰ� ���� ������ ����ؼ� Ž���ϰ� ���� ������ ��� �� ������ �����ִ� �Լ� (�迭, ���� ��, ���� ��)
int resultFunc(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int tmpRetMineCnt); // ���ڸ� ã�� �� ���� ���� �Ǵ��ϴ� �Լ� (�迭, ���̵�, ���ڰ���, ã�ƾ� �� ���ڰ����� ��Ÿ���� ����)
int levelSelect(struct Node(*gameMap)[MAX_Y]); // ���� ���̵��� �����ϴ� �Լ� (�迭)

int randx(int x, int y); // ���ڸ� �������� ��ġ�ϴ� �Լ�
void gotoxy(int x, int y); // ���� Ŀ�� ��ġ�� �����ϱ� ���� �Լ�
void textcolor(int color_number); // Windows.h ������Ͽ� ���ԵǾ� �ִ� �Լ�, �ܼ� â�� ��µ� ���ڵ��� ���� ������

//����� ���� ����� ���� ���� �߰�
int gameLevel_user;
//����� ���� ������ ���� ���� �߰�
//������ ������ 10���� �ʱ�ȭ <- �ʼ�!
int mineCntSet_u = 10;

//////////////////////////////////////////////////////////////////////////////////

void main()
{
	struct Node gameMap[MAX_X][MAX_Y];
	//max_x �� ���濡 ���� �ʱ� ������ ���� ������ �� ����
	int gameLevel = 42;
	int select;
	int retMineCnt = 10;
	int elapsedTime = 0;
	int level1 = 999, level2 = 999, level3 = 999;

	while (1)
	{
		system("cls");
		mainMenu();
		fflush(stdin); scanf("%d", &select);
		system("cls");

		switch (select)
		{
		case 1:

			initMap(gameMap, gameLevel, &retMineCnt);
			elapsedTime = gameStart(gameMap, gameLevel, &retMineCnt);

			//max_x �� ���濡 ���� ���� ������ �� ����
			if (gameLevel == 42)		if (level1 > elapsedTime)	level1 = elapsedTime;
			else if (gameLevel == 37)	if (level2 > elapsedTime)	level2 = elapsedTime;
			else if (gameLevel == 32)	if (level3 > elapsedTime)	level3 = elapsedTime;
			break;
		case 2:
			gameLevel = levelSelect(gameMap);
			break;
		case 3:
			printf("�Ǣ� �ְ� ��� �Ǣ�\n");
			printf(" 1.�� �� : %d \n", level1);
			printf(" 2.�� �� : %d \n", level2);
			printf(" 3.�� �� : %d \n", level3);
			system("pause");
			break;
			// ����� ���� �ɼ� �߰�
		case 4:

			printf("�Ǣ� ����� ���� �Ǣ�\n");
			while (1)
			{
				printf("����� ���� ����(n*n) : \n");
				printf("1~50�� �� ����\n");
				scanf("%d", &gameLevel_user);
				//����ڰ� ���ϴ� ����� ������ �Է� ����

				if (gameLevel_user > 50) {
					printf("����� ������ 50���� ������ �ȵ˴ϴ�.\n");
					printf("�ٽ� �Է����ּ���.\n");
					printf("\n");
					//����ڰ� ������ ������ �߸��Է��Ͽ��� ��� ����� �Է��� ������ ���� �ݺ�
					//�߸� �Է��� ��� ���� ������ ���� ���� �ʱ� ������ �����ϰ� �޾ƾ� ��

				}
				else
				{
					gameLevel = 50 - gameLevel_user;
					//������ �Լ��鿡�� ����ϱ� ���� gameLevel ������ ���ϴ� ����� ������ ���� �� �ֵ��� ����
					//�� �� ����� ������ �� ����� ������ ����
					//���� 50-gameLevel_user�� gameLevel�� �Ǵ� ��

					//����ڰ� ����� �Է��ߴٸ� ���� �ݺ��� ����
					break;
				}

			}
			//������ ������ �����ϱ� ���� ��� ȭ���� ����
			system("pause");

			printf("������ ���� ���� : \n");
			while (1)
			{
				scanf("%d", &mineCntSet_u);
				//����ڰ� ���ϴ� ������ ������ �Է� ����
				//�̶� mineCntSet_u ������ ���������̱� ������, ���Ŀ� �ٸ� �Լ������� ��� ��� ����

				if (mineCntSet_u > gameLevel_user * gameLevel_user)
				{
					printf("������ ������ ����� ������ �ʰ��� �� �����ϴ�.\n");
					printf("�ٽ� �Է����ּ���.\n");
					printf("\n");
					// ������ ������ ����� ������ �ʰ����� �ʵ��� ����
				}
				else
				{
					break;
				}
			}

			break;

		case 0:
			puts("������ �����մϴ�");
			return;
		default:
			break;
		}
	}
}


void mainMenu()
{
	textcolor(14);
	puts(" �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
	puts("��       �Ǣ� ���� ã�� ���� �Ǣ�        ��");
	puts("��                                   ��");
	puts("��          1. ���� ����             ��");
	puts("��                                   ��");
	puts("��          2. ���̵� ����           ��");
	puts("��                                   ��");
	puts("��          3. �ְ� ����             ��");
	puts("��                                   ��");
	puts("��          4. ����� ����           ��");
	puts("��                                   ��");
	puts("��          0. ��     ��             ��");
	puts("��                                   ��");
	puts(" �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
	printf(" Select > ");
}

void levelSelectMenu()
{
	textcolor(11);
	puts("  �� �� �� �� �� �� �� �� �� �� �� �� �� �� ");
	puts("��      �Ǣ� ���̵� ���� �Ǣ�      ��");
	puts("��                             ��");
	puts("��        1. ��     ��         ��");
	puts("��                             ��");
	puts("��        2. ��     ��         ��");
	puts("��                             ��");
	puts("��        3. ��     ��         ��");
	puts("��                             ��");
	puts("  �� �� �� �� �� �� �� �� �� �� �� �� �� �� ");
	printf(" Select > ");
}


void initMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt)
{
	int i, j;
	int cx, cy;

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			gameMap[i][j].wall = 0;
			gameMap[i][j].flag = 0;
			gameMap[i][j].mine = 0;
			gameMap[i][j].strip = 0;
			gameMap[i][j].mineCnt = 0;
		}
	}

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (i == 0 || i == MAX_X - gameLevel - 1 || j == 0 || j == MAX_Y - gameLevel - 1)
				gameMap[i][j].wall = 1;
			else
				gameMap[i][j].strip = 0;
		}
	}
	randMine(gameMap, gameLevel, retMineCnt);

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			for (cx = i - 1; cx < i + 2; cx++)
			{
				for (cy = j - 1; cy < j + 2; cy++)
				{
					if (gameMap[cx][cy].mine == 1)
					{
						gameMap[i][j].mineCnt++;
					}
				}
			}
		}
	}
}

void randMine(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt)
{
	int i, j;
	int mineCntSet = 0;
	//mineCntSet ������ �ʱⰪ ����
	mineCntSet = mineCntSet_u;

	*retMineCnt = mineCntSet;


	do
	{
		i = randx(1, MAX_X - gameLevel - 2);
		j = randx(1, MAX_Y - gameLevel - 2);

		if (gameMap[i][j].mine == 0)
		{
			gameMap[i][j].mine = 1;
			mineCntSet--;
		}
	} while (mineCntSet);
}


//////////////////////////////////////////////////////////////

int gameStart(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt)
// gameStart �Լ� (���ӽ��� �� ������ ����) 
{
	struct APoint player; // player ����ü 
	char ch;              // Ű����� �Է��� ��  
	int i, j;             // ��ǥ�� ���� �� 
	int tmpRetMinCnt;     // ã�ƾ� �� ���� ���� 
	time_t startTime;     // ���� ����  
	time_t endTime;       // ���� ���� 
	double elapsedTime = 0; // ��� �ð�  

	time(&startTime); // ���� ���� 
	tmpRetMinCnt = *retMineCnt;

	player.x = 2;
	player.y = 1;

	displayMap(gameMap, gameLevel, retMineCnt);
	gotoxy(player.x, player.y);

	while (1) // Ű���� �Է¿� ���� �÷��̾� ��ġ�� �̵� 
	{
		if (_kbhit())
		{
			ch = _getch(); // _getch() Ű �Է� �Լ� 
			switch (ch)
			{
			case LEFT:
				if (player.x - 1 > 0) gotoxy(player.x--, player.y);
				break;
			case RIGHT:
				if (player.x < MAX_Y - gameLevel - 1) gotoxy(player.x++, player.y);
				break;
			case UP:
				if (player.y - 1 > 0) gotoxy(player.x, --player.y);
				break;
			case DOWN:
				if (player.y + 1 < MAX_X - gameLevel - 1)   gotoxy(player.x, ++player.y);
				break;
			default:
				break;
			}

			if (ch == 'f' || ch == 'F')   // ��� ǥ��
			{
				if (gameMap[player.y][player.x].flag == 0) { gameMap[player.y][player.x].flag = 1;   --* retMineCnt; }
				else { gameMap[player.y][player.x].flag = 0;   ++* retMineCnt; }
				// ����� ������ ����� �߰��ϰ� ã�ƾ� �� ���ڰ����� �ϳ� ���δ� / ����� ������ ����� �����ϰ� ã�ƾ� �� ���ڰ����� �ϳ� �ø���  
				if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt) == 1)
					// ���� ������ ��� ���� = �� ���� ������ ���
				{
					time(&endTime); // ���� ���� 
					elapsedTime = difftime(endTime, startTime);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
					printf("�� ��� �ð� : %3d �� ", (int)elapsedTime); // ��� �ð� ǥ��
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
					system("pause");
					return (int)elapsedTime;
				}
			}
			else if (ch == SPACEBAR) // �ش� ��ġ�� ���� �ֺ��� ���� ���� ǥ�� 
			{
				if (gameMap[player.y][player.x].mine != 1)
				{
					mineRecursive(gameMap, player.y, player.x);
					if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt) == 1)
						// ���� ������ ��� ���� = �� ���� ������ ���
					{
						time(&endTime); // ���� ����
						elapsedTime = difftime(endTime, startTime);
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
						printf("�� ��� �ð� : %3d �� ", (int)elapsedTime); // ��� �ð� ǥ��
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
						system("pause");
						return (int)elapsedTime;
					}
				}
				else   // ������ ���
				{
					for (i = 0; i < MAX_X - gameLevel; i++)
					{
						for (j = 0; j < MAX_Y - gameLevel; j++)
						{
							gameMap[i][j].strip = 1; // ���� ��� ����� ���� 
						}
					}					

					displayMap(gameMap, gameLevel, retMineCnt);
					gotoxy(0, 0);
					for (i = 0; i < MAX_X - gameLevel; i++)
					{
						for (j = 0; j < MAX_Y - gameLevel; j++)
						{
							if (gameMap[i][j].wall == 1) { textcolor(15);	printf("��"); }
							else
							{
								if (gameMap[i][j].mine == 1) { textcolor(12);	printf("��"); }
								else { textcolor(12);	printf("��"); }
								// ���� ���ڰ� �ƴϸ� �� ������� �ٲ۴� 
							}
						}
						printf("\n");
					}
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
					printf("You die...!!  ");
					Beep(380, 200); // ���� �� �����
					Beep(330, 200);
					Beep(260, 200);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
					system("pause");
					return 999;
				}
			}
			else if (ch == ESC) // ���ư��� 
			{
				return 999;
			}
			displayMap(gameMap, gameLevel, retMineCnt);
			gotoxy(player.x, player.y);
		}
		Sleep(10);
	}
}

void displayMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt)
// ���� �� ��� (���� '��'��, �̰�ô ������ '��' �Ǵ� '��'��, ǥ�õ� ���ڴ� '��'�� ǥ��)
// (���ڰ� �ƴ� ���, �ֺ� ���� ������ ���� '��', '��', '��', ..., '��'�� ǥ��)
{
	int i, j;

	gotoxy(0, 0);
	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (gameMap[i][j].wall == 1) { textcolor(15);	printf("��"); }
			else
			{
				if (gameMap[i][j].strip == 0)
				{
					if (gameMap[i][j].flag == 0) { textcolor(3);	printf("��"); }
					else { textcolor(9);	printf("��"); }
				}
				else if (gameMap[i][j].strip == 1)
				{
					if (gameMap[i][j].mine == 1) { textcolor(12);	printf("��"); }
					else
					{
						if (gameMap[i][j].mineCnt == 0) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 1) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 2) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 3) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 4) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 5) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 6) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 7) { textcolor(14);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 8) { textcolor(14);	printf("��"); }
					}
				}
			}
		}
		printf("\n");
	}

	gotoxy(((MAX_X - gameLevel) * 2) + 3, 3);	textcolor(14);	printf("�� ��� ���� : %2d", *retMineCnt);
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
}

//////////////////////////////////////////////////////////////////////


//##��������� ����ã�⸦ �����ϴ� �Լ�##
//�� ���� ��ġ���� �����Ͽ� ���ڰ� ���� ������ ����ؼ� Ž���ϰ� ���� ���ο� �ִ� ��� �� ������ ������


void mineRecursive(struct Node(*gameMap)[MAX_Y], int tmpX, int tmpY)
{
	int i, j;

	if (gameMap[tmpX][tmpY].wall == 1) return;
	if (gameMap[tmpX][tmpY].strip == 1) return;
	if (gameMap[tmpX][tmpY].mineCnt)
	{
		gameMap[tmpX][tmpY].strip = 1;
		return;
	}

	gameMap[tmpX][tmpY].strip = 1;
	for (i = tmpX - 1; i < tmpX + 2; i++)
	{
		for (j = tmpY - 1; j < tmpY + 2; j++)
		{
			mineRecursive(gameMap, i, j);
		}
	}
}

//1. ���� ���� ��ġ�� ���� ��� �Լ��� ���������ϴ�.
//2. ���� ���� ��ġ�� �̹� ����� ��� �Լ��� ���������ϴ�.
//3. ���� ���� ��ġ�� ���ڰ� ������ ���� ��ġ�� ���� �ʰ� �Լ��� ���������ϴ�.
//4. ���� ��ġ�� ����, �ֺ��� �ִ� ��� ��ġ�� ���� ��������� mineRecursive �Լ��� ȣ���մϴ�.

//���� ������ ���� �Լ��� �� ������ �߰��� ������ �� ��ġ�� ����, �ش� ��ġ�� �������� ������ ��ġ�� Ž���ϸ� �� ������ ��� �����ִ� ������ �մϴ�.
//�̸� ���� ���� �÷��̾�� �� ������ ���� ������ ���� �� �ְ�, ������ ������ �� �ֽ��ϴ�.
//
//* �� ��������� �� �Լ��� ���� �ϴ°� ?
//�� ���� ��ġ�� �߽����� 8���⿡ ���� ��������� Ž���� ���ڸ� ã�Ƴ��� ���� ����, ��͸� ������� ������ �ݺ����� ����ؾ���.������ �̴� ��ȿ�����̰� �������� �������Ƿ� ��͹� �ۼ�



//##���ڸ� ã�� �Ŀ� ������ �������� �Ǵ��ϴ� �Լ�#

int resultFunc(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int tmpRetMineCnt)
{
	int i, j;
	int stripCnt = 0;
	int tmpCnt;
	tmpCnt = tmpRetMineCnt;
	/*FILE* fp;
	char data[1000];*/



	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (gameMap[i][j].wall != 1 && gameMap[i][j].strip == 0)	stripCnt++;
		}
	}

	//��for ���� Ȱ���� ��gameMap�� �迭���� ���� �ƴϰ� ���� ������� ���� ������ ������  stripCnt�� ���Ѵ�.
	//��, ���� ������� ���� ���� ������ ���� ������ ��,
	//���ӿ��� ������� ���� ���� ���ڰ� ���� ���� �ְ� ���� ���� ����.���� �� �κп��� ���ڸ� ã�� �Ŀ� ���� ������� ���� ���� �󸶳� ���Ҵ����� Ȯ����.
	//
	//for���� ����� i��j�� ��� ���տ� ����  gameMap[i][j]�� wall ���� 1�� �ƴϰ�(���� �ƴϰ�) strip���� 0��(������� ���� ����)���� �ִٸ� stripCnt�� 1���� ��Ŵ.
	//���� if ������  stripCnt��  tmpCnt�� ���� ���� ���� ���� ����

	if (*retMineCnt == 0 && stripCnt == tmpCnt)
	{
		displayMap(gameMap, gameLevel, retMineCnt);

		gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
		printf("Good...!!  ");
		gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
		Beep(260, 500);
		Beep(330, 500);
		Beep(380, 500);
		return 1;
	}
	return 0;
}

//�� if ���� Ȱ���� ã�ƾ� �� ������ ������ retMineCnt�� 0�̰�(���ڸ� �� ã�Ұų�), ���� ������� ���� ������ ������ stripCnt�� �ӽ������� ã�ƾ� �� ������ ���� tmpCnt�� ���ٸ�(������� ���� ������ ��� ���ڰ� �ִٸ�),
//displayMap�Լ��� ȣ�� �� ���� �� ���
//�޼��� "Good...!!  "�� �Է�
//return 1�� �Լ��� ����
//���ǹ��� �������� �ʴ� ��� return 0���� �Լ��� ����


//##����ڰ� ���� ���̵��� �����ϴ� �Լ�##.
int levelSelect(struct Node(*gameMap)[MAX_Y])
{
	int select;
	while (1)
	{
		system("cls");
		levelSelectMenu();
		fflush(stdin);	scanf("%d", &select);

		if (select == 1)
		{
			//�ʱ��� ������ ��� ������ ������ 10���� �ʱ�ȭ
			mineCntSet_u = 10;
			//max�� ������ ���� gameLevel�� ����
			return 42;
		}
		else if (select == 2)
		{
			//�߱��� ������ ��� ������ ������ 30���� �ʱ�ȭ
			mineCntSet_u = 30;
			//max�� ���忡 ���� gameLevel�� ����
			return 37;
		}
		else if (select == 3)
		{
			//����� ������ ��� ������ ������ 80���� �ʱ�ȭ
			mineCntSet_u = 80;
			//max�� ������ ���� gameLevel�� ����
			return 32;
		}
		else					continue;
	}
}


//##���ڸ� �������� ��ġ�ϴ� �Լ�##
int randx(int x, int y)
{
	static int flag = 0; //���� ������ flag�� ����
	int retVal;
	if (flag == 0)
	{
		srand(time(NULL));
		rand(); rand(); rand(); rand();
		srand(rand());
		flag = 1;
	}

	//�� if ���� ���� flag������ 0�� ��쿡�� ���� �õ�� �ʱ�ȭ �ϰ� 1�� ���� ������. 
		//���� �õ��, ������ ������ �� ����ϴ� ���ۼ��ڸ� �ǹ�.
			//���� �õ� �ʱ�ȭ��, ��ǻ�� ���ο��� � ����� ������ �� ���� �õ� ���� ������� ������ ����
			//-> ���������� ����
	//*�õ�� ������ �����ϱ� ���� �ʱⰪ���� �Ϲ������� ��ǻ���� ���� �ð����� �ʴ����� ���� ����ϴ� �Ϳ� ���� �κ�
		//������, rand()�� ����ϸ� �õ尪�� ������ ���� �־� rand()�� ������ ȣ�� �� srand()�Լ��� ���
	retVal = rand() % (y - x + 1) + x;
	return retVal;
}

//##���� Ŀ�� ��ġ�� �����ϱ� ���� �Լ�##

void gotoxy(int x, int y)
{
	COORD cd; //COORD��� ����ü ���� cd ����
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
// �� �Լ��� �ܼ� ȭ�鿡�� ��ġ�� �����ϴµ� ���, �ֿܼ��� �ؽ�Ʈ ��� UI�� �����ϴµ� ����

//##�ܼ� ����� �� ���� �Լ�##
void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}