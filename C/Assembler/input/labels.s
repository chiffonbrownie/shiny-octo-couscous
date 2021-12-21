label1: add   x1, x2, x0

label2:	beq x1,x2, label4
	bne x2, x3, label1
label3:	jal x0, label2
	bne x3, x4, label1
			
label4:	jal x0, label1

	beq x1, x2, label2
