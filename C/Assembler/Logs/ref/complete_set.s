label1:
	add x1, x2, x3
	or t1, t2, t3
	sub x4, x5, x6
label2:
	jal x7, label1
	sll x8, x9, t4
	addi x10, x11, 0x2f
	andi x12, x13, 0xff
	ori x14, x15, 25
	lb x16, 5, x17
label3:
	lw x18, 5, x19
	sb x20, 100, x21
	sw x22, 100, x23
	beq x24, x0, label2
	blt x25, x26, label4
	jalr x27, x28, label1
label4:
	slli x29, x30, 5
label5:
	sra x31, t6, x2
	bne t5, zero, label5
	add t0, s2, s3
	add s4, s5, s6
	sub s7, s8, s9
	or s10, s11, s0
	sll s1, a0, a1
	add a2, a3, a4
	sub a5, a6, a7
	add fp, tp, gp
	sub sp, ra, x1

