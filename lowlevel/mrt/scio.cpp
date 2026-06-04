#pragma once

#include "asm.cpp"

void write(unsigned char c, int x, int y) {
    reinterpret_cast<short *>(0xb8000)[x + (y * 80)] = static_cast<short>(c) | 0x07 << 8;
}

constexpr char _norm_scd[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

constexpr char _shift_scd[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

bool _lshift_p;
bool _rshift_p;

uint8_t getsc() {
    while (!(inb(0x64) & 1)) {}
    return inb(0x60);
}

bool shift_pressed() {
    return _lshift_p || _rshift_p;
}

char getchar() {
    uint8_t sc;
    do {
        handler:
        switch (sc = getsc()) {
            case 0x2A: _lshift_p = true;  goto handler;
            case 0x36: _rshift_p = true;  goto handler;
            case 0xAA: _lshift_p = false; goto handler;
            case 0xB6: _rshift_p = false; goto handler;
        }
    } while (sc & 0x80);

    return shift_pressed() ? _shift_scd[sc] : _norm_scd[sc];
}

void clear() {
    for (int i = 0; i < 25*80; ++i) {
        write(0, i, 0);
    }
}

bool is_newline(char c) {
    return c == '\n';
}
