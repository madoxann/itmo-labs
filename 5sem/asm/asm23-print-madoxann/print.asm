section .text
   global print

; Да. Это тупо переписанный вручную код на C. Ну и че вы мне сделаете? Задача этой домашики просто написать правильный код. Удачи :)

is_not_zero:
    mov eax, [esp + 4]
    push ebx
    mov edx, 0
    mov ebx, 8
not_zero_loop:
    test ebx, ebx
    jz not_zero_leave
    mov cx, word [eax]
    lea eax, [eax + 2]
    or edx, ecx
    dec ebx
    jmp not_zero_loop
not_zero_leave:
    pop ebx
    mov eax, edx
    ret

hex_to_dec:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]
    cmp eax, '9'
    jg A_to_dec
    cmp eax, '0'
    jl ub_to_dec
    sub eax, '0'
    jmp hex_to_dec_return
A_to_dec:
    cmp eax, 'F'
    jg a_to_dec
    cmp eax, 'A'
    jl ub_to_dec
    sub eax, 'A' - 10
    jmp hex_to_dec_return
a_to_dec:
    cmp eax, 'f'
    jg ub_to_dec
    cmp eax, 'a'
    jl ub_to_dec
    sub eax, 'a' - 10
    jmp hex_to_dec_return
ub_to_dec:
    xor eax, eax
hex_to_dec_return:
    mov esp, ebp
    pop ebp
    ret

uint128_inc:
    push ebp
    mov eax, [esp + 8]
    movzx edx, word [eax] ; edx - x
    inc edx
    mov [eax], dx
    mov ecx, 8 ; ecx - i
uint128_inc_for:
    lea eax, [eax + 2]
    shr edx, 16
    movzx ebp, word [eax]
    add edx, ebp
    mov [eax], dx

    dec ecx
    test ecx, ecx
    jnz uint128_inc_for

    pop ebp
    ret

uint128_divmod:
    push ebp
    push esi
    mov esi, [esp + 4 + 4 * 2]
    lea esi, [esi + 7 * 2]
    movzx eax, word [esi] ; eax - x
    mov ecx, 7 ; ecx - i
    mov ebp, 10
uint128_div_for:
        xor edx, edx
        div ebp
        mov word [esi], ax
        shl edx, 16
        lea esi, [esi - 2]
        movzx eax, word [esi]
        add eax, edx

        dec ecx
        test ecx, ecx
        jnz uint128_div_for
    xor edx, edx
    div ebp
    mov word [esi], ax
    mov eax, edx

    pop esi
    pop ebp
    ret

convert_64:
    push ebp
    mov ebp, esp
    push esi
    push ebx
    push edi

    mov eax, dword [ebp + 16]; eax = sz
    cdq
    mov ebx, 4
    idiv ebx
    add eax, edx
    sub eax, 1
    cmp eax, dword [ebp + 12]
    jl convert_zero
    mov eax, dword [ebp + 12]; eaz = no
    lea edx, [eax * 4 + 4]
    mov ebx, dword [ebp + 16]
    sub ebx, edx; ebx = begin
    mov edx, 4; edx = length
    test ebx, ebx
    jns convert_full_length
    lea ebx, [eax * 4]
    mov edx, dword [ebp + 16]
    sub edx, ebx
    xor ebx, ebx
convert_full_length:
    xor edi, edi ; = result
    lea esi, [ebx + edx - 1]; ok? = i

convert_body:
    cmp esi, ebx
    mov eax, edi
    jl convert_return

    mov ecx, ebx
    add ecx, edx
    dec ecx
    sub ecx, esi
    lea ecx, [4*ecx]
    mov eax, [ebp + 8]
    lea eax, [eax + esi]
    movzx eax, byte [eax]
    push eax
    call hex_to_dec
    add esp, 4
    sal eax, cl
    or edi, eax

    dec esi
    jmp convert_body
convert_zero:
    xor eax, eax
convert_return:
    pop edi
    pop ebx
    pop esi
    mov esp, ebp
    pop ebp
    ret

print_remainder:
    push ebx
    mov ebx, [esp + 8 + 4]; format
    push ebp
    push esi
    push edi
    sub esp, 8; is_plus-is_minus-is_empty-is_zero-width
    mov byte [esp + 8], 0
    mov byte [esp + 7], 0
    mov byte [esp + 6], 0
    mov byte [esp + 5], 0
    mov dword [esp], 0
    mov esi, -1; cnt
    xor ecx, ecx; i
format_loop:
    movzx eax, byte [esp + 5]; is_zero
    movzx eax, byte [ebx + ecx];out[i]
    test eax, eax
    jz format_loop_end
    cmp eax, '-'
    jne test_plus
    mov byte [esp + 7], 1; is_minus
    mov byte [esp + 5], 0; is_zero
    jmp test_end
test_plus:
    cmp eax, '+'
    jne test_empty
    mov byte [esp + 8], 1; is_plus
    mov byte [esp + 6], 0;is_empty
    jmp test_end
