	add x1, x2, x3
loop1:	addi x1, x2, 3
	lw   x3, 4(x1)
	bne  x2, x3, loop1
loop: 	or x2, x3, x1
	jal x0, loop

	slli x1, x2, 3
