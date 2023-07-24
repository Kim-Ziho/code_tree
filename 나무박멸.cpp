#include <iostream>

using namespace std;

void init();
void run();
void print();
void growing();
void breeding();
void killing();
void planting();
void next_year();

int N, M, K, C;
int map[21][21];
int planted[21][21];
int kill_tree;
int direct_y[4] = {-1 , 0, 1, 0};
int direct_x[4] = {0, 1, 0, -1};
int diagonal_y[4] = {-1, -1, 1, 1};
int diagonal_x[4] = {-1, 1, 1, -1};

void print() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            printf("%7d", map[y][x]);
        }
        printf("\n");
    }

    printf("%d-----------------------------------\n", kill_tree);
}

void init() {
    cin >> N >> M >> K >> C;
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            int tmp;
            cin >> tmp;
            if (tmp == -1) map[y][x] = -9999;
            else {
                map[y][x] = tmp;
                if (tmp > 0) planted[y][x] = 1;
            }
        }
    }
    kill_tree = 0;
}

void growing() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (map[y][x] <= 0) continue;
            int grow = 0;
            for (int i = 0; i < 4; i++) {
                int sy = y + direct_y[i];
                int sx = x + direct_x[i];
                if (sy < 0 || sy >= N || sx < 0 || sx >= N) continue;
                if (map[sy][sx] > 0) grow++;
            }
            map[y][x] += grow;
        }
    }
    //printf("growing\n");
    //print();
}

void breeding() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (!planted[y][x]) continue;
            int empty_space = 0;
            for (int i = 0; i < 4; i++) {
                int sy = y + direct_y[i];
                int sx = x + direct_x[i];
                if (sy < 0 || sy >= N || sx < 0 || sx >= N) continue;
                if (map[sy][sx] < 0) continue;
                if (planted[sy][sx] == 0) empty_space++;
            }
            if (empty_space == 0) continue;
            int breed = map[y][x] / empty_space;
            for (int i = 0; i < 4; i++) {
                int sy = y + direct_y[i];
                int sx = x + direct_x[i];
                if (sy < 0 || sy >= N || sx < 0 || sx >= N) continue;
                if (map[sy][sx] < 0) continue;
                if (planted[sy][sx]) continue;
                map[sy][sx] += breed;
            }
        }
    }
    planting();
    //printf("bleeding\n");
    //print();
}

void killing() {
    int max_y = -1, max_x = -1, maxi = -1;
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            int good[4] = {1, 1, 1, 1};
            if (planted[y][x] == 0) continue;
            int cnt_kill = map[y][x];
            for (int len = 1; len <= K; len++) {
                for (int i = 0; i < 4; i++) {
                    if (good[i] == 0) continue;
                    int sy = y + len * diagonal_y[i];
                    int sx = x + len * diagonal_x[i];
                    if (sy < 0 || sy >= N || sx < 0 || sx >= N) continue;
                    if (map[sy][sx] == -9999 || map[sy][sx] <= 0) {
                        good[i] = 0;
                        continue;
                    }
                    cnt_kill += map[sy][sx];
                }
            }
            if (cnt_kill > maxi) {
                maxi = cnt_kill;
                max_y = y;
                max_x = x;
            }
        }
    }
    int okay[4] = {1, 1, 1, 1};
    map[max_y][max_x] = -1 * C;
    for (int len = 1; len <= K; len++) {
        for (int i = 0; i < 4; i++) {
            if (okay[i] == 0) continue;
            int sy = max_y + len * diagonal_y[i];
            int sx = max_x + len * diagonal_x[i];
            if (sy < 0 || sy >= N || sx < 0 || sx >= N) continue;
            if (map[sy][sx] == -9999) {
                okay[i] = 0;
                continue;
            }
            if (map[sy][sx] <= 0) okay[i] = 0;
            map[sy][sx] = -1 * C;
        }
    }
    kill_tree += maxi;
    planting();
    //printf("killing\n");
    //print();
}

void planting() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (map[y][x] > 0) planted[y][x] = 1;
            else planted[y][x] = 0;
        }
    }
}

void next_year() {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (map[y][x] == -9999) continue;
            if (map[y][x] < 0) map[y][x]++;
        }
    }
}

void run() {
    for (int i = 0; i < M; i++) {
        int left_tree = 0;
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                if (map[y][x] > 0) left_tree++;
            }
        }
        if (left_tree == 0) return;
        growing();
        breeding();
        next_year();
        killing();
        left_tree = 0;
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                if (map[y][x] > 0) left_tree++;
            }
        }
        if (left_tree == 0) return;
    }
}

int main() {
    //freopen("input.txt", "r", stdin);
    init();
    //print();
    run();
    cout << kill_tree << endl;
    return 0;
}