test_empty:
    cmp eax, ' '
    jne test_zero
    movzx edx, byte [esp + 8]; tmp - is_plus
    test edx, edx
    jnz test_zero
    mov byte [esp + 6], 1;is_empty
    jmp test_end
test_zero:
    cmp eax, '0'
    jne test_number
    movzx edx, byte [esp + 7]; tmp - is_minus
    test edx, edx
    jnz test_number
    mov byte [esp + 5], 1; is_zero
    jmp test_end
test_number:
    cmp eax, '1'
    jl test_end
    cmp eax, '9'
    jg test_end
    mov esi, ecx; cnt = i
    jmp format_loop_end
test_end:
    inc ecx
    jmp format_loop
format_loop_end:
    cmp esi, -1
    je skip_number
    mov ecx, esi
    xor eax, eax; eax - width
    mov edi, 10; temp - for mul
number_loop:
    movzx esi, byte [ebx + ecx]; esi - format[i]
    test esi, esi
    jz number_end
    sub esi, '0'
    mul edi
    add eax, esi

    inc ecx
    jmp number_loop
number_end:
    ;movzx eax, byte [esp + 5]
    mov [esp], eax; eax - width
skip_number:
    ;movzx eax, byte [esp + 5]
    ; all register clear
    ; eax = width
    ; eax - tmp - for iteration; ecx - i; edx - out_buff?; ebx - free(width) ebp - kostil esi - has_sign edi-(2nd tmp)pos?
    mov ebx, eax; width
    mov edx, [esp + 8 + 4*4 + 4]; out_buf
    mov esi, [esp + 8 + 4*4 + 4*4]; has_sign = sign_minus
    movzx eax, byte [esp + 8]
    or esi, eax ;plus
    movzx eax, byte [esp + 6]
    or esi, eax;empty
    mov ebp, [esp + 8 + 4*4 + 4*3]; pos-tmp, filter is not needed; kostil = pos
    ;mov ebp, ebx; kostil = pos
    xor ecx, ecx; i = 0
    test esi, esi; has_sign = 0 -> shift loop does nothing
    jz skip_shift
shift_loop:
    cmp ecx, ebp
    jge skip_shift
    lea edi, [edx + ebp]; out[pos]
    sub edi, ecx; [pos - i]
    sub edi, esi ;out[pos - i - has_sign]
    movzx eax, byte [edi]
    lea edi, [edi + esi]; out[pos - i]
    ;mov byte [edi], eax
    mov byte [edi], al
    inc ecx
    jmp shift_loop
skip_shift:
    mov eax, [esp + 8 + 4*4 + 4*4];sign_minus
    test eax, eax
    jz print_plus
    mov byte [edx], '-'
    jmp skip_print
print_plus:
    movzx eax, byte [esp + 8]; is_plus
    test eax, eax
    jz print_empty
    mov byte [edx], '+'
    jmp skip_print
print_empty:
    movzx eax, byte [esp + 6];is_empty
    test eax, eax
    jz skip_print
    mov byte [edx], ' '
    jmp skip_print
skip_print:
    ; mb split here?
    cmp ebx, ebp
    jle null_terminate
    add ebp, esi; kotsil = pos+has_sign;
    movzx eax, byte [esp + 7]; tmp - is_minus
    test eax, eax
    jz skip_fill
    mov ecx, ebp; i = kostil
    movzx eax, byte [esp + 5]; is_zero
    test eax, eax
    jnz set_zero
    mov eax, ' '
    jmp first_fill
set_zero:
    mov eax, '0'; eax = filler_char
first_fill:
    cmp ecx, ebx; i < width
    jge null_terminate_width
    lea edi, [edx + ecx]; out[i]
    ;mov byte [edi], eax
    mov byte [edi], al
    inc ecx
    jmp first_fill
skip_fill:
    ; else-block
    mov ecx, ebp
    dec ecx ; i = kostil - 1
second_fill:
    cmp ecx, -1
    jle third_fill
    lea edi, [edx + ecx]; out[i]
    movzx eax, byte [edi]
    ;lea edi, [edi - ebp]; out[i - kostil]
    sub edi, ebp; out[i - kostil]
    lea edi, [edi + ebx]; out[i - kostil + width]
    ;mov byte [edi], eax
    mov byte [edi], al
    dec ecx
    jmp second_fill
third_fill:
    ; pos no longer needed in else!
    ; kostil = pos + has_sign
    sub ebp, esi ; pos
    mov eax, [esp]; width
    sub eax, ebp
    mov ebp, eax; diff!
    movzx eax, byte [esp + 5]; is_zero
    test eax, eax
    jnz set_third_zero
    mov eax, ' '
    jmp third_if
set_third_zero:
    mov eax, '0'; eax = filler_char
third_if:
    ; width not really needed
    movzx ebx, byte [esp + 5]; is_zero
    test ebx, ebx
    jnz third_fill_signed
    xor ecx, ecx; i = 0
    sub ebp, esi; diff - has_sign
