runtest:
    movq $2, %rbx
    movq $3, %rcx
    addq %rcx, %rbx
    addq $4, %rbx
    ret %r15