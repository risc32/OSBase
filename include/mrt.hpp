#pragma once

void kernel();
extern "C" void _start() {
    kernel();
    while (1) {
        asm volatile("pause");
    }
}
namespace mrt {
#include "../lowlevel/mstd/basic.cpp"
#include "../lowlevel/mstd/asm.cpp"
#include "../lowlevel/mstd/disk.cpp"
#include "../lowlevel/mstd/scio.cpp"
}
