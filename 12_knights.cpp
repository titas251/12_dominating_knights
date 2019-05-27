/*
Bandomos visas kombinacijas žirgų pirmame lentos ketvirtyje langeliuose pažymėtuose 0:
Trying all possible knights combinations in the first board quarter. (0 - empty cell, 1 - blocked cell)

1 1 1 1 | 0 0 0 0
1 0 0 0 | 0 0 0 0
1 0 0 0 | 0 0 0 0
1 0 0 0 | 0 0 0 0
—————————————————
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0

Tikrinama, ar užpildyti langeliai pažymėti pavyzdyje x ženklu:
Checking if all 'x' cells are attacked.

x x x x | 0 0 0 0
x x 0 0 | 0 0 0 0
x 0 0 0 | 0 0 0 0
x 0 0 x | 0 0 0 0
—————————————————
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0
0 0 0 0 | 0 0 0 0

Nes suradus vieną kampą galima jį nukopijuoti, pasukti 90 laipsnių ir pritaikyti kitiems.
If we find one quarter with 'x' like in the example above, we can rotate it 90 degrees and apply for other quarter.
*/

#include <iostream>
#include <iomanip>

using namespace std;
//langelio struktūra
struct Cell {
    int status = 0;              // 0 - tuščias, 1 - blokuojamas, 2 - pastatytas žirgas (0 - empty, 1 - blocked, 2 - occupied)
    int is_attacked = 0;         // 0 - neatakuojama, 1 - atakuojama (0 - not attacked, 1 - attacked)
    int count = 0;
};
//lentos struktūra
struct Board {
    Cell cell[8][8];
};

void place_knight(int x, int y, Board &board);
//funkcija padedanti žirgą ant lentos

void remove_knight(int x, int y, Board &board);
//funcija nuimanti žirgą nuo lentos

void define_board(Board &board);
//nustatoma lenta (nustatomi užblokuoti langeliai)

bool check_Quad(Board &board);
//funkcija patikrinanti ar ketvirtyje lentos visi reikiami langeliai atakuojami

bool check_cell(int x, int y, Board &board);
//fukcija patikrinanti ar langelis yra atakuojamas

void check_board(Board board);
//funkcija patikrinanti ar visi lentos langeliai atakuojami

void start_search(Board &board);
//funkcija pradedanti ieškoti tinkamų kombinacijų

void print_board(Board board);
//funkcija atspausdinanti lentą

void remove_knights(int &x, int &y, int &x1, int &y1, int &x2, int &y2, int &x3, int &y3, int &count, Board &b);
//funkcija ištrinanti žirgus reikiama tvarka, jei lentos ketvertyje reikalingi langeliai nėra atakuojami.

void fill_board(Board &board);
//funkcija pritaikanti 90 laipsnių pasukimą visai lentai iš pirmo ketvirčio

void put_all_knights(Board board, Board &board1);
//funkcija padedanti visus žirgus ant lentos

int main() {
    Board board;
    define_board(board);
    start_search(board);
    return 0;
}

void start_search(Board &board) {
    int zx1, zy1, zx2, zy2, zx3, zy3;           //žirgų koordinatės (knights coordinates)
    int knights_count = 0;                      //žirgų skaičius ketvirtyje (knights count in the quarter)

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (board.cell[x][y].status == 0) {
                place_knight(x, y, board);
                knights_count++;

                if (knights_count == 1) {
                    zx1 = x;
                    zy1 = y;
                } else if (knights_count == 2) {
                    zx2 = x;
                    zy2 = y;

                } else if (knights_count == 3) {
                    zx3 = x;
                    zy3 = y;
                }
                if (knights_count == 3 && check_Quad(board) == false) {
                    remove_knights(x, y, zx1, zy1, zx2, zy2, zx3, zy3, knights_count, board);
                } else if (check_Quad(board) == true) {
                    check_board(board);
                    remove_knights(x, y, zx1, zy1, zx2, zy2, zx3, zy3, knights_count, board);
                }
            }
        }
    }
}

void remove_knights(int &x, int &y, int &x1, int &y1, int &x2, int &y2, int &x3, int &y3, int &count, Board &b) {
    remove_knight(x3, y3, b);
    count--;
    if (x2 == 3 && y2 == 2 && x3 == 3 && y3 == 3) {
        remove_knight(x2, y2, b);
        remove_knight(x1, y1, b);
        count -= 2;
        x = x1;
        y = y1;
    } else if (x3 == 3 && y3 == 3) {
        remove_knight(x2, y2, b);
        count--;
        x = x2;
        y = y2;
    }
}

void define_board(Board &board) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (x == 0 || y == 0) {
                board.cell[x][y].status = 1;
            }
        }
    }
}

void check_board(Board board) {
    Board board1;
    fill_board(board);
    put_all_knights(board, board1);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board1.cell[i][j].is_attacked == 0) {
                return;
            }
        }
    }
    print_board(board1);
}

void fill_board(Board &board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board.cell[i][j + 4] = board.cell[4 - j - 1][i];
            board.cell[i + 4][j + 4] = board.cell[4 - i - 1][4 - j - 1];
            board.cell[i + 4][j] = board.cell[j][4 - i - 1];
        }
    }
}

void put_all_knights(Board board, Board &board1) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.cell[i][j].status == 2) {
                place_knight(i, j, board1);
            }
        }
    }
}

