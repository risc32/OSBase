cmake_minimum_required(VERSION 4.1)
project(OSBase)

set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_C_COMPILER gcc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++23 -ffreestanding -nostdlib -fno-rtti -fno-exceptions -fno-pic -fno-pie -fno-use-cxa-atexit -O0 -Iinclude")
#set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})

add_executable(kernel.elf
        src/kernel.cpp
        lowlevel/boot/middle.cpp
        include/mrt.hpp
)

target_link_options(kernel.elf PRIVATE -m32 -Iinclude -nostdlib -nostartfiles -ffreestanding -static -Ttext 0x200000)

add_custom_command(TARGET kernel.elf POST_BUILD
        COMMAND COMMAND ${CMAKE_CXX_COMPILER} -O binary $<TARGET_FILE:kernel.elf> ${CMAKE_BINARY_DIR}/kernel.bin
)

set(ASM_FILES
        lowlevel/boot/boot.asm
)

foreach(asm ${ASM_FILES})
    get_filename_component(name ${asm} NAME_WE)
    add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/${name}.bin
            COMMAND fasm ${asm} ${CMAKE_BINARY_DIR}/${name}.bin
            DEPENDS ${asm}
    )
    list(APPEND BIN_FILES ${CMAKE_BINARY_DIR}/${name}.bin)
endforeach()

set(IMG_FILE ${CMAKE_BINARY_DIR}/os.img)

add_custom_command(
        OUTPUT ${IMG_FILE}
        COMMAND dd if=/dev/zero of=${IMG_FILE} bs=512 count=32768
        COMMAND dd if=${CMAKE_BINARY_DIR}/boot.bin of=${IMG_FILE} bs=512 count=1 conv=notrunc
        COMMAND dd if=${CMAKE_BINARY_DIR}/middle.bin of=${IMG_FILE} bs=512 seek=1 conv=notrunc
        COMMAND dd if=${CMAKE_BINARY_DIR}/kernel.bin of=${IMG_FILE} bs=512 seek=6 conv=notrunc
        DEPENDS kernel.elf ${BIN_FILES}
)

add_custom_target(disk_image DEPENDS ${IMG_FILE})

add_custom_target(run
        COMMAND qemu-system-i386 -drive format=raw,file=${IMG_FILE}
        DEPENDS disk_image
)

target_include_directories(kernel.elf PRIVATE include)
