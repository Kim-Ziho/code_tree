#include <iostream>

struct Coord {
    int r;
    int c;
};

///////////////////12      9     6     3   // 반시계 45도
int direct_y[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
int direct_x[9] = {0, 0, -1, -1, -1, 0, 1, 1, 1};
int direct_pac_y[4] = {-1, 0, 1, 0}; // 반시계 90도
int direct_pac_x[4] = {0, -1, 0, 1};
int M, T;
int map[5][5];
int dead_area[5][5];
int monster[5][5][9];
int egg[5][5][9];
Coord pacman;
Coord temp_path[3];
Coord path[3];
int max_kill;

void copying(); // 알
void moving(); // 한칸, 45도 반시계 회전
void moving_pacman(int, int, int, int); // 4방향 3칸, 상하좌우, 시체, 함께 있었던 몬스터X
void disappear(); // 시체 2턴 -3
void reborn(); // 알 부화 +1

void print();
void print_path();
void print_pacman();
void init();
void run();
int total_monster();

void print_path() {
    for (int i = 0; i < 3; i++) {
        std::cout << '(' << path[i].r << ',' << path[i].c << ')' << ' ';
    }
    std::cout << "pacman=(" << pacman.r << ',' << pacman.c << ')' << std::endl;

}

void print_pacman() {
    std::cout << "pacman=(" << pacman.r << ',' << pacman.c << ')' << std::endl;
}

void print() {
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            printf("%5d/%d", map[y][x], dead_area[y][x]);
        }
        printf("\n");
    }
    printf("----------------------\n");
}

int total_monster() {
    int answer = 0;
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            answer += map[y][x];
        }
    }
    return answer;
}

void init() {
    std::cin >> M >> T;
    std::cin >> pacman.r >> pacman.c;
    for (int i = 0; i < M; i++) {
        int r, c, d;
        std::cin >> r >> c >> d;
        monster[r][c][d]++;
        map[r][c]++;
    }
}

void copying() {
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            for (int d = 1; d <= 8; d++) {
                egg[y][x][d] = monster[y][x][d];
            }
        }
    }
}

void moving() {
    int temp[5][5][9] = {0};
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            if (map[y][x] <= 0) continue;
            for (int d = 1; d <= 8; d++) {
                if (monster[y][x][d] <= 0) continue;
                int dir = d;
                int flag = 1;
                do {
                    int ny = y + direct_y[dir];
                    int nx = x + direct_x[dir];
                    if (!(ny < 1 || nx < 1 || ny > 4 || nx > 4 || dead_area[ny][nx] < 0 || (ny == pacman.r && nx == pacman.c))){
                        temp[ny][nx][dir] += monster[y][x][d];
                        flag = 0;
                        break;
                    }
                    if (dir == 8) dir = 1;
                    else dir++;
                } while (dir != d);
                if (flag == 1) temp[y][x][d] += monster[y][x][d];
            }
        }
    }
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            int sum_monster = 0;
            for (int d = 1; d <= 8; d++) {
                monster[y][x][d] = temp[y][x][d];
                sum_monster += temp[y][x][d];
            }
            map[y][x] = sum_monster;
        }
    }
}

void moving_pacman(int y, int x, int lev, int sum) {
    if (lev == 3) {
        if (sum > max_kill) {
            max_kill = sum;
            for (int i = 0; i < 3; i++) {
                path[i] = temp_path[i];
            }
        }
        return;
    }
    for (int i = 0; i < 4; i++) {
        int ny = y + direct_pac_y[i];
        int nx = x + direct_pac_x[i];
        if (ny < 1 || nx < 1 || ny > 4 || nx > 4) continue;
        int flag = 0;
        for (int j = 0; j < 3; j++) {
            if (ny == temp_path[j].r && nx == temp_path[j].c) flag = 1;
        }
        int add;
        if (flag == 1) add = 0;
        else add = map[ny][nx];
        temp_path[lev] = {ny, nx};
        moving_pacman(ny, nx, lev + 1, sum + add);
        temp_path[lev] = {0, 0};
    }
}

void disappear() {
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            if (dead_area[y][x] < 0) dead_area[y][x]++;
        }
    }
    for (int i = 0; i < 3; i++) {
        int r = path[i].r;
        int c = path[i].c;
        if (map[r][c] > 0) {
            dead_area[r][c] = -2;
            map[r][c] = 0;
            for (int j = 1; j <= 8; j++) {
                monster[r][c][j] = 0;
            }
        }
    }
    pacman = {path[2].r, path[2].c};
}

void reborn() {
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            for (int d = 1; d <= 8; d++) {
                monster[y][x][d] += egg[y][x][d];
                egg[y][x][d] = 0;
            }
        }
    }
    for (int y = 1; y <= 4; y++) {
        for (int x = 1; x <= 4; x++) {
            int sum_monster = 0;
            for (int d = 1; d <= 8; d++) {
                sum_monster += monster[y][x][d];
            }
            map[y][x] = sum_monster;
        }
    }
}

void run() {
    copying();
    //print_pacman();
    //print();
    moving();
    //print();
    max_kill = -1;
    moving_pacman(pacman.r, pacman.c, 0, 0);
    disappear();
    //print();
    //print_path();
    reborn();
    //print();
}

int main() {
    //freopen("꼬리잡기놀이.txt", "r", stdin);
    init();
    for (int i = 0; i < T; i++) {
        run();
    }
    int bug = 1;
    std::cout << total_monster() << std::endl;
    return 0;
}