third_fill_not_signed:
    cmp ecx, ebp
    jge null_terminate_width
    lea edi, [edx + ecx]; out[i]
    ;mov byte [edi], eax; out[i] = filler_char
    mov byte [edi], al; out[i] = filler_char
    inc ecx
    jmp third_fill_not_signed
third_fill_signed:
    mov ecx, esi; i = has_sign
third_fill_signed_loop:
    cmp ecx, ebp
    jge null_terminate_width
    lea edi, [edx + ecx]; out[i]
    ;mov byte [edi], eax; out[i] = filler_char
    mov byte [edi], al; out[i] = filler_char
    inc ecx
    jmp third_fill_signed_loop
null_terminate_width:
    mov eax, [esp]; width
    lea edi, [edx + eax]; out[width]
    mov byte [edi], 0
    jmp print_remainder_ret
null_terminate:
    ; ebp still pos
    lea edi, [edx + ebp];out[pos]
    lea edi, [edi + esi];out[pos+has_sign]
    mov byte [edi], 0
print_remainder_ret:
    add esp, 8
    pop edi
    pop esi
    pop ebp
    pop ebx

    ret

print:
    push ebx
    mov ebx, [esp + 4 + 4 * 3]; hex_number
    push ebp
    push esi
    push edi
    ; eax, ecx - tmp, edx - len, edi - sign_minus
    xor edx, edx
hexlen:
    movzx eax, byte [ebx + edx]
    test eax, eax
    jz skip_hexlen
    inc edx
    jmp hexlen
skip_hexlen:
    xor edi, edi
eat_minus:
    movzx eax, byte [ebx]
    cmp eax, '-'
    jne eat_0x
    inc ebx
    xor edi, 1
    dec edx
    jmp eat_minus
eat_0x:
    cmp edx, 2
    jl print_convert
    cmp byte [ebx], '0'
    jne print_convert
    cmp byte [ebx + 1], 'x'
    je eat_0x_correct
    cmp byte [ebx + 1], 'X'
    je eat_0x_correct
    jmp print_convert
eat_0x_correct:
    add ebx, 2; hex+=2
    sub edx, 2; len -=2
print_convert:
    sub esp, 16; parts
    xor ebp, ebp; ebp = ecx, ecx is not saved :/
print_convert_for:
    cmp ebp, 8
    jge print_convert_for_end; call here - edx is spoiled
    push edx
    push ebp
    push ebx
    call convert_64
    add esp, 4*2
    pop edx
    mov word [esp + 2*ebp], ax; parts?
    inc ebp
    jmp print_convert_for
print_convert_for_end:
    mov al, [esp + 15]
    and eax, 128
    cmp eax, 128
    jne divmod_loop
    xor edi, 1
    xor ecx, ecx
flip_for:
    cmp ecx, 8
    jge flip_end
    xor word [esp + 2*ecx], 0xFFFF
    inc ecx
    jmp flip_for
flip_end:
    lea esi, [esp]
    push esi
    call uint128_inc
    add esp, 4
    mov al, [esp+15]
    and eax, 128
    cmp eax, 128
    jne divmod_loop
    mov edi, 1
divmod_loop:
    push edi ; on stack for print_remainder call!!!
    ; pos will be edi
    xor edi, edi
    lea ebx, [esp + 4]; parts - hex_number no longer needed
    mov esi, [esp + 4 + 16 + 4*4 + 4]; out
divmod_while:
    push ebx
    call is_not_zero
    test eax, eax
    lea esp, [esp + 4]
    jz divmod_end
    push ebx
    call uint128_divmod
    add esp, 4
    add eax, '0'
    mov word [esi + edi], ax
    inc edi
    jmp divmod_while
divmod_end:
    test edi, edi
    jnz swap_remainder
    pop eax; get sign_minus
    mov eax, 0
    push eax
    mov byte [esi], '0'
    mov byte [esi + 1], 0
    inc edi
    jmp swap_remainder
    ;add esp, 4; no need for print_remainrer -> no need for edi on stack
    ;jmp print_ret
swap_remainder:
    push edi; setup for print_remainder...
    mov eax, edi; eax = pos
    cdq
    mov ebx, 2; tmp
    idiv ebx
    ; eax - pos/2, edi - pos
    xor ecx, ecx
    ; edx=length not needed..., ebx=parts as well
    mov ebp, eax
swap_for:
    cmp ecx, ebp
    jge print_remainder_call
    lea ebx, [esi + edi]
    sub ebx, ecx
    dec ebx; ebx = out[pos - i - 1]
    movzx edx, byte [ebx]; tmp
    movzx eax, byte [esi + ecx]; out[i]
    mov byte [ebx], al
    xchg eax, edx
    mov byte [esi + ecx], al
    inc ecx
    jmp swap_for
print_remainder_call:
    mov eax, [esp + 8 + 16 + 4*4 + 4*2]
    push eax
    push esi
    call print_remainder
    add esp, 4*4
print_ret:
    add esp, 4*4
    pop edi
    pop esi
    pop ebp
    pop ebx

    ret
