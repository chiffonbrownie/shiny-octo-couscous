Loop:                   # loop id x1 != 0
    beq  x1, x0, End    # if (r0 > 0)
    bge  x0, x1, Else   # else
    addi x1, x1, -1     # r0 = r0 - 1;
    jal  x0, Loop
Else:
    addi x1, x1, 1      # r0 = r0 + 1;
    jal  x0, Loop
End: