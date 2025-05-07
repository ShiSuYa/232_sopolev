#include "lib.h"
#include <stdio.h>

void __wrap_fred(int x) {
    printf("interposed fred\n");
}

void __wrap_john(double y) {
    printf("interposed john\n");
}

int main() {
    bill("hello");
    fred(42);
    john(3.14);
    sam(3.1415);
    return 0;
}