void print_board(Board board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.cell[i][j].status != 2) {
                board.cell[i][j].status = 0;
            }
            if (board.cell[i][j].status == 2){
              cout << " K";
            }
            else {
              cout << " " << board.cell[i][j].count;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void remove_knight(int x, int y, Board &board) {
    board.cell[x][y].status = 0;
    //board.cell[x][y].count--;

    if (check_cell(x, y, board) == true) {
        board.cell[x][y].is_attacked = 0;
    }
    if (y + 2 < 8 && x + 1 < 8 && check_cell(x + 1, y + 2, board) == true) {
        board.cell[x + 1][y + 2].is_attacked = 0;
        //board.cell[x + 1][y + 2].count--;
    }
    if (y + 2 < 8 && x - 1 >= 0 && check_cell(x - 1, y + 2, board) == true) {
        board.cell[x - 1][y + 2].is_attacked = 0;
        //board.cell[x - 1][y + 2].count--;
    }
    if (y + 1 < 8 && x + 2 < 8 && check_cell(x + 2, y + 1, board) == true) {
        board.cell[x + 2][y + 1].is_attacked = 0;
        //board.cell[x + 2][y + 1].count--;
    }
    if (y + 1 < 8 && x - 2 >= 0 && check_cell(x - 2, y + 1, board) == true) {
        board.cell[x - 2][y + 1].is_attacked = 0;
      //  board.cell[x - 2][y + 1].count--;
    }
    if (y - 2 >= 0 && x + 1 < 8 && check_cell(x + 1, y - 2, board) == true) {
        board.cell[x + 1][y - 2].is_attacked = 0;
        //board.cell[x + 1][y - 2].count--;
    }
    if (y - 2 >= 0 && x - 1 >= 0 && check_cell(x - 1, y - 2, board) == true) {
        board.cell[x - 1][y - 2].is_attacked = 0;
        //board.cell[x - 1][y - 2].count--;
    }
    if (y - 1 >= 0 && x - 2 >= 0 && check_cell(x - 2, y - 1, board) == true) {
        board.cell[x - 2][y - 1].is_attacked = 0;
        //board.cell[x - 2][y - 1].count--;
    }
    if (y - 1 >= 0 && x + 2 < 8 && check_cell(x + 2, y - 1, board) == true) {
        board.cell[x + 2][y - 1].is_attacked = 0;
        //board.cell[x + 2][y - 1].count--;
    }
}

bool check_cell(int x, int y, Board &board) {

    if (board.cell[x + 1][y + 2].status == 2 && y + 2 < 8 && x + 1 < 8 ||
        board.cell[x - 1][y + 2].status == 2 && y + 2 < 8 && x - 1 >= 0 ||
        board.cell[x + 2][y + 1].status == 2 && y + 1 < 8 && x + 2 < 8 ||
        board.cell[x - 2][y + 1].status == 2 && y + 1 < 8 && x - 2 >= 0 ||
        board.cell[x + 1][y - 2].status == 2 && y - 2 >= 0 && x + 1 < 8 ||
        board.cell[x - 1][y - 2].status == 2 && y + 1 < 8 && x - 2 >= 0 ||
        board.cell[x - 2][y - 1].status == 2 && y - 2 >= 0 && x - 1 >= 0 ||
        board.cell[x + 2][y - 1].status == 2 && y - 1 >= 0 && x + 2 < 8) {
        return false;
    }
    return true;

}

bool check_Quad(Board &board) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (board.cell[0][y].is_attacked == 0) {
                return false;
            }
        }
        if (board.cell[x][0].is_attacked == 0) {
            return false;
        }
    }
    if (board.cell[1][1].is_attacked == 0 || board.cell[3][3].is_attacked == 0) {
        return false;
    }
    return true;
}

void place_knight(int x, int y, Board &board) {
    board.cell[x][y].status = 2;
    board.cell[x][y].is_attacked = 1;


    if (y + 2 < 8 && x + 1 < 8) {
        board.cell[x + 1][y + 2].is_attacked = 1;
        board.cell[x + 1][y + 2].count++;
    }
    if (y + 2 < 8 && x - 1 >= 0) {
        board.cell[x - 1][y + 2].is_attacked = 1;
        board.cell[x - 1][y + 2].count++;
    }
    if (y + 1 < 8 && x + 2 < 8) {
        board.cell[x + 2][y + 1].is_attacked = 1;
        board.cell[x + 2][y + 1].count++;
    }
    if (y + 1 < 8 && x - 2 >= 0) {
        board.cell[x - 2][y + 1].is_attacked = 1;
        board.cell[x - 2][y + 1].count++;
    }
    if (y - 2 >= 0 && x + 1 < 8) {
        board.cell[x + 1][y - 2].is_attacked = 1;
        board.cell[x + 1][y - 2].count++;
    }
    if (y - 2 >= 0 && x - 1 >= 0) {
        board.cell[x - 1][y - 2].is_attacked = 1;
        board.cell[x - 1][y - 2].count++;
    }
    if (y - 1 >= 0 && x - 2 >= 0) {
        board.cell[x - 2][y - 1].is_attacked = 1;
        board.cell[x - 2][y - 1].count++;
    }
    if (y - 1 >= 0 && x + 2 < 8) {
        board.cell[x + 2][y - 1].is_attacked = 1;
        board.cell[x + 2][y - 1].count++;
    }
}
