		.data
n:      .word 9


        .text
main: 	addi     t0, x0, 0
		addi     t1, x0, 1
	    la       t3, n
	    lw       t3, 0(t3)
fib: 	beq      t3, x0, finish
	    add      t2, t1,t0
	    addi     t0, t1, 0
	    addi     t1, t2, 0
	    addi     t3, t3, -1
	    jal       x0, fib
finish: 
		addi a1, t0, 0
		addi a0, x0, 10
       
		
		ecall # Terminate ecall
		