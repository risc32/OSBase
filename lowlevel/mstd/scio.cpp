#pragma once

void write(unsigned char c, int x, int y) {
    reinterpret_cast<short *>(0xb8000)[x + (y * 80)] = static_cast<short>(c) | 0x07 << 8;
}
