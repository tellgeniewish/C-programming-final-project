#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h> // Beep()�Լ��� ����ϱ� ���� ��� ����
//Beep(����, ���ð�)
#include <string.h> // strcmp�� strcpy ����ϱ� ���� ��� ����
#include <stdio.h>
#define BOARD_SIZE 10
void display(char b[][BOARD_SIZE]) {
    int i, j;
    printf("     ");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // �ܼ� �ڵ��� ������
    SetConsoleTextAttribute(hConsole, 14); // �ٵ��ǰ� ����� ���� ��� �������� ����
    for (i = 0; i < BOARD_SIZE; i++)
        printf("%2d", i);
    printf("\n     ---------------------\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%3d |", i);
        SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����

        for (j = 0; j < BOARD_SIZE; j++) {
            if (b[i][j] == 'X')
                printf("��"); // �鵹 ���� �����ϴ� ���� ��Ģ
            else if (b[i][j] == 'O') {
                printf("��");
            }
            else
                printf(" %c", b[i][j]);
            //printf(" ");
        //printf(" %c", b[i][j]);
        }
        SetConsoleTextAttribute(hConsole, 14); // �ٵ��ǰ� ����� ���� ��� �������� ����
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
}
int winCheck(char b[][BOARD_SIZE], int r, int c) {
    int i, j;
    int HorizonCount = 0, VerticalCount = 0, DiagonalCount = 0, ConverseDiagonalCount = 0;
    char omok = b[r][c];

    // ����
    for (i = 0; i < BOARD_SIZE; i++) {
        if (b[r][i] == omok) {
            HorizonCount++;

            if (HorizonCount == 5 && b[r][i + 1] != omok) // ���޾� 5���� ���ÿ� �� ���� ĭ�� omok�� �޶�� �Ѵ�
                return 1; // ���� 1
        }
        else {
            HorizonCount = 0;
        }
    }

    // ����
    for (i = 0; i < BOARD_SIZE; i++) {
        if (b[i][c] == omok) {
            VerticalCount++;

            if (VerticalCount == 5 && b[i + 1][c] != omok) // ���޾� 5���� ���ÿ� �� ���� ĭ�� omok�� �޶�� �Ѵ�
                return 1; // ���� 1
        }
        else {
            VerticalCount = 0;
        }
    }

    // �밢��
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            while (b[i][j] == omok) {
                DiagonalCount++;

                if (DiagonalCount == 5 && b[i + 1][j + 1] != omok) // ���޾� 5���� ���ÿ� �� ���� ĭ�� omok�� �޶�� �Ѵ�
                    return 1; // ���� 1

                i++;
                j++;
            }
            DiagonalCount = 0;
        }
    }

    // ���밢��
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            while (b[i][j] == omok) {
                ConverseDiagonalCount++;

                if (ConverseDiagonalCount == 5 && b[i + 1][j - 1] != omok) // ���޾� 5���� ���ÿ� �� ���� ĭ�� omok�� �޶�� �Ѵ�
                    return 1; // ���� 1

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
    char turn[8]; // ���ڿ��� ũ�⸦ 8�� ����
    int r, c;
    int i, j;
    int count;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            board[i][j] = ' ';
    
    char answer;
    printf("������ �����Ͻðڽ��ϱ�?(Y / N): "); // ���� ���� ����
    answer = getchar();
    if (answer != 'Y') // Y�� �ƴϸ� �״�� ����
        return 0;

    printf("\n���� ������ �����ϰڽ��ϴ�!\n\n");

    // ����� �г��� �Է¹ޱ�
    char player1[8], player2[8];
    do {
        printf("player1�� �г����� 7�� �̳��� �Է����ּ���: ");
        scanf("%s", player1);
    } while (7 < strlen(player1)); // ������ ũ�⸦ ����� ����� �Է¹��� ������ ��ӵȴ�

    strcpy(turn, player1); // ù��° ����ڸ� turn�� ����

    do {
        printf("player2�� �г����� 7�� �̳��� �Է����ּ���: ");
        scanf("%s", player2);
    } while (7 < strlen(player2));

    char omokColor = 'X'; 

    count = 1;
    display(board);

    do {
        do {
            printf("Player %s(�� ��): ", turn); // %c���� %s�� ����
            while (scanf("%d %d", &r, &c) != 2) { // �� ���� ������ �Է� �޴� ���� �߸��� �Է��� ���� ���
                while (getchar() != '\n'); // ���� ����
            }

            if (board[r][c] != ' ') { // �������� �̹� ���� ��
                // �Է� �߸��Ǿ���
                Beep(340, 1000);
                Beep(580, 1000);
            }
            else if (board[r][c] == ' ' && omokColor == 'X') // X�� ���� ����� ��
                Beep(260, 500); // ��
            else // if (board[r][c] == ' ' && omokColor == 'O')   // O�� ���� ����� ��
                Beep(290, 500); // ��
        } while (board[r][c] != ' ');

        board[r][c] = omokColor;
        display(board);

        if (winCheck(board, r, c) == 1) {
            printf("\nPlayer "); // %c���� %s�� ����
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // �ܼ� �ڵ��� ������
            SetConsoleTextAttribute(hConsole, 12); // ��ȫ �������� ����
            printf("%s wins", turn); // %c���� %s�� ����
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("!\n");

            Beep(260, 500); // ��
            Beep(330, 500); // ��
            Beep(380, 500); // ��
            Beep(520, 700); // ���� ��

            SetConsoleTextAttribute(hConsole, 9); // �ϴ� �������� ����
            printf("\n  ��");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("     $$    $$     \n");
            printf("   �� $$    $$    $$    ");
            SetConsoleTextAttribute(hConsole, 9); // �ϴ� �������� ����
            printf("��\n");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("    $    -     -    $$   \n");
            printf("   $  // e     e  // $$  \n");
            printf("    $       u      $$        ");
            SetConsoleTextAttribute(hConsole, 13); // ��ȫ �������� ����
            printf(" $$    $    $$  $$   $$    $   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("    $$            $$  ��       ");
            SetConsoleTextAttribute(hConsole, 13); // ��ȫ �������� ����
            printf("$$  $ $  $$   $$   $ $$  $   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("   $  &            $$  ");
            SetConsoleTextAttribute(hConsole, 9); // �ϴ� �������� ����
            printf("��       ");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            SetConsoleTextAttribute(hConsole, 13); // ��ȫ �������� ����
            printf("$$    $$     $$   $    $$   $$\n");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("    $ &&          $ $$ \n");
            printf("     $$           $$                                       ");
            SetConsoleTextAttribute(hConsole, 13); // ��ȫ �������� ����
            printf(" $$\n");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            SetConsoleTextAttribute(hConsole, 9); // �ϴ� �������� ����
            printf("  ��");
            SetConsoleTextAttribute(hConsole, 15); // �⺻ �������� ����
            printf("    $$$$$$$$$$    \n");
            printf("         $$   $$  \n");

            break;
        }

        if (omokColor == 'X')
            omokColor = 'O';
        else
            omokColor = 'X';

        strcpy(turn, strcmp(turn, player1) == 0 ? player2 : player1); // ���׿����� �ȿ��� strcmp�� ���Ͽ� turn�� ���ϰ� strcpy�� �����Ͽ� �ٲ��ش�

        count++;
    } while (count <= BOARD_SIZE * BOARD_SIZE);

    if (winCheck(board, r, c) == 0 && count >= BOARD_SIZE * BOARD_SIZE + 1) {
        printf("Nobody wins!\n");
        //��� �� ȿ����
        Beep(520, 500); //���� �� 
        Beep(380, 500); //��
        Beep(330, 500); //��
        Beep(260, 700); //��
    }

    return 0;
}