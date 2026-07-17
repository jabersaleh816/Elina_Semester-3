#include "Stack.h"
#include <iostream>
#include <exception>
using namespace std;

int main() {
    try {
        Stack s(3);
        s.push(10);
        s.push(20);

        cout << "Top: " << s.top() << "\n";
        cout << "Size: " << s.size() << "\n";

        s.pop();
        cout << "Top after pop: " << s.top() << "\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
