.data
newline: .asciz "\n"

.text
.globl main
main:
    li a7, 5
    ecall
    mv t0, a0
    
    li a7, 5
    ecall
    mv t1, a0
    
    srli t2, t0, 2
    addi t3, t1, -1
    slli t3, t3, 3
    add t2, t2, t3
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    sll t2, t0, t1
    addi t2, t2, -10
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    sra t2, t0, t1
    addi t2, t2, 10
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    slli t2, t0, 2
    sub t2, t2, t1
    addi t2, t2, 5
    srai t2, t2, 1
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    slli t2, t0, 2
    add t2, t2, t0
    add t2, t2, t0
    slli t3, t1, 1
    add t3, t3, t1
    sub t2, t2, t3
    li a7, 1
    mv a0, t2
    ecall
    
    li a7, 10
    ecall
