#include "mrt.hpp"

// don't rename or delete the entry function
void kernel() {
    mrt::clear();

    const char* hello = "hello";
    int x = 0;
    while (*hello) {
        mrt::write(*hello++, x++, 0);
    }
}
