	.arch armv8-a
	.file	"load.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	mov	w1, 0
	mov	w0, 0
.L3:
	ands	w3, w1, 1
	add	w2, w0, w1
	csel	w0, w2, w0, eq
	add	w1, w1, 1
	cmp	w1, 30
	bne	.L3
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
