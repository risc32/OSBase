ORG 0x7C00
use16

; Долбаный загрузчик написан дипсиком!!!! Я не присваиваю себе написание именно этого файла и других ассемблерных файлов.
; Я скопипастил из нейронки точно так же, как и все остальные с osdev.org и перешел сразу к созданию ядра. Я не мазохист
TRANSITION_ADDR equ 0x10000
TRANSITION_SECTORS equ 2
TRANSITION_START_SECTOR equ 2

Main:
    jmp 0x0000:.FlushCS

.FlushCS:
    mov ax, TRANSITION_ADDR shr 4
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 4
    mov ch, 0
    mov cl, TRANSITION_START_SECTOR
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc .disk_error

    cli
    lgdt [GDT_Descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:.ProtectedMode

use32
.ProtectedMode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    cld

    mov esp, 0x200000

    jmp TRANSITION_ADDR

.disk_error:
    mov si, DiskError
    call Print

.Die:
    hlt
    jmp .Die

getch:
    mov ah, 0x00
    int 0x16
    ret

.text:
.end:
    ret


Print:
    pusha
.PrintLoop:
    lodsb
    test al, al
    jz .PrintDone
    mov ah, 0x0E
    int 0x10
    jmp .PrintLoop
.PrintDone:
    popa
    ret

LGASK db "Boot in graphics mode? [Y/n]", 0x0D, 0x0A, 0
DiskError db "Disk Error.", 0
egm db "Graphics error.", 0
align 4
GDT_Descriptor:
    dw GDT_End - GDT_Start - 1
    dd GDT_Start

GDT_Start:
    dd 0, 0
    dw 0xFFFF
    dw 0
    db 0
    db 0x9A
    db 0xCF
    db 0
    dw 0xFFFF
    dw 0
    db 0
    db 0x92
    db 0xCF
    db 0
GDT_End:
times 510 - ($-$$) db 0
dw 0xAA55