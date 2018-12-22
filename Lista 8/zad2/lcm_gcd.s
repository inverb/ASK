        .globl lcm_gcd
        .type lcm_gcd,@function

        .section .text

lcm_gcd:
        cmpq            %rsi, %rdi
        jg              .swap
.dalej:
        movq            %rdi, %rax
        mulq            %rsi
        movq            %rax, %r8
        movq            %rdi, %rax
        a =             %rax
        b =             %rsi
        r =             %rdx
        ab =            %r8
.loop:
        testq           b, b
        je              .end             
        cdq
        div            b
        movq            b, a
        movq            r, b
        jmp             .loop
.swap:  movq            %rsi, %rax
        movq            %rdi, %rsi
        movq            %rax, %rdi
        jmp .dalej
.end:
        movq            a, b
        movq            ab, %rax
        idiv            b
        movq            b, %rdx
        ret

# vim: ts=8 sw=8 et
