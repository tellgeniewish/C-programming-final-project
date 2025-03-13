#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h> // Beep()함수를 사용하기 위해 헤더 정의
//Beep(음파, 대기시간)
#include <string.h> // strcmp와 strcpy 사용하기 위해 헤더 정의
#include <stdio.h>
#define BOARD_SIZE 10
void display(char b[][BOARD_SIZE]) {
    int i, j;
    printf("     ");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들을 가져옴
    SetConsoleTextAttribute(hConsole, 14); // 바둑판과 비슷한 밝은 노랑 색상으로 설정
    for (i = 0; i < BOARD_SIZE; i++)
        printf("%2d", i);
    printf("\n     ---------------------\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%3d |", i);
        SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정

        for (j = 0; j < BOARD_SIZE; j++) {
            if (b[i][j] == 'X')
                printf("●"); // 백돌 먼저 시작하는 것이 원칙
            else if (b[i][j] == 'O') {
                printf("○");
            }
            else
                printf(" %c", b[i][j]);
            //printf(" ");
        //printf(" %c", b[i][j]);
        }
        SetConsoleTextAttribute(hConsole, 14); // 바둑판과 비슷한 밝은 노랑 색상으로 설정
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
}
int winCheck(char b[][BOARD_SIZE], int r, int c) {
    int i, j;
    int HorizonCount = 0, VerticalCount = 0, DiagonalCount = 0, ConverseDiagonalCount = 0;
    char omok = b[r][c];

    // 수평
    for (i = 0; i < BOARD_SIZE; i++) {
        if (b[r][i] == omok) {
            HorizonCount++;

            if (HorizonCount == 5 && b[r][i + 1] != omok) // 연달아 5개인 동시에 그 다음 칸은 omok과 달라야 한다
                return 1; // 리턴 1
        }
        else {
            HorizonCount = 0;
        }
    }

    // 수직
    for (i = 0; i < BOARD_SIZE; i++) {
        if (b[i][c] == omok) {
            VerticalCount++;

            if (VerticalCount == 5 && b[i + 1][c] != omok) // 연달아 5개인 동시에 그 다음 칸은 omok과 달라야 한다
                return 1; // 리턴 1
        }
        else {
            VerticalCount = 0;
        }
    }

    // 대각선
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            while (b[i][j] == omok) {
                DiagonalCount++;

                if (DiagonalCount == 5 && b[i + 1][j + 1] != omok) // 연달아 5개인 동시에 그 다음 칸은 omok과 달라야 한다
                    return 1; // 리턴 1

                i++;
                j++;
            }
            DiagonalCount = 0;
        }
    }

    // 역대각선
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            while (b[i][j] == omok) {
                ConverseDiagonalCount++;

                if (ConverseDiagonalCount == 5 && b[i + 1][j - 1] != omok) // 연달아 5개인 동시에 그 다음 칸은 omok과 달라야 한다
                    return 1; // 리턴 1

                i++;
                j--;
            }
            ConverseDiagonalCount = 0;
        }
    }

    if (HorizonCount == 5 || VerticalCount == 5 || DiagonalCount == 5 || ConverseDiagonalCount == 5)
        return 1;
    else
        return 0;
}
int main(void) {
    char board[BOARD_SIZE][BOARD_SIZE];
    char turn[8]; // 문자열의 크기를 8로 지정
    int r, c;
    int i, j;
    int count;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            board[i][j] = ' ';
    
    char answer;
    printf("게임을 실행하시겠습니까?(Y / N): "); // 게임 시작 묻기
    answer = getchar();
    if (answer != 'Y') // Y가 아니면 그대로 종료
        return 0;

    printf("\n오목 게임을 시작하겠습니다!\n\n");

    // 사용자 닉네임 입력받기
    char player1[8], player2[8];
    do {
        printf("player1의 닉네임을 7자 이내로 입력해주세요: ");
        scanf("%s", player1);
    } while (7 < strlen(player1)); // 정해진 크기를 벗어나면 제대로 입력받을 때까지 계속된다

    strcpy(turn, player1); // 첫번째 사용자를 turn에 복사

    do {
        printf("player2의 닉네임을 7자 이내로 입력해주세요: ");
        scanf("%s", player2);
    } while (7 < strlen(player2));

    char omokColor = 'X'; 

    count = 1;
    display(board);

    do {
        do {
            printf("Player %s(행 열): ", turn); // %c에서 %s로 수정
            while (scanf("%d %d", &r, &c) != 2) { // 두 개의 정수를 입력 받는 동안 잘못된 입력이 들어온 경우
                while (getchar() != '\n'); // 버퍼 비우기
            }

            if (board[r][c] != ' ') { // 누군가가 이미 돌을 둠
                // 입력 잘못되었음
                Beep(340, 1000);
                Beep(580, 1000);
            }
            else if (board[r][c] == ' ' && omokColor == 'X') // X가 돌을 제대로 둠
                Beep(260, 500); // 도
            else // if (board[r][c] == ' ' && omokColor == 'O')   // O가 돌을 제대로 둠
                Beep(290, 500); // 레
        } while (board[r][c] != ' ');

        board[r][c] = omokColor;
        display(board);

        if (winCheck(board, r, c) == 1) {
            printf("\nPlayer "); // %c에서 %s로 수정
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들을 가져옴
            SetConsoleTextAttribute(hConsole, 12); // 분홍 색상으로 설정
            printf("%s wins", turn); // %c에서 %s로 수정
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("!\n");

            Beep(260, 500); // 도
            Beep(330, 500); // 미
            Beep(380, 500); // 솔
            Beep(520, 700); // 높은 도

            SetConsoleTextAttribute(hConsole, 9); // 하늘 색상으로 설정
            printf("\n  ★");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("     $$    $$     \n");
            printf("   ☆ $$    $$    $$    ");
            SetConsoleTextAttribute(hConsole, 9); // 하늘 색상으로 설정
            printf("★\n");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("    $    -     -    $$   \n");
            printf("   $  // e     e  // $$  \n");
            printf("    $       u      $$        ");
            SetConsoleTextAttribute(hConsole, 13); // 분홍 색상으로 설정
            printf(" $$    $    $$  $$   $$    $   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("    $$            $$  ☆       ");
            SetConsoleTextAttribute(hConsole, 13); // 분홍 색상으로 설정
            printf("$$  $ $  $$   $$   $ $$  $   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("   $  &            $$  ");
            SetConsoleTextAttribute(hConsole, 9); // 하늘 색상으로 설정
            printf("★       ");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            SetConsoleTextAttribute(hConsole, 13); // 분홍 색상으로 설정
            printf("$$    $$     $$   $    $$   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("    $ &&          $ $$ \n");
            printf("     $$           $$                                       ");
            SetConsoleTextAttribute(hConsole, 13); // 분홍 색상으로 설정
            printf(" $$\n");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            SetConsoleTextAttribute(hConsole, 9); // 하늘 색상으로 설정
            printf("  ★");
            SetConsoleTextAttribute(hConsole, 15); // 기본 색상으로 설정
            printf("    $$$$$$$$$$    \n");
            printf("         $$   $$  \n");

            break;
        }

        if (omokColor == 'X')
            omokColor = 'O';
        else
            omokColor = 'X';

        strcpy(turn, strcmp(turn, player1) == 0 ? player2 : player1); // 삼항연산자 안에서 strcmp로 비교하여 turn을 정하고 strcpy로 복사하여 바꿔준다

        count++;
    } while (count <= BOARD_SIZE * BOARD_SIZE);

    if (winCheck(board, r, c) == 0 && count >= BOARD_SIZE * BOARD_SIZE + 1) {
        printf("Nobody wins!\n");
        //비길 시 효과음
        Beep(520, 500); //높은 도 
        Beep(380, 500); //솔
        Beep(330, 500); //미
        Beep(260, 700); //도
    }

    return 0;
}