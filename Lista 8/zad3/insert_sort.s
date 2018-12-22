        .global insert_sort
        .type insert_sort,@function

        .section .text

insert_sort:
        movq            %rdi, %rax
.loop1:
        cmpq            %rax, %rsi
        jle             .end
        movq            (%rax), %r8
        movq            %rax, %rbx
        addq            $8, %rbx
        movq            %rax, %rcx
.loop2:
        cmpq            %r8, (%rbx)
        jge             .incr1
        movq            (%rbx), %r9
        movq            %r8, (%rbx)
        movq            %r9, (%rcx)
        subq            $8, %rbx
        subq            $8, %rcx
        movq            (%rcx), %r8
        cmpq            %rcx, %rdi
        jle             .loop2
.incr1:
        addq            $8, %rax
        jmp             .loop1
.end:
        ret

# vim: ts=8 sw=8 et
