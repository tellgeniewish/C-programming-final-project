 #define _CRT_SECURE_NO_WARNINGS
#define MAX_X 52 // MAX_X의 최댓값이 52 -> 사용자 맞춤 범위를 위해 값 수정
#define MAX_Y 52 //	MAX_Y의 최댓값이 52 -> 사용자 맞춤 범위를 위해 값 수정
#define LEFT 75 // 좌측방향키: 75
#define RIGHT 77 // 우측방향키: 77
#define UP 72 // 위쪽방향키: 72
#define DOWN 80 // 아래방향키: 80
#define SPACEBAR 32 // 스페이스바
#define ESC 27 // ESC

#include <stdio.h> // 표준 입출력 작업을 위한 헤더파일
#include <conio.h> // 키 입력의 작업을 도와주기 위한 헤더파일
#include <time.h> // 지뢰찾기의 총 시간을 보여주기 위한 헤더파일
#include <windows.h> // 윈도우 응용 프로그램을 만들기 위한 헤더파일


struct Node // Node 구조체
{
	BOOL mine; // 지뢰
	BOOL strip; // 블록 제거
	BOOL flag; // 깃발
	BOOL wall; // 벽면
	int mineCnt; // 주변 지뢰 갯수
};

struct APoint // APoint 구조체
{
	int x; // 플레이어의 x 좌표
	int y; // 플레이어의 y 좌표
};

void mainMenu(); // 지뢰찾기의 첫 메인화면을 보여주는 함수 선언
void levelSelectMenu();  // 난이도를 선택하는 화면을 보여주는 함수 선언

void initMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // 지뢰찾기 배열을 초기화 시키기 위한 함수 선언 (배열, 난이도, 지뢰갯수)
void randMine(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // 랜덤 지뢰 넣기, 지뢰 위치 저장하기 위한 함수 선언 (배열, 난이도, 지뢰갯수)
int gameStart(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // 지뢰찾기를 시작했을 때 게임을 보여주는 함수 선언 (배열, 난이도, 지뢰갯수)
void displayMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt); // Map 상태를 나타내는 함수 (배열, 난이도, 지뢰갯수)
void mineRecursive(struct Node(*gameMap)[MAX_Y], int tmpX, int tmpY); // 지뢰가 없는 영역을 계속해서 탐색하고 영역 내부의 모든 빈 공간을 열어주는 함수 (배열, 지뢰 행, 지뢰 열)
int resultFunc(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int tmpRetMineCnt); // 지뢰를 찾은 후 게임 끝을 판단하는 함수 (배열, 난이도, 지뢰갯수, 찾아야 할 지뢰갯수를 나타내는 변수)
int levelSelect(struct Node(*gameMap)[MAX_Y]); // 게임 난이도를 선택하는 함수 (배열)

int randx(int x, int y); // 지뢰를 무작위로 배치하는 함수
void gotoxy(int x, int y); // 현재 커서 위치를 설정하기 위한 함수
void textcolor(int color_number); // Windows.h 헤더파일에 포함되어 있는 함수, 콘솔 창에 출력된 글자들의 색을 변경함

//사용자 맞춤 블록의 개수 변수 추가
int gameLevel_user;
//사용자 맞춤 지뢰의 개수 변수 추가
//지뢰의 개수를 10으로 초기화 <- 필수!
int mineCntSet_u = 10;

//////////////////////////////////////////////////////////////////////////////////

void main()
{
	struct Node gameMap[MAX_X][MAX_Y];
	//max_x 값 변경에 따라 초급 레벨에 대한 변수의 값 수정
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

			//max_x 값 변경에 따라 레벨 변수의 값 수정
			if (gameLevel == 42)		if (level1 > elapsedTime)	level1 = elapsedTime;
			else if (gameLevel == 37)	if (level2 > elapsedTime)	level2 = elapsedTime;
			else if (gameLevel == 32)	if (level3 > elapsedTime)	level3 = elapsedTime;
			break;
		case 2:
			gameLevel = levelSelect(gameMap);
			break;
		case 3:
			printf("▤▤ 최고 기록 ▤▤\n");
			printf(" 1.초 급 : %d \n", level1);
			printf(" 2.중 급 : %d \n", level2);
			printf(" 3.고 급 : %d \n", level3);
			system("pause");
			break;
			// 사용자 맞춤 옵션 추가
		case 4:

			printf("▤▤ 사용자 맞춤 ▤▤\n");
			while (1)
			{
				printf("블록의 개수 설정(n*n) : \n");
				printf("1~50개 중 선택\n");
				scanf("%d", &gameLevel_user);
				//사용자가 원하는 블록의 개수를 입력 받음

				if (gameLevel_user > 50) {
					printf("블록의 개수는 50개가 넘으면 안됩니다.\n");
					printf("다시 입력해주세요.\n");
					printf("\n");
					//사용자가 지뢰의 개수를 잘못입력하였을 경우 제대로 입력할 때까지 무한 반복
					//잘못 입력할 경우 지뢰 실행이 전혀 되지 않기 때문에 엄격하게 받아야 함

				}
				else
				{
					gameLevel = 50 - gameLevel_user;
					//이후의 함수들에서 사용하기 위해 gameLevel 변수에 원하는 블록의 개수가 나올 수 있도록 설정
					//이 때 블록의 개수는 벽 블록을 제외한 값임
					//따라서 50-gameLevel_user가 gameLevel이 되는 것

					//사용자가 제대로 입력했다면 무한 반복을 멈춤
					break;
				}

			}
			//지뢰의 개수를 설정하기 전에 잠시 화면을 멈춤
			system("pause");

			printf("지뢰의 개수 설정 : \n");
			while (1)
			{
				scanf("%d", &mineCntSet_u);
				//사용자가 원하는 지뢰의 개수를 입력 받음
				//이때 mineCntSet_u 변수는 전역변수이기 때문에, 이후에 다른 함수에서도 계속 사용 가능

				if (mineCntSet_u > gameLevel_user * gameLevel_user)
				{
					printf("지뢰의 개수가 블록의 개수를 초과할 수 없습니다.\n");
					printf("다시 입력해주세요.\n");
					printf("\n");
					// 지뢰의 개수가 블록의 개수를 초과하지 않도록 제한
				}
				else
				{
					break;
				}
			}

			break;

		case 0:
			puts("게임을 종료합니다");
			return;
		default:
			break;
		}
	}
}


