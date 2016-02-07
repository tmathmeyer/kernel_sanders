;
; This file contains the VGA mode 0x13 setup and color palette
;

[bits 32]

global init_vga

global palette

section .text

    ; void init_vga()
    ; Initialize VGA mode (0x13)
    init_vga:
        pushad

        ; Set mode
        mov esi, mode0x13
        call set_regs

        ; Set up palette
        mov esi, palette
        call set_palette

        ; Clear screen
        mov edi, 0xa0000
        mov ax, 0x0000
        mov ecx, 0x20000
        rep stosw

        popad
        ret

    ; Set VGA mode
    set_regs:
        cli
        mov dx, 0x3C2
        lodsb
        out dx, al

        mov dx, 0x3DA
        lodsb
        out dx, al

        xor ecx, ecx
        mov dx, 0x3C4
    .l1:
        lodsb
        xchg al, ah
        mov al, cl
        out dx, ax
        inc ecx
        cmp cl, 4
        jbe .l1

        mov dx, 0x3D4
        mov ax, 0x0E11
        out dx, ax

        xor ecx, ecx
        mov dx,  0x3D4
    .l2:
        lodsb
        xchg al, ah
        mov al, cl
        out dx, ax
        inc ecx
        cmp cl, 0x18
        jbe .l2

        xor ecx, ecx
        mov dx, 0x3cE
    .l3:
        lodsb
        xchg al, ah
        mov al, cl
        out dx, ax
        inc ecx
        cmp cl, 8
        jbe .l3

        mov dx, 0x3DA
        in al, dx

        xor ecx, ecx
        mov dx, 0x3C0
    .l4:
        in ax, dx
        mov al, cl
        out dx, al
        lodsb
        out dx, al
        inc ecx
        cmp cl, 0x14
        jbe .l4

        mov al, 0x20
        out dx, al

        sti
        ret

    ; void set_palette()
    ; Set palette colors
    set_palette:
        push ax
        push cx
        push dx

        xor cx, cx
    .next_color:
        mov dx, 0x03C8
        mov al, cl
        out dx, al
        inc dx
        mov al, byte [esi]
        out dx, al
        inc esi
        mov al, byte [esi]
        out dx, al
        inc esi
        mov al, byte [esi]
        out dx, al
        inc esi

        inc cx
        cmp cx, 256
        jl .next_color

        pop dx
        pop cx
        pop ax
        ret

section .data

    ; 256 color RGB palette (6 bits per channel)
    palette     db 00, 00, 00, 63, 63, 63, 39, 51, 63, 29, 41, 22, 19, 27, 14
                db 25, 38, 17, 16, 25, 11, 30, 40, 21, 20, 26, 14, 22, 34, 14
                db 14, 22, 09, 27, 40, 18, 18, 26, 12, 27, 40, 19, 23, 35, 15
                db 15, 23, 10, 24, 37, 16, 16, 24, 10, 33, 44, 24, 22, 29, 16
                db 35, 46, 25, 23, 30, 16, 31, 41, 21, 20, 27, 14, 26, 38, 18
                db 17, 25, 12, 27, 40, 20, 18, 26, 13, 23, 37, 14, 15, 24, 09
                db 31, 43, 22, 20, 28, 14, 23, 36, 13, 15, 24, 08, 21, 34, 12
                db 14, 22, 08, 24, 37, 15, 26, 37, 17, 17, 24, 11, 23, 35, 14
                db 15, 23, 09, 26, 37, 16, 17, 24, 10, 23, 35, 13, 15, 23, 08
                db 26, 38, 17, 17, 25, 11, 29, 40, 19, 19, 26, 12, 25, 39, 16
                db 16, 26, 10, 28, 40, 19, 28, 39, 18, 26, 39, 18, 17, 26, 12
                db 27, 39, 19, 27, 38, 17, 18, 25, 11, 31, 42, 21, 27, 39, 18
                db 28, 38, 17, 32, 43, 22, 21, 28, 14, 26, 39, 17, 17, 26, 11
                db 23, 36, 14, 20, 33, 11, 13, 22, 07, 18, 32, 10, 12, 21, 06
                db 23, 36, 15, 15, 24, 10, 25, 37, 18, 16, 24, 12, 25, 36, 16
                db 22, 35, 13, 14, 23, 08, 23, 37, 15, 22, 36, 13, 14, 24, 08
                db 29, 41, 19, 19, 27, 12, 27, 39, 17, 18, 26, 11, 18, 31, 09
                db 12, 20, 06, 19, 32, 10, 20, 32, 12, 13, 21, 08, 25, 36, 15
                db 24, 38, 16, 16, 25, 10, 29, 42, 21, 19, 28, 14, 20, 34, 11
                db 21, 35, 12, 24, 38, 15, 29, 41, 20, 19, 27, 13, 30, 42, 20
                db 20, 28, 13, 20, 34, 12, 13, 22, 08, 28, 41, 19, 18, 27, 12
                db 24, 36, 16, 23, 34, 14, 15, 22, 09, 19, 33, 11, 12, 22, 07
                db 22, 34, 13, 31, 43, 21, 32, 43, 23, 21, 28, 15, 29, 41, 21
                db 24, 36, 15, 30, 42, 21, 17, 30, 08, 11, 20, 05, 28, 40, 20
                db 28, 40, 18, 27, 40, 17, 22, 36, 14, 14, 24, 09, 30, 41, 20
                db 20, 27, 13, 33, 45, 23, 22, 30, 15, 32, 44, 23, 21, 29, 15
                db 25, 40, 17, 16, 26, 11, 25, 38, 16, 25, 37, 17, 16, 24, 11
                db 26, 41, 18, 17, 27, 12, 26, 40, 17, 21, 33, 14, 21, 35, 13
                db 27, 39, 21, 18, 26, 14, 27, 37, 18, 18, 24, 12, 31, 42, 22
                db 30, 40, 20, 20, 26, 13, 36, 26, 18, 24, 17, 12, 23, 16, 11
                db 15, 10, 07, 29, 21, 14, 19, 14, 09, 21, 21, 21, 14, 14, 14
                db 17, 12, 08, 11, 08, 05, 26, 26, 26, 17, 17, 17, 22, 17, 13
                db 14, 11, 08, 22, 35, 14, 14, 23, 09, 27, 36, 18, 21, 33, 13
                db 27, 36, 17, 18, 24, 11, 30, 39, 21, 20, 32, 11, 13, 21, 07
                db 25, 34, 16, 16, 22, 10, 19, 32, 11, 12, 21, 07, 25, 35, 16
                db 16, 23, 10, 20, 33, 12, 21, 34, 13, 18, 31, 10, 19, 31, 11
                db 12, 20, 07, 28, 38, 19, 18, 25, 12, 25, 37, 16, 28, 37, 18
                db 28, 37, 19, 29, 38, 20, 19, 25, 13, 19, 31, 10, 17, 29, 08
                db 11, 19, 05, 15, 28, 07, 10, 18, 04, 21, 33, 12, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
                db 00, 00, 00, 00, 00, 00, 00, 00, 00, 36, 25, 46, 40, 47, 52
                db 24, 12, 00

    ; VGA mode 0x13 register values
    mode0x13    db 0x63, 0x00, 0x03, 0x01, 0x0F, 0x00, 0x0E, 0x5F, 0x4F
                db 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00
                db 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28
                db 0x40, 0x96, 0xB9, 0xA3, 0xFF, 0x00, 0x00, 0x00, 0x00
                db 0x00, 0x40, 0x05, 0x0F, 0xFF, 0x00, 0x01, 0x02, 0x03
                db 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C
                db 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00, 0x00