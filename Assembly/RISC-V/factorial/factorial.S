.data
n: .word 7

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a0, x0, 10
    ecall # Exit

factorial:
    addi t1, x0, 1
loop:    
    beq a0, x0, zero_check
    mul t1, t1, a0  # n*(1 or previous n)
    addi a0, a0, -1 # n-1
    jal x0, loop
zero_check:
    mv a0, t1
    ret
