@echo off
mkdir build
mkdir prep

start /b gcc -std=c++23 src/kernel.cpp -w -o prep/prep.cpp -E -fpermissive -Iinclude

i686-elf-g++ -m32 -std=c++23 -ffreestanding -nostdlib -nostdlib++ -fno-rtti -fno-builtin -fno-exceptions -fno-pic -fno-pie -fno-use-cxa-atexit -O0 -Iinclude -c src/kernel.cpp -o build/kernel.o
if errorlevel 1 exit /b 1

i686-elf-g++ -m32 -std=c++23 -ffreestanding -nostdlib -nostdlib++ -fno-rtti -fno-builtin -fno-exceptions -fno-pic -fno-pie -fno-use-cxa-atexit -O0 -Iinclude -c lowlevel/boot/middle.cpp -o build/middle.o
if errorlevel 1 exit /b 1

i686-elf-g++ -m32 -nostdlib -nostartfiles -ffreestanding -static -fno-builtin -Ttext 0x200000 build/kernel.o -o build/kernel.elf -lgcc
if errorlevel 1 exit /b 1

i686-elf-g++ -m32 -nostdlib -nostartfiles -ffreestanding -static -fno-builtin -Ttext 0x10000 build/middle.o -o build/middle.elf -lgcc
if errorlevel 1 exit /b 1

i686-elf-objcopy -O binary build/kernel.elf build/kernel.bin
if errorlevel 1 exit /b 1

i686-elf-objcopy -O binary build/middle.elf build/middle.bin
if errorlevel 1 exit /b 1

fasm lowlevel/boot/boot.asm build/boot.bin
if errorlevel 1 exit /b 1

dd if=/dev/zero of=os.img bs=512 count=32768
if errorlevel 1 exit /b 1

dd if=build/boot.bin of=os.img bs=512 count=1 conv=notrunc
if errorlevel 1 exit /b 1

dd if=build/middle.bin of=os.img bs=512 seek=1 conv=notrunc
if errorlevel 1 exit /b 1

dd if=build/kernel.bin of=os.img bs=512 seek=6 conv=notrunc
if errorlevel 1 exit /b 1

qemu-system-i386 -drive format=raw,file=os.img -display gtk,show-tabs=on -monitor stdio -m 8G -d int,cpu_reset -cpu max -s
