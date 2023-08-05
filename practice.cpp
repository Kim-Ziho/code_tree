#include <iostream>
#include <set>

using namespace std;

template<typename T>
void print(set<T> & s) {
    for (auto iter = s.begin(); iter != s.end(); iter++) {
        cout << *iter << ' ';
    }
    cout << endl;
}

int main() {

    set<int> s;
    s.insert(4);
    s.insert(3);
    s.insert(6);
    s.insert(1);
    print(s);



    return 0;
}