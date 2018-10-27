	.file	"heapsort.c"
	.text
	.globl	sift_down
	.type	sift_down, @function
sift_down:
.LFB8:
	.cfi_startproc
	jmp	.L2
.L4:
	cmpq	%rax, %rdi
	je	.L1
	leaq	(%rdx,%rax,8), %rcx
	movq	(%rcx), %rdi
	movq	%rdi, (%r9)
	movq	%r8, (%rcx)
	movq	%rax, %rdi
.L2:
	leaq	(%rdi,%rdi), %rcx
	leaq	1(%rcx), %rax
	cmpq	%rsi, %rax
	jg	.L1
	leaq	(%rdx,%rdi,8), %r9
	movq	(%r9), %r8
	movq	%rax, %r10
	cmpq	(%rdx,%rax,8), %r8
	jl	.L3
	movq	%rdi, %rax
.L3:
	addq	$2, %rcx
	cmpq	%rsi, %rcx
	jg	.L4
	movq	8(%rdx,%r10,8), %r11
	cmpq	%r11, (%rdx,%rax,8)
	jge	.L4
	movq	%rcx, %rax
	jmp	.L4
.L1:
	rep ret
	.cfi_endproc
.LFE8:
	.size	sift_down, .-sift_down
	.globl	heapify
	.type	heapify, @function
heapify:
.LFB9:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movq	%rsi, %r12
	leaq	-2(%rdi), %rbx
	sarq	%rbx
	jmp	.L9
.L10:
	leaq	-1(%rbp), %rsi
	movq	%r12, %rdx
	movq	%rbx, %rdi
	call	sift_down
	subq	$1, %rbx
.L9:
	testq	%rbx, %rbx
	jns	.L10
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE9:
	.size	heapify, .-heapify
	.globl	heap_sort
	.type	heap_sort, @function
heap_sort:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	call	heapify
	subq	$1, %rbx
	jmp	.L13
.L14:
	leaq	0(%rbp,%rbx,8), %rax
	movq	(%rax), %rdx
	movq	0(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	%rdx, 0(%rbp)
	subq	$1, %rbx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movl	$0, %edi
	call	sift_down
.L13:
	testq	%rbx, %rbx
	jg	.L14
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE10:
	.size	heap_sort, .-heap_sort
	.globl	run
	.type	run, @function
run:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rdx
# 0 "" 2
#NO_APP
	movq	%rdx, %r12
	andl	$1, %r12d
	andl	$2, %edx
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rdi
# 0 "" 2
#NO_APP
	movq	%rdi, %rbx
	leaq	0(,%rdi,8), %r8
	leaq	allocator_base(%rip), %rax
	addq	%r8, %rax
	movq	%rax, cur_allocator(%rip)
	movl	$0, %eax
	jmp	.L17
.L18:
	leaq	allocator_base(%rip), %rcx
	leaq	(%rcx,%rax,8), %rcx
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rsi
# 0 "" 2
#NO_APP
	movq	%rsi, (%rcx)
	addq	$1, %rax
.L17:
	cmpq	%rbx, %rax
	jl	.L18
	testq	%rdx, %rdx
	je	.L19
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	movl	$0, %eax
	jmp	.L20
.L21:
	leaq	0(%rbp,%rax,8), %rdx
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rcx
# 0 "" 2
#NO_APP
	movq	%rcx, (%rdx)
	addq	$1, %rax
.L20:
	cmpq	%rax, %rbx
	jg	.L21
.L22:
	movq	%rbp, %rsi
	call	heap_sort
	testq	%r12, %r12
	je	.L16
	movl	$0, %eax
	jmp	.L25
.L19:
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	jmp	.L23
.L24:
	leaq	0(%rbp,%rdx,8), %rax
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rcx
# 0 "" 2
#NO_APP
	movq	%rcx, (%rax)
	addq	$1, %rdx
.L23:
	cmpq	%rdx, %rbx
	jg	.L24
	jmp	.L22
.L27:
	movq	0(%rbp,%rax,8), %rdx
#APP
# 17 "x86prime_lib.h" 1
	out %rdx,(0)
# 0 "" 2
#NO_APP
	addq	$1, %rax
.L25:
	cmpq	%rax, %rbx
	jg	.L27
.L16:
	movq	%rbp, %rax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	run, .-run
	.comm	allocator_base,8,8
	.comm	cur_allocator,8,8
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
