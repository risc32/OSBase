#pragma once

#include "../include/all.hpp"

// don't rename or delete the entry function
void kernel() {
    const char* hello = "hello";
    int x = 0;
    while (*hello) {
        write(*hello++, x++, 0);
    }
}
