	addi x1, x2, x3			# not a number
label:  jal				# no label
	ori x2, 99, 0xAB		# invalid register
	bne x2, x1, not_found		# nonexistant label
	addi x3, x2, 0x80808080		# number too large
# Can you think of any others?
	lw x1, 0x1fff(x2)		# invalid immediate (read_mem)
	sw x1, 0x1fff(x2)		# invalid immediate (write_mem)
	slli x1, x2, 33			# invalid immediate (write_shift_imm)
	beq x1,x0, 0xffff		# invalid immediate (write_branch)
	auipc x1, 0x1ffff		# invalid sentence
