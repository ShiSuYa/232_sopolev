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
    
    not t2, zero
    slli t2, t2, 2
    and t2, t0, t2
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    not t2, zero
    srli t2, t2, 30
    or t2, t0, t2
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    li t2, 1
    sll t2, t2, t1
    or t3, t0, t2
    li a7, 1
    mv a0, t3
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    li t2, 1
    sll t2, t2, t1
    not t2, t2
    and t3, t0, t2
    li a7, 1
    mv a0, t3
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    addi t2, t1, 3
    slt t3, t0, t2
    slt t4, t2, t0
    or t2, t3, t4
    li a7, 1
    mv a0, t2
    ecall
    la a0, newline
    li a7, 4
    ecall
    
    li t2, -5
    slt t3, t2, t0
    li t4, 5
    slt t5, t1, t4
    and t2, t3, t5
    li a7, 1
    mv a0, t2
    ecall
    
    li a7, 10
    ecall
