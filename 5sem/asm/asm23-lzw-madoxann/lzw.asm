section .text
    extern lzw_decode

; по скорости как-то так:
; Time 'test_data/in1': 3399
; Time 'test_data/in2': 2955
; что на самом деле оч печально, ведь эталонный результат
; Time 'test_data/in1': 2597
; Time 'test_data/in2': 1733
; но справедливости ради, я вообще не оптимизировал, парился только над корректностью
; можно попробоавть читать по 4-8 байт и выходить за границы
lzw_decode:
    push ebx
    push ebp
    push esi
    push edi
    ; test if null
    mov eax, [esp + 16 + 8]
    test eax, eax
    jz lzw_ret_correct
    xor eax, eax
    not eax

    test eax, [esp + 16 + 4]
    jz lzw_ret_err

    test eax, [esp + 16 + 12]
    jz lzw_ret_err

    test eax, [esp + 16 + 16]
    jz lzw_ret_err

    mov edi, [esp + 16 + 16]; l

    ;mov eax, [esp + 16 + 4]; in
    sub esp, 12300 ; 12 + 4096 + 8192 = 4(bits) + 4(byte_buf) + 4 (table_size)+ 12288

    mov dword [esp + 12], 0; bbit
    mov dword [esp + 8], 0; byte_buf
    mov dword [esp + 4], 9 ; table_size - always on stack

    mov ebx, 258 ; slot
    push -1 ; future
    push -1 ; old
    push 0 ; c
    mov edx, 512;highest

    ;LEN IN EDI
lzw_decode_loop:
    ;get_code
    mov ecx, [(esp + 12300 + 16 + 4) + 4 * 3] ; ecx = in
    ; ebp - c reg
    mov ebp, [esp + 12 + 12]; bbit
    mov eax, [esp + 8 + 12]; byte_buf
    mov esi, [(esp + 12300 + 16 + 8) + 4 * 3] ; esi = in_size

    lzw_get_code:
        cmp ebp, [esp + 4 + 12]
        jge lzw_get_code_exit

        test esi, esi
        lea esi, [esi - 1]
        jz lzw_ret_buffer_is_over

        shl eax, 8
        or al, [ecx]
        lea ecx, [ecx + 1]
        add ebp, 8
        jmp lzw_get_code
    lzw_get_code_exit:
        mov [(esp + 12300 + 16 + 4) + 4 * 3], ecx ; in  =ebp
        mov [(esp + 12300 + 16 + 8) + 4 * 3], esi ; esi = in_size
        sub ebp, [esp + 4 + 12]
        mov [esp + 12 + 12], ebp
        mov [esp + 8 + 12], eax

        xchg ecx, ebp
        shr eax, cl
        xchg ecx, ebp

        lea ecx, [edx - 1]
        and ecx, eax
        mov dword [esp + 4], ecx; mov c
        ; end get_code

        cmp ecx, 257 ; end
        je lzw_ret_alloc_correct
        cmp ecx, 256 ; clear
        jne lzw_decode_main

        mov dword [esp + 4 + 12], 9; table_size
        mov ebx, 258; slot
        mov edx, 512; highest
        mov dword [esp + 4 * 3], -1 ;future
        mov dword [esp + 4 * 2], -1 ;old
        jmp lzw_decode_loop
        ; free: eax, ecx(code), ebp(sz), esi(flag)| edx - highest ebx - slot edi - l
    lzw_decode_main:
        ;mov ecx, ebp; ecx = code (= c = ebp)
        mov ebp, 1; sz
        mov esi, 0; flag

        cmp ecx, ebx; double-check
        jne out_of_slot

        mov eax, [esp + 4 * 3]; future

        test eax, eax
        js out_of_slot

        mov esi, 1
        inc ebp
        mov ecx, [esp + 4 * 2];old

        jmp multiple
    out_of_slot:
        cmp ecx, ebx
        jge lzw_ret_err

        multiple:
            cmp ecx, 258
            jl lzw_write

            inc ebp
            lea eax, [esp + 4 * 3 + ecx*2 + 12]; prefix[ecx]
            ;lea eax, [eax + ecx]
            movzx ecx, word [eax];
            jmp multiple

    lzw_write:
        mov eax, [(esp + 12300 + 16 + 12) + 4 * 3]; out; instead of free register

        mov [eax], cl; out = code
        test edi, edi; l
        lea edi, [edi - 1]
        jz lzw_ret_buffer_is_over

        test esi, esi; esi - flag
        jz not_flag

        test edi, edi; l
        lea edi, [edi - 1]
        jz lzw_ret_buffer_is_over
        dec ebp
        lea esi, [eax + ebp]
        mov ecx, [esp + 4 * 3]; ecx will be reassigned
        mov [esi], ecx; *flag = future
        inc esi

        mov ecx, [esp + 4 * 2]; code = old
        jmp table_push
    not_flag:
        lea esi, [eax + ebp] ;flag = (size_t) (out + sz);
        mov ecx, [esp + 4]; code = c

    table_push:; eax is still out
        lea eax, [eax + ebp]; ebp is free
        mov [(esp + 12300 + 16 + 12) + 4 * 3], esi
        xchg eax, ebp ; no 8-bit part for ebp :(
        table:
            cmp ecx, 258
            jl table_end

            test edi, edi; l
            lea edi, [edi - 1]
            jz lzw_ret_buffer_is_over
            lea ebp, [ebp - 1]
            movzx eax, byte [esp + 8192 + 4 * 3 + 12 + ecx]; suffix[code]
            mov [ebp], al
            movzx eax, word [esp + 4 * 3 + 12 + ecx*2]; prefix[code]
            mov ecx, eax; ok?
            jmp table

    table_end:; esi, ebp, eax - free
                ; edx holds suffix, esi holds prefix
        ;mov ebp, edx; ebp - suffix
        ;pop edx; edx - highest, ebx - slot, ecx - code

        mov eax, [esp + 4 * 2]; old - fixed

        cmp ebx, edx
        jge set_codes

        test eax, eax
        js set_codes
        mov [esp + 8192 + 4 * 3 + 12 + ebx], cl; suffux
        mov [esp + 4 * 3 + 12 + ebx * 2], ax; prefix
        inc ebx
    set_codes:
        mov eax, [esp + 4]
        mov [esp + 4 * 3], ecx ; future = code
        mov [esp + 4 * 2], eax ;old = c

        lea eax, [ebx + 1]

        cmp eax, edx
        jl lzw_decode_loop

        mov eax, [esp + 4 + 12]; table_size

        cmp eax, 12
        jge lzw_decode_loop;is it correct?

        inc eax
        mov [esp + 4 + 12], eax; table_size
        shl edx, 1

        jmp lzw_decode_loop

lzw_ret_buffer_is_over:
    add esp, 12312
    jmp lzw_ret_err
lzw_ret_alloc_correct:; need to know l
    add esp, 12312
    mov eax, [esp + 16 + 16]
    sub eax, edi
    jmp lzw_ret
lzw_ret_correct:
    mov eax, 0
    jmp lzw_ret
lzw_ret_err:
    mov eax, -1
lzw_ret:
    pop edi
    pop esi
    pop ebp
    pop ebx

    ret
