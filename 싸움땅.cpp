#include <iostream>
#include <queue>

using namespace std;

struct Man {
    int y;
    int x;
    int d;
    int s;
    int gun;
    int point;
};

int N, M, K;
Man men[31];
int direct_y[4] = {-1, 0, 1, 0};
int direct_x[4] = {0, 1, 0, -1};
priority_queue<int> gun_map[21][21];
int map[21][21];

void answer();
void run();
void print();
void init();
void escape(int);
void pick_gun(int);
void fight(int, int, int&, int&);
void move(int); // 순차적, 한칸, 정반대방향, 총 or 플레이, 총들가운데쎈총,
// 싸움, 초기능력치+총, 초기우선, 총공격력의차이득점,
// 지면->총을두고, 원래방향이동(오른쪽90도)(다른플레이어,범위밖), 총들가운데쎈총
// 이기면->총들가운데쎈총

void answer() {
    for (int i = 1; i <= M; i++) {
        cout << men[i].point << ' ';
    }
}

void print() {
    for (int y = 1; y <= N; y++) {
        for (int x = 1; x <= N; x++) {
            if (gun_map[y][x].empty()) printf("%6d/%d", 0, map[y][x]);
            else printf("%6d/%d", gun_map[y][x].top(), map[y][x]);
        }
        printf("\n");
    }
    printf("----------------------------------------------------\n");
}

void move(int a) {
    int ny = men[a].y + direct_y[men[a].d];
    int nx = men[a].x + direct_x[men[a].d];
    if (ny < 1 || nx < 1 || ny > N || nx > N) {
        ny -= 2 * direct_y[men[a].d];
        nx -= 2 * direct_x[men[a].d];
        if (men[a].d == 0) men[a].d = 2;
        else if (men[a].d == 1) men[a].d = 3;
        else if (men[a].d == 2) men[a].d = 0;
        else if (men[a].d == 3) men[a].d = 1;
    }
    if (map[ny][nx] != 0) {
        map[men[a].y][men[a].x] = 0;
    }
    else {
        map[men[a].y][men[a].x] = 0;
        map[ny][nx] = a;
    }
    men[a].y = ny;
    men[a].x = nx;
}

void pick_gun(int a) {
    if (gun_map[men[a].y][men[a].x].empty()) return;
    if (men[a].gun >= gun_map[men[a].y][men[a].x].top()) return;
    if (men[a].gun != 0) gun_map[men[a].y][men[a].x].push(men[a].gun);
    men[a].gun = gun_map[men[a].y][men[a].x].top();
    gun_map[men[a].y][men[a].x].pop();
}

void fight(int a, int b, int& winner, int& looser) {
    int score = (men[a].s + men[a].gun) - (men[b].s + men[b].gun);
    if (score > 0) {winner = a; looser = b;}
    else if (score == 0) {
        if (men[a].s > men[b].s) {winner = a; looser = b;}
        else {winner = b; looser = a;}
    }
    else {winner = b; looser = a; score *= -1;}
    men[winner].point += score;
    map[men[winner].y][men[winner].x] = winner;
}

void escape(int a) {
    if (men[a].gun != 0) gun_map[men[a].y][men[a].x].push(men[a].gun);
    men[a].gun = 0;
    int ny = men[a].y + direct_y[men[a].d];
    int nx = men[a].x + direct_x[men[a].d];
    while (ny < 1 || nx < 1 || ny > N || nx > N || map[ny][nx] != 0) {
        if (men[a].d == 3) men[a].d = 0;
        else men[a].d += 1;
        ny = men[a].y + direct_y[men[a].d];
        nx = men[a].x + direct_x[men[a].d];
    }
    men[a].y = ny;
    men[a].x = nx;
    pick_gun(a);
    map[men[a].y][men[a].x] = a;
}

void init() {
    for (int y = 0; y < 21; y++) {
        for (int x = 0; x < 21; x++) {
            map[y][x] = 0;
            while(!gun_map[y][x].empty()) {
                gun_map[y][x].pop();
            }
        }
    }
    for (int i = 0; i < 31; i++) {
        men[i] = {0, 0};
    }
    cin >> N >> M >> K;
    for (int y = 1; y <= N; y++) {
        for (int x = 1; x <= N; x++) {
            int temp;
            cin >> temp;
            if (temp != 0) gun_map[y][x].push(temp);
        }
    }
    for (int i = 1; i <= M; i++) {
        cin >> men[i].y >> men[i].x >> men[i].d >> men[i].s;
        men[i].gun = 0;
        men[i].point = 0;
        map[men[i].y][men[i].x] = i;
    }
}

void run() {
    for (int i = 0; i < K; i++) {
        for (int j = 1; j <= M; j++) {
            print();
            int winner = -1;
            int looser = -1;
            move(j);
            print();
            if (map[men[j].y][men[j].x] != j) {
                fight(j, map[men[j].y][men[j].x], winner, looser);
                escape(looser);
                pick_gun(winner);
            } else pick_gun(j);
        }
    }
}

int main() {
    freopen("싸움땅.txt", "r", stdin);
    init();
    run();
    answer();
    return 0;
}