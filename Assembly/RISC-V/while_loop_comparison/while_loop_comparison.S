    addi x6, x0, 0      # int result = 0
    addi x29, x0, 100   # number of iterations
Loop:
    lw   x7, 0(x10)     # x7 = *MemArray
    add  x5, x5, x7     # j = j + *MemArray
    addi x10, x10, 8    # MemArray = MemArray + 2
    addi x6, x6, 1      # result ++
    blt  x6, x29, Loop  # while (result < 100)