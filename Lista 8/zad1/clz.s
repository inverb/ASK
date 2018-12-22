        .global clz
        .type clz,@function

        .section .text

clz:
        cmp         $0, %rdi
        jne         .dalej
        movq        $64, %rax
        ret
.dalej:
        max =       %rcx
        min =       %r8
        average =   %rax
        movq        $0, min
        movq        $65, max
.loop:
        movq        $0, %rdx
        movq        min, average
        addq        max, average
        movq        $2, %r9
        divq        %r9
        movq        %rdi, %rsi
        movq        average, %r9
        jmp         .shift
.next:
        cmpq        $0, %rsi
        jz          .less
        movq        average, min
        jmp         .condition
.less:
        movq        average, max
.condition:
        movq        max, average
        subq        min, average
        cmpq        $1, average
        jg          .loop
        movq        $64, average
        subq        min, average
        subq        $1, average
        ret
.shift:
        cmpq        $0, %r9
        je          .next
        subq        $1, %r9
        shr         $1, %rsi
        jmp         .shift

# vim: ts=8 sw=8 et
