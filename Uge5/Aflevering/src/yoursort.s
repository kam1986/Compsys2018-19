	.file	"yoursort.c"
	.text
	.globl	dual_pivot_quick_sort
	.type	dual_pivot_quick_sort, @function
dual_pivot_quick_sort:
.LFB8:
	.cfi_startproc
	cmpq	%rdx, %rsi
	jl	.L15
	rep ret
.L15:
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	leaq	(%rdi,%rsi,8), %r15
	movq	(%r15), %rax
	leaq	(%rdi,%rdx,8), %r14
	movq	(%r14), %rcx
	cmpq	%rcx, %rax
	jle	.L3
	movq	%rcx, (%r15)
	movq	%rax, (%r14)
.L3:
	movq	(%r15), %r12
	movq	(%r14), %r11
	leaq	1(%rsi), %rbp
	leaq	-1(%rdx), %rbx
	movq	%rbp, %rax
	jmp	.L4
.L5:
	cmpq	%r11, %rcx
	jg	.L7
.L6:
	addq	$1, %rax
.L4:
	cmpq	%rax, %rbx
	jl	.L16
	leaq	(%rdi,%rax,8), %r8
	movq	(%r8), %rcx
	cmpq	%r12, %rcx
	jge	.L5
	leaq	(%rdi,%rbp,8), %r9
	movq	(%r9), %r10
	movq	%r10, (%r8)
	movq	%rcx, (%r9)
	addq	$1, %rbp
	jmp	.L6
.L9:
	subq	$1, %rbx
.L7:
	leaq	(%rdi,%rbx,8), %r9
	movq	(%r9), %r10
	cmpq	%r11, %r10
	jle	.L8
	cmpq	%rax, %rbx
	jg	.L9
.L8:
	movq	%r10, (%r8)
	movq	%rcx, (%r9)
	subq	$1, %rbx
	movq	(%r8), %rcx
	cmpq	%r12, %rcx
	jge	.L6
	leaq	(%rdi,%rbp,8), %r9
	movq	(%r9), %r10
	movq	%r10, (%r8)
	movq	%rcx, (%r9)
	addq	$1, %rbp
	jmp	.L6
.L16:
	movq	%rdx, %r12
	movq	%rdi, %r13
	movq	(%r15), %rdx
	leaq	-8(%rdi,%rbp,8), %rax
	movq	(%rax), %rcx
	movq	%rcx, (%r15)
	movq	%rdx, (%rax)
	movq	(%r14), %rdx
	leaq	8(%rdi,%rbx,8), %rax
	movq	(%rax), %rcx
	movq	%rcx, (%r14)
	movq	%rdx, (%rax)
	leaq	-2(%rbp), %rdx
	call	dual_pivot_quick_sort
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	dual_pivot_quick_sort
	leaq	2(%rbx), %rsi
	movq	%r12, %rdx
	movq	%r13, %rdi
	call	dual_pivot_quick_sort
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE8:
	.size	dual_pivot_quick_sort, .-dual_pivot_quick_sort
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
	jmp	.L18
.L19:
	leaq	allocator_base(%rip), %rsi
	leaq	(%rsi,%rax,8), %rsi
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rdi
# 0 "" 2
#NO_APP
	movq	%rdi, (%rsi)
	addq	$1, %rax
.L18:
	cmpq	%rbx, %rax
	jl	.L19
	testq	%rcx, %rcx
	je	.L20
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	movl	$0, %eax
	jmp	.L21
.L22:
	leaq	0(%rbp,%rax,8), %rcx
#APP
# 6 "x86prime_lib.h" 1
	in (0),%rsi
# 0 "" 2
#NO_APP
	movq	%rsi, (%rcx)
	addq	$1, %rax
.L21:
	cmpq	%rax, %rbx
	jg	.L22
.L23:
	subq	$1, %rdx
	movl	$0, %esi
	movq	%rbp, %rdi
	call	dual_pivot_quick_sort
	testq	%r12, %r12
	je	.L17
	movl	$0, %eax
	jmp	.L26
.L20:
	movq	cur_allocator(%rip), %rbp
	addq	%rbp, %r8
	movq	%r8, cur_allocator(%rip)
	jmp	.L24
.L25:
	leaq	0(%rbp,%rcx,8), %rax
#APP
# 12 "x86prime_lib.h" 1
	in (1),%rsi
# 0 "" 2
#NO_APP
	movq	%rsi, (%rax)
	addq	$1, %rcx
.L24:
	cmpq	%rcx, %rbx
	jg	.L25
	jmp	.L23
.L28:
	movq	0(%rbp,%rax,8), %rdx
#APP
# 17 "x86prime_lib.h" 1
	out %rdx,(0)
# 0 "" 2
#NO_APP
	addq	$1, %rax
.L26:
	cmpq	%rax, %rbx
	jg	.L28
.L17:
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
