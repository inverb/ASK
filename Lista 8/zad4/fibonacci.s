        .global fibonacci
        .type fibonacci,@function

        .section .text

fibonacci:
        cmpq            $0,%rdi
        je              .zero
        cmpq            $1,%rdi
        je              .jeden
        subq            $1, %rdi
        push            %rdi
        call            fibonacci
        pop             %rdi
        push            %rax
        subq            $1, %rdi
        call            fibonacci
        movq            %rax, %rdi
        pop             %rax
        addq            %rdi, %rax
        ret
.jeden:
        movq            $1, %rax
        ret
.zero:
        movq            $0, %rax
        ret

# vim: ts=8 sw=8 et
