	.file	"add.c"
	.text
	.globl	add
	.type	add, @function
add:
.LFB22:
	.cfi_startproc
	leal	(%rdi,%rsi), %eax
	ret
	.cfi_endproc
.LFE22:
	.size	add, .-add
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	movl	$4, %esi
	movl	$3, %edi
	call	add
	ret
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
