#!/bin/bash
./assembler input/simple.s out/simple.int out/simple.out
./assembler input/comments.s out/comments.int out/comments.out
./assembler input/imm.s out/imm.int out/imm.out
./assembler input/labels.s out/labels.int out/labels.out
./assembler input/p1_errors.s out/p1_errors.int out/p1_errors.out -log log/p1_errors.log
./assembler input/p2_errors.s out/p2_errors.int out/p2_errors.out -log log/p2_errors.log
./assembler input/lab2.s out/lab2.int out/lab2.out