void mainMenu()
{
	textcolor(14);
	puts(" ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━");
	puts("┃       ▤▤ 지뢰 찾기 게임 ▤▤        ┃");
	puts("┃                                   ┃");
	puts("┃          1. 게임 시작             ┃");
	puts("┃                                   ┃");
	puts("┃          2. 난이도 선택           ┃");
	puts("┃                                   ┃");
	puts("┃          3. 최고 점수             ┃");
	puts("┃                                   ┃");
	puts("┃          4. 사용자 맞춤           ┃");
	puts("┃                                   ┃");
	puts("┃          0. 종     료             ┃");
	puts("┃                                   ┃");
	puts(" ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━");
	printf(" Select > ");
}

void levelSelectMenu()
{
	textcolor(11);
	puts("  ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ");
	puts("┃      ▤▤ 난이도 선택 ▤▤      ┃");
	puts("┃                             ┃");
	puts("┃        1. 초     급         ┃");
	puts("┃                             ┃");
	puts("┃        2. 중     급         ┃");
	puts("┃                             ┃");
	puts("┃        3. 고     급         ┃");
	puts("┃                             ┃");
	puts("  ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ━ ");
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
	//mineCntSet 변수의 초기값 설정
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
// gameStart 함수 (게임시작 및 진행의 역할) 
{
	struct APoint player; // player 구조체 
	char ch;              // 키보드로 입력한 값  
	int i, j;             // 좌표를 위한 값 
	int tmpRetMinCnt;     // 찾아야 할 지뢰 개수 
	time_t startTime;     // 시작 시점  
	time_t endTime;       // 종료 시점 
	double elapsedTime = 0; // 경과 시간  

	time(&startTime); // 게임 시작 
	tmpRetMinCnt = *retMineCnt;

	player.x = 2;
	player.y = 1;

	displayMap(gameMap, gameLevel, retMineCnt);
	gotoxy(player.x, player.y);

	while (1) // 키보드 입력에 따라 플레이어 위치를 이동 
	{
		if (_kbhit())
		{
			ch = _getch(); // _getch() 키 입력 함수 
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

			if (ch == 'f' || ch == 'F')   // 깃발 표시
			{
				if (gameMap[player.y][player.x].flag == 0) { gameMap[player.y][player.x].flag = 1;   --* retMineCnt; }
				else { gameMap[player.y][player.x].flag = 0;   ++* retMineCnt; }
				// 깃발이 없으면 깃발을 추가하고 찾아야 할 지뢰개수를 하나 줄인다 / 깃발이 있으면 깃발을 제거하고 찾아야 할 지뢰개수를 하나 늘린다  
				if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt) == 1)
					// 벽을 제외한 블록 개수 = 총 지뢰 개수일 경우
				{
					time(&endTime); // 게임 종료 
					elapsedTime = difftime(endTime, startTime);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
					printf("▤ 경과 시간 : %3d 초 ", (int)elapsedTime); // 경과 시간 표시
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
					system("pause");
					return (int)elapsedTime;
				}
			}
			else if (ch == SPACEBAR) // 해당 위치를 열고 주변의 지뢰 정보 표시 
			{
				if (gameMap[player.y][player.x].mine != 1)
				{
					mineRecursive(gameMap, player.y, player.x);
					if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt) == 1)
						// 벽을 제외한 블록 개수 = 총 지뢰 개수일 경우
					{
						time(&endTime); // 게임 종료
						elapsedTime = difftime(endTime, startTime);
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
						printf("▤ 경과 시간 : %3d 초 ", (int)elapsedTime); // 경과 시간 표시
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
						system("pause");
						return (int)elapsedTime;
					}
				}
				else   // 지뢰일 경우
				{
					for (i = 0; i < MAX_X - gameLevel; i++)
					{
						for (j = 0; j < MAX_Y - gameLevel; j++)
						{
							gameMap[i][j].strip = 1; // 맵의 모든 블록을 제거 
						}
					}					

					displayMap(gameMap, gameLevel, retMineCnt);
					gotoxy(0, 0);
					for (i = 0; i < MAX_X - gameLevel; i++)
					{
						for (j = 0; j < MAX_Y - gameLevel; j++)
						{
							if (gameMap[i][j].wall == 1) { textcolor(15);	printf("■"); }
							else
							{
								if (gameMap[i][j].mine == 1) { textcolor(12);	printf("※"); }
								else { textcolor(12);	printf("★"); }
								// 벽과 지뢰가 아니면 별 모양으로 바꾼다 
							}
						}
						printf("\n");
					}
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
					printf("You die...!!  ");
					Beep(380, 200); // 실패 시 경고음
					Beep(330, 200);
					Beep(260, 200);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
					system("pause");
					return 999;
				}
			}
			else if (ch == ESC) // 돌아가기 
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
// 게임 맵 출력 (벽은 '■'로, 미개척 지역은 '■' 또는 '¶'로, 표시된 지뢰는 '※'로 표시)
// (지뢰가 아닌 경우, 주변 지뢰 개수에 따라 '○', '①', '②', ..., '⑧'로 표시)
{
	int i, j;

	gotoxy(0, 0);
	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (gameMap[i][j].wall == 1) { textcolor(15);	printf("■"); }
			else
			{
				if (gameMap[i][j].strip == 0)
				{
					if (gameMap[i][j].flag == 0) { textcolor(3);	printf("■"); }
					else { textcolor(9);	printf("¶"); }
				}
				else if (gameMap[i][j].strip == 1)
				{
					if (gameMap[i][j].mine == 1) { textcolor(12);	printf("※"); }
					else
					{
						if (gameMap[i][j].mineCnt == 0) { textcolor(14);	printf("○"); }
						else if (gameMap[i][j].mineCnt == 1) { textcolor(14);	printf("①"); }
						else if (gameMap[i][j].mineCnt == 2) { textcolor(14);	printf("②"); }
						else if (gameMap[i][j].mineCnt == 3) { textcolor(14);	printf("③"); }
						else if (gameMap[i][j].mineCnt == 4) { textcolor(14);	printf("④"); }
						else if (gameMap[i][j].mineCnt == 5) { textcolor(14);	printf("⑤"); }
						else if (gameMap[i][j].mineCnt == 6) { textcolor(14);	printf("⑥"); }
						else if (gameMap[i][j].mineCnt == 7) { textcolor(14);	printf("⑦"); }
						else if (gameMap[i][j].mineCnt == 8) { textcolor(14);	printf("⑧"); }
					}
				}
			}
		}
		printf("\n");
	}

	gotoxy(((MAX_X - gameLevel) * 2) + 3, 3);	textcolor(14);	printf("▤ 깃발 개수 : %2d", *retMineCnt);
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
}

