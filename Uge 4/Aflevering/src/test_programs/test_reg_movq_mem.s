runtest:
    movq $4, %rbx
    movq $3, %rax
    movq %rbx, (%rax)
    movq (%rax), %rcx
    ret %r15
