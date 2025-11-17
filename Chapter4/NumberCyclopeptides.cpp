#include <iostream>

using namespace std;

int main() {
    int n = 39560;
    int count = 1;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            count++;
        }
    }
    cout << count << endl;
}