# Things to catch

	add x1, x4, x1, x2, x5			# Extra arguments
label:
	or  x2, x2, x3
	jal x2, label
3hello:											#Invalid label
								
	sub x1, x2, x3 slli x1, x2, 3		# Multiple instructions
		
label:	lb x1, 4(x2)				# Multiple definition of label
	jal x0, label
l1: l2: addi x1, x1, 5				# Multiple labels on one line
	ori x3, x0, 8
		
# Things to ignore
	addi x3, x44, 4				#invalid register
	ori x1, x2, 65365			# invalid immediate
	bne x1, x2,  not_found			# nonexistant label
