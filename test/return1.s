	.file	"return1.c"
	.section	".rodata"
	.align 8
.LC0:
	.asciz	"return +1()  = %d\n"
	.section	".text"
	.align 4
	.global main
	.type	main, #function
	.proc	04
main:
	save	%sp, -104, %sp
	st	%i0, [%fp+68]
	st	%i1, [%fp+72]
	st	%i2, [%fp+76]
	st	%g0, [%fp-4]
	ld	[%fp-4], %o0
	call	return1, 0
	 nop
	mov	%o0, %g1
	mov	%g1, %o1
	sethi	%hi(.LC0), %g1
	or	%g1, %lo(.LC0), %o0
	call	printf, 0
	 nop
	mov	0, %g1
	mov	%g1, %i0
	restore
	jmp	%o7+8
	 nop
	.size	main, .-main
	.align 4
	.global return1
	.type	return1, #function
	.proc	04
return1:
	save	%sp, -96, %sp
	st	%i0, [%fp+68]
	ld	[%fp+68], %g1
	add	%g1, 1, %g1
	mov	%g1, %i0
	restore
	jmp	%o7+8
	 nop
	.size	return1, .-return1
	.ident	"GCC: (crosstool-NG 1.21.0) 5.1.0"
	.section	.note.GNU-stack,"",@progbits