//////////////////////////////////////////////////////////////////////


//##재귀적으로 지뢰찾기를 수행하는 함수##
//→ 현재 위치에서 시작하여 지뢰가 없는 영역을 계속해서 탐색하고 영역 내부에 있는 모든 빈 공간을 열어줌


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

//1. 만약 현재 위치가 벽인 경우 함수를 빠져나갑니다.
//2. 만약 현재 위치가 이미 노출된 경우 함수를 빠져나갑니다.
//3. 만약 현재 위치에 지뢰가 있으면 현재 위치를 열지 않고 함수를 빠져나갑니다.
//4. 현재 위치를 열고, 주변에 있는 모든 위치에 대해 재귀적으로 mineRecursive 함수를 호출합니다.

//위의 과정을 통해 함수는 빈 공간을 발견할 때마다 그 위치를 열고, 해당 위치를 시작으로 인접한 위치를 탐색하며 빈 공간을 모두 열어주는 역할을 합니다.
//이를 통해 게임 플레이어는 빈 공간에 대한 정보를 얻을 수 있고, 게임을 진행할 수 있습니다.
//
//* 왜 재귀적으로 이 함수에 들어가야 하는가 ?
//→ 현재 위치를 중심으로 8방향에 대해 재귀적으로 탐색해 지뢰를 찾아내는 것이 목적, 재귀를 사용하지 않으면 반복문을 사용해야함.하지만 이는 비효율적이고 가독성이 떨어지므로 재귀문 작성



