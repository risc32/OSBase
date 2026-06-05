# OSBase

OSBase is a minimal foundation for writing your own operating system in C++.

The goal of OSBase is not to provide a ready-to-use kernel, scheduler, filesystem, memory manager, GUI, or standard library.

The goal is simple:

> Start writing your operating system immediately without fighting bootloaders, assembly boilerplate, linker scripts, and low-level initialization.

---

## Philosophy

OSBase provides only the primitives required to communicate with hardware and enter C++ code.

Everything else is your responsibility.

No scheduler.

No allocator.

No STL.

No containers.

No strings.

No panic framework.

No filesystem.

No architecture decisions imposed on you.

If you need a memory allocator, write one.

If you need a scheduler, design one.

If you need a standard library, build one.

OSBase gives you a foundation, not a finished operating system.

---

## MRT

OSBase includes MRT (Minimal Runtime).

MRT is not a standard library.

MRT is not a framework.

MRT exists only to hide unavoidable hardware and toolchain complexity.

MRT provides:

* basic C/C++ types
* memory primitives (`memcpy`, `memset`, `memcmp`)
* low-level assembly wrappers
* port I/O
* ATA disk access
* VGA text output
* keyboard input
* interrupt infrastructure (future)

MRT intentionally does not provide:

* STL
* containers
* algorithms
* allocators
* strings
* schedulers
* filesystems

---

Example:

```cpp
#include "mrt.hpp"

void write(const char* s) {
    static int x = 0, y = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '\n') {
            x = 0;
            ++y;
            continue;
        }
        mrt::write(s[i], x++, y);
        if (x == 80) {
            x = 0;
            ++y;
        }
    }
}

void kernel()
{
    mrt::clear();

    write("Hello, World!");
}
```

---

## Project Structure

```
mrt/
    hardware abstraction layer

lowlevel/
    architecture-specific code

src/
    your operating system

kernel.cpp
    entry point
```

---

# Requirements

Before building OSBase, install the required development tools.

## GCC Cross Compiler

OSBase is built using a cross-compiler targeting i686.

Recommended target:

```bash
i686-elf-gcc
i686-elf-g++
```

Guide:

https://wiki.osdev.org/GCC_Cross-Compiler

---

## QEMU

QEMU is used to run the operating system.

Download:

https://www.qemu.org/download/

Verify installation:

```bash
qemu-system-i386 --version
```

---

## DD

OSBase uses `dd` to write the generated image.

Linux:

```bash
sudo apt install coreutils
```

Most Linux distributions already include it.

Windows:

Recommended:

https://gnuwin32.sourceforge.net/packages/coreutils.htm

or use:

https://www.msys2.org/

Verify installation:

```bash
dd --version
```

---

# Quick Start

Fork this repository and clone your fork:

```bash
git clone https://github.com/<your-username>/OSBase.git
cd OSBase
```

Build & Run:

```bash
cmd /c comp.bat
```

Or manually:

```bash
qemu-system-i386 -drive format=raw,file=os.img
```

---

# Toolchain Summary

* i686-elf-gcc
* i686-elf-g++
* ld
* objcopy
* dd
* qemu-system-i386
* fasm

---

# First Kernel

Open:

```text
src/kernel.cpp
```

Build and run:

```bash
make run
```

You are now running your own operating system.


## Who Is This For?

OSBase is for people who want to build operating systems.

Not use them.

Not customize them.

Build them.

If you want a complete kernel, OSBase is probably not for you.

If you want to design your own architecture, memory model, scheduler, runtime, and system APIs, welcome.

---

## License

Do whatever you want.

Build something interesting.
