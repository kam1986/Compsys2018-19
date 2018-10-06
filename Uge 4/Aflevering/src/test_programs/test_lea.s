runtest:
    movq $2, %rax
    movq $3, %rbx
    movq $4, %rcx
    leaq $5(%rbx, %rcx, 2), %rax
    ret %rax