//##지뢰를 찾은 후에 게임이 끝났는지 판단하는 함수#

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

	//→for 문을 활용해 ‘gameMap’ 배열에서 벽이 아니고 아직 노출되지 않은 지점의 개수를  stripCnt에 더한다.
	//즉, 아직 노출되지 않은 셀의 개수를 세는 역할을 함,
	//게임에서 노출되지 않은 셀은 지뢰가 있을 수도 있고 없을 수도 있음.따라서 이 부분에서 지뢰를 찾은 후에 아직 노출되지 않은 셀이 얼마나 남았는지를 확인함.
	//
	//for문을 사용해 i와j의 모든 조합에 대해  gameMap[i][j]의 wall 값이 1이 아니고(벽이 아니고) strip값이 0인(노출되지 않은 지점)셀이 있다면 stripCnt를 1증가 시킴.
	//이후 if 문에서  stripCnt와  tmpCnt를 비교해 게임 종료 여부 결정

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

//→ if 문을 활용해 찾아야 할 지뢰의 개수인 retMineCnt가 0이고(지뢰를 다 찾았거나), 아직 노출되지 않은 지점의 개수를 stripCnt가 임시적으로 찾아야 할 지뢰의 개수 tmpCnt와 같다면(노출되지 않은 지점에 모두 지뢰가 있다면),
//displayMap함수를 호출 → 게임 맵 출력
//메세지 "Good...!!  "을 입력
//return 1로 함수를 종료
//조건문이 성립하지 않는 경우 return 0으로 함수를 종료


//##사용자가 게임 난이도를 선택하는 함수##.
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
			//초급을 선택할 경우 지뢰의 개수를 10개로 초기화
			mineCntSet_u = 10;
			//max값 조정에 따라 gameLevel값 수정
			return 42;
		}
		else if (select == 2)
		{
			//중급을 선택할 경우 지뢰의 개수를 30개로 초기화
			mineCntSet_u = 30;
			//max값 조장에 따라 gameLevel값 수정
			return 37;
		}
		else if (select == 3)
		{
			//고급을 선택할 경우 지뢰의 개수를 80개로 초기화
			mineCntSet_u = 80;
			//max값 조정에 따라 gameLevel값 수정
			return 32;
		}
		else					continue;
	}
}


//##지뢰를 무작위로 배치하는 함수##
int randx(int x, int y)
{
	static int flag = 0; //정적 변수로 flag를 선언
	int retVal;
	if (flag == 0)
	{
		srand(time(NULL));
		rand(); rand(); rand(); rand();
		srand(rand());
		flag = 1;
	}

	//→ if 문을 통해 flag변수가 0인 경우에만 랜덤 시드로 초기화 하고 1로 값을 변경함. 
		//랜덤 시드란, 난수를 생성할 때 사용하는 시작숫자를 의미.
			//랜덤 시드 초기화란, 컴퓨터 내부에서 어떤 계산을 수행한 후 랜덤 시드 값을 기반으로 난수를 생성
			//-> 무작위성을 높임
	//*시드는 난수를 생성하기 위한 초기값으로 일반적으로 컴퓨터의 현재 시각에서 초단위로 얻어와 사용하는 것에 대한 부분
		//하지만, rand()를 사용하면 시드값이 같아질 수도 있어 rand()를 여러번 호출 후 srand()함수로 사용
	retVal = rand() % (y - x + 1) + x;
	return retVal;
}

//##현재 커서 위치를 설정하기 위한 함수##

void gotoxy(int x, int y)
{
	COORD cd; //COORD라는 구조체 변수 cd 선언
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
// 이 함수는 콘솔 화면에서 위치를 조정하는데 사용, 콘솔에서 텍스트 기반 UI를 구현하는데 유용

//##콘솔 출력의 색 변경 함수##
void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}