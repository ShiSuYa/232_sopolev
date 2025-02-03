.data
newline: .asciz "\n"

.text
.globl main
main:
    li a7, 5
    ecall
    mv s0, a0
    ecall
    mv s1, a0

    addi t0, s0, 5
    addi t1, s1, -7
    sub a0, t0, t1
    call print_int

    mul t0, s0, s0
    slli t0, t0, 1
    li t1, 3
    mul t1, t1, s1
    sub t0, t0, t1
    addi a0, t0, 4
    call print_int

    addi t0, s0, 5
    div t0, t0, s1
    addi t1, s1, -1
    li t2, 10
    div t1, t2, t1
    add a0, t0, t1
    call print_int

    div t0, s0, s1
    mul t0, t0, s1
    rem t1, s0, s1
    add a0, t0, t1
    call print_int

    slt t0, s1, s0
    mv a0, t0
    call print_int

    li a7, 10
    ecall

print_int:
    li a7, 1
    ecall
    li a7, 4
    la a0, newline
    ecall
    ret
