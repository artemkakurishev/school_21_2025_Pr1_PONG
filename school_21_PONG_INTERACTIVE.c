
#include <ncurses.h>
#include <stdio.h>
 
void screenPainter(int rocket1_y, int rocket2_y, int ball_x, int ball_y, int score_player_1,
                   int score_player_2, int WIDTH_SCREEN, int HEIGHT_SCREEN) {
    for (int y = 0; y < HEIGHT_SCREEN; ++y) {
        for (int x = 0; x < WIDTH_SCREEN; ++x) {
            if ((y == 0 && x != 0 && x != WIDTH_SCREEN - 1) ||
                (y == HEIGHT_SCREEN - 1 && x != 0 && x != WIDTH_SCREEN - 1)) {
                printw("#");
            } else if (((x == 0 && y != 0 && y != HEIGHT_SCREEN - 1) ||
                        (x == WIDTH_SCREEN - 1 && y != 0 && y != HEIGHT_SCREEN - 1))) {
                printw("|");
            } else if (x == 1 && (y == rocket1_y || y == rocket1_y + 1 || y == rocket1_y - 1)) {
                printw("|");
            } else if (x == WIDTH_SCREEN - 2 &&
                       (y == rocket2_y || y == rocket2_y + 1 || y == rocket2_y - 1)) {
                printw("|");
            } else if (x == ball_x && y == ball_y) {
                printw("0");
            } else {
                printw(" ");
            }
        }
 
        printw("\n");
    }
}
 
int count_table(int score_player_1, int score_player_2) {
    int WIDTH_SCREEN = 80;
    int HEIGHT_SCREEN = 5;
    int mid = WIDTH_SCREEN / 2;
    int score_width = 2;
 
    for (int y = 0; y < HEIGHT_SCREEN; y++) {
        for (int x = 0; x < WIDTH_SCREEN; x++) {
            if (y == 0 || y == HEIGHT_SCREEN - 1) {
                printw("^");
            } else if (x == 0 || x == WIDTH_SCREEN - 1) {
                printw("*");
            } else if (x == mid) {
                printw("|");
            } else if (y == HEIGHT_SCREEN / 2) {
                if (x == mid / 2 - 1 - 4) {
                    printw("Player 1: %d", score_player_1);
                    x += score_width;
                } else if (x == mid + mid / 2 - 1 - 5) {
                    printw("Player 2: %d", score_player_2);
                    x += score_width;
                } else if (x > mid - 9 && x < mid) {
                } else if (x > mid + 30 && x < WIDTH_SCREEN) {
                } else {
                    printw(" ");
                }
            }
 
            else {
                printw(" ");
            }
        }
        printw("\n");
    }
}
 
// Обновление позиции мяча по Х
int get_new_ball_x(int ball_x, int ball_dx) { return ball_x + ball_dx; }
 
// Обновление позиции мяча по У
int get_new_ball_y(int ball_y, int ball_dy) { return ball_y + ball_dy; }
 
// Отскок от рокеток
int get_new_dx(int ball_x, int ball_dx, int ball_y, int rocket1_y, int rocket2_y) {
    // левая ракетка
    if (ball_x <= 2 && (ball_y == rocket1_y - 1 || ball_y == rocket1_y || ball_y == rocket1_y + 1)) {
        return 1;  // летит вправо
    }
 
    // правая ракетка
    if (ball_x >= 77 && (ball_y == rocket2_y - 1 || ball_y == rocket2_y || ball_y == rocket2_y + 1)) {
        return -1;  // летит влево
    }
 
    return ball_dx;
}
 
// Отскок от стен
int get_new_dy(int ball_y, int ball_dy) {
    if (ball_y <= 1 || ball_y >= 23) {
        return -ball_dy;
    }
    return ball_dy;
}
 
// Чек гол
int check_goal(int ball_x) {
    if (ball_x <= 0) {
        return 2;  // забил player2 => в main нужно обновить счет
    } else if (ball_x >= 79) {
        return 1;  // забил player1 => в main нужно обновить счет
    } else {
        return 0;  // никто не забил
    }
}
 
// Artemiy added INSTRUCTIONS
void print_instructions() {
    printw("WELCOME in Pong!\n");
    printw("  Player 1: 'A' (up), 'Z' (down), Space (skip)\n");
    printw("  Player 2: 'K' (up), 'M' (down), Space (skip)\n");
    printw("First to 21 points wins!\n");
    printw("==============================\n\n");
}
 
// Artemiy added each turn to play
 
int main(void) {
    int WIDTH_SCREEN = 80, HEIGHT_SCREEN = 25, rocket1_y = 12, rocket2_y = 12;
    int ball_x = WIDTH_SCREEN / 2, ball_y = HEIGHT_SCREEN / 2, ball_dx = 1, ball_dy = 1;
    int score_player_1 = 0, score_player_2 = 0;
 
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
 
    print_instructions();
 
    screenPainter(rocket1_y, rocket2_y, ball_x, ball_y, score_player_1, score_player_2, WIDTH_SCREEN,
                  HEIGHT_SCREEN);
 
    do {
        int inp = getch();
        if (score_player_1 == 21 || score_player_2 == 21) {
            if (score_player_1 == 21) {
                endwin();
                printf("Игрок №1 Победил");
                return 0;
            } else {
                endwin();
                printf("Игрок №2 Победил");
                return 0;
            }
        }
        if (inp == 'a' || inp == 'A') {
            if (rocket1_y > 2) {
                rocket1_y -= 1;
            }
        } else if (inp == 'z' || inp == 'Z') {
            if (rocket1_y < 22) {
                rocket1_y += 1;
            }
        }
        if (inp == 'k' || inp == 'K') {
            if (rocket2_y > 2) {
                rocket2_y -= 1;
            }
        } else if (inp == 'm' || inp == 'M') {
            if (rocket2_y < 22) {
                rocket2_y += 1;
            }
        }
        clear();
 
        ball_x = get_new_ball_x(ball_x, ball_dx);
        ball_y = get_new_ball_y(ball_y, ball_dy);
        ball_dx = get_new_dx(ball_x, ball_dx, ball_y, rocket1_y, rocket2_y);
        ball_dy = get_new_dy(ball_y, ball_dy);
        int goal = check_goal(ball_x);
 
        if (goal == 1) {
            score_player_1++;
            ball_x = WIDTH_SCREEN / 2;
            ball_y = HEIGHT_SCREEN / 2;
            ball_dx = 1;
            ball_dy = 1;
        } else if (goal == 2) {
            score_player_2++;
            ball_x = WIDTH_SCREEN / 2;
            ball_y = HEIGHT_SCREEN / 2;
            ball_dx = 1;
            ball_dy = 1;
        }
        count_table(score_player_1, score_player_2);
        screenPainter(rocket1_y, rocket2_y, ball_x, ball_y, score_player_1, score_player_2, WIDTH_SCREEN,
                      HEIGHT_SCREEN);
        refresh();
        napms(70);
    } while (1);
    endwin();
    return 0;
}