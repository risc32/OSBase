#pragma once

void kernel();
extern "C" void _start() {
    kernel();
    while (1) {
        asm volatile("pause");
    }
}
namespace mrt {
#include "../lowlevel/mrt/basic.cpp"
#include "../lowlevel/mrt/asm.cpp"
#include "../lowlevel/mrt/disk.cpp"
#include "../lowlevel/mrt/scio.cpp"
}
