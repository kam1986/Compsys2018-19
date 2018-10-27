	.file	"quicksort.c"
	.text
	.globl	quick_sort
	.type	quick_sort, @function
quick_sort:
.LFB8:
	.cfi_startproc
	cmpq	%rdx, %rsi
	jl	.L12
	rep ret
.L12:
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	leaq	(%rsi,%rdx), %rax
	sarq	%rax
	movq	(%rdi,%rax,8), %r9
	leaq	-1(%rsi), %r10
	leaq	1(%rdx), %rbx
	jmp	.L3
.L7:
	movq	%rax, %rbx
.L5:
	leaq	-1(%rbx), %rax
	leaq	(%rdi,%rax,8), %rcx
	movq	(%rcx), %r8
	cmpq	%r9, %r8
	jg	.L7
	cmpq	%rax, %r10
	jge	.L6
	movq	%r8, 0(%rbp)
	movq	%r11, (%rcx)
	movq	%rax, %rbx
.L3:
	addq	$1, %r10
	leaq	(%rdi,%r10,8), %rbp
	movq	0(%rbp), %r11
	cmpq	%r9, %r11
	jl	.L3
	jmp	.L5
.L6:
	movq	%rdx, %r12
	movq	%rdi, %rbp
	movq	%rax, %rdx
	call	quick_sort
	movq	%r12, %rdx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	quick_sort
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE8:
	.size	quick_sort, .-quick_sort
	.globl	run
	.type	run, @function
run:
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
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rcx
# 0 "" 2
#NO_APP
	movq	%rcx, %r12
	andl	$1, %r12d
	andl	$2, %ecx
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rdx
# 0 "" 2
#NO_APP
	movq	%rdx, %rbx
	leaq	0(,%rdx,8), %r8
	leaq	allocator_base(%rip), %rax
	addq	%r8, %rax
	movq	%rax, cur_allocator(%rip)
	movl	$0, %eax
	jmp	.L14
.L15:
	leaq	allocator_base(%rip), %rsi
	leaq	(%rsi,%rax,8), %rsi
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rdi
# 0 "" 2
#NO_APP
	movq	%rdi, (%rsi)
	addq	$1, %rax
.L14:
	cmpq	%rbx, %rax
	jl	.L15
	testq	%rcx, %rcx
	je	.L16
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	movl	$0, %eax
	jmp	.L17
.L18:
	leaq	0(%rbp,%rax,8), %rcx
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rsi
# 0 "" 2
#NO_APP
	movq	%rsi, (%rcx)
	addq	$1, %rax
.L17:
	cmpq	%rax, %rbx
	jg	.L18
.L19:
	subq	$1, %rdx
	movl	$0, %esi
	movq	%rbp, %rdi
	call	quick_sort
	testq	%r12, %r12
	je	.L13
	movl	$0, %eax
	jmp	.L22
.L16:
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	jmp	.L20
.L21:
	leaq	0(%rbp,%rcx,8), %rax
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rsi
# 0 "" 2
#NO_APP
	movq	%rsi, (%rax)
	addq	$1, %rcx
.L20:
	cmpq	%rcx, %rbx
	jg	.L21
	jmp	.L19
.L24:
	movq	0(%rbp,%rax,8), %rdx
#APP
# 17 "x86prime_lib.h" 1
	out %rdx,(0)
# 0 "" 2
#NO_APP
	addq	$1, %rax
.L22:
	cmpq	%rax, %rbx
	jg	.L24
.L13:
	movq	%rbp, %rax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE9:
	.size	run, .-run
	.comm	allocator_base,8,8
	.comm	cur_allocator,8,8
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
