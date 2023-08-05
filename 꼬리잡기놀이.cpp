// 풀이시간 21:30 ~ 24:30 (3시간 소요)
#include <iostream>
using namespace std;

struct Coord {
    int y;
    int x;
};

struct Team {
    Coord head;
    Coord tail;
};

Team team[5];
int N, M, K;
int map[22][22];
int total_score;
int direct_y[4] = {0, -1, 0, 1};
int direct_x[4] = {1, 0, -1, 0};

void find_head(int, Coord, int&, Coord&, int(&)[22][22]);
void moving(); // 3찾아2로가고4남겨 1찾아4로가고2남겨 꼬리랑머리기억
void shooting(int); // 1~n n+1~2n 2n+1~3n 3n+1~4n / 4n나눠
void changing(); // 꼬리랑머리바꿔
void init();
void print();

void find_head(int lev, Coord now, int& ret, Coord& head, int (&used)[22][22]) {
    if (map[now.y][now.x] == 1) {
        ret = lev;
        head = {now.y, now.x};
        return;
    }
    for (int i = 0; i < 4; i++) {
        int ny = now.y + direct_y[i];
        int nx = now.x + direct_x[i];
        if (ny < 1 || nx < 1 || ny > N || nx > N) continue;
        if (map[ny][nx] == 0 || map[ny][nx] == 3 || map[ny][nx] == 4) continue;
        if (lev == 1 && map[ny][nx] == 1 && map[now.y][now.x] == 3) continue;
        if (used[ny][nx] == 1) continue;
        used[ny][nx] = 1;
        Coord next = {ny, nx};
        find_head(lev + 1, next, ret, head, used);
    }
}


void print() {
    for (int y = 1; y <= N; y++) {
        for (int x = 1; x <= N; x++) {
            cout << map[y][x] << ' ';
        }
        cout << endl;
    }
    cout << "---------------------------" << endl;
}

void init() {
    cin >> N >> M >> K;
    for (int y = 1; y <= N; y++) {
        for (int x = 1; x <= N; x++) {
            cin >> map[y][x];
        }
    }
    int idx = 0;
    int used[22][22] = {0};
    for (int y = 1; y <= N; y++) {
        for (int x = 1; x <= N; x++) {
            if (map[y][x] == 3) {
                team[idx].tail = {y, x};
                int ret = -1;
                Coord head = {-1, -1};
                find_head(1, {y, x}, ret, head, used);
                team[idx].head = head;
                idx++;
            }
        }
    }
}

void moving() {
    for (int i = 0; i < M; i++) {
        int ty = team[i].tail.y;
        int tx = team[i].tail.x;
        map[ty][tx] = 4;
        for (int j = 0; j < 4; j++) {
            int ny = ty + direct_y[j];
            int nx = tx + direct_x[j];
            if (ny < 1 || nx < 1 || ny > N || nx > N) continue;
            if (map[ny][nx] == 2) {
                map[ny][nx] = 3;
                team[i].tail.y = ny;
                team[i].tail.x = nx;
                break;
            }
        }
        int hy = team[i].head.y;
        int hx = team[i].head.x;
        map[hy][hx] = 2;
        for (int j = 0; j < 4; j++) {
            int ny = hy + direct_y[j];
            int nx = hx + direct_x[j];
            if (ny < 1 || nx < 1 || ny > N || nx > N) continue;
            if (map[ny][nx] == 4) {
                map[ny][nx] = 1;
                team[i].head.y = ny;
                team[i].head.x = nx;
                break;
            }
        }
    }
}

void shooting(int rnd) {
    Coord shoot = {-1, -1};
    int direct = -1;

    int round = rnd % (4 * N);
    if (round >= 1 && round <= N) {
        shoot = {round, 1};
        direct = 0;
    }
    else if (round >= N + 1 && round <= 2 * N) {
        shoot = {N, round - N};
        direct = 1;
    }
    else if (round >= 2 * N + 1 && round <= 3 * N) {
        shoot = {3 * N + 1 - round, N};
        direct = 2;
    }
    else if (round >= 3 * N + 1 && round <= 4 * N) {
        shoot = {1, 4 * N + 1 - round};
        direct = 3;
    }
    else if (round == 0) {
        shoot = {1, 1};
        direct = 3;
    }

    int used[22][22] = {0};
    Coord head = {-1, -1};
    int ret = -1;
    for (int i = 0; i < N; i++) {
        if (map[shoot.y][shoot.x] == 1 || map[shoot.y][shoot.x] == 2 || map[shoot.y][shoot.x] == 3) {
            used[shoot.y][shoot.x] = 1;
            find_head(1, shoot, ret, head, used);
            break;
        }
        else {
            shoot.y += direct_y[direct];
            shoot.x += direct_x[direct];
        }
    }
    if (ret == -1) return;
    for (int i = 0; i < 5; i++) {
        if (team[i].head.y == head.y && team[i].head.x == head.x) {
            Coord temp = team[i].tail;
            team[i].tail = team[i].head;
            team[i].head = temp;
            map[team[i].head.y][team[i].head.x] = 1;
            map[team[i].tail.y][team[i].tail.x] = 3;
            total_score += ret * ret;
            break;
        }
    }
}

int main() {
    freopen("꼬리잡기놀이.txt", "r", stdin);
    init();
    print();
    for (int round = 1; round <= K; round++) {
        cout << round << ' ' << total_score << endl;
        moving();
        print();
        if (round == 23)
            int bug = 23;
        shooting(round);
        print();
    }

    cout << total_score << endl;

    return 0;
}