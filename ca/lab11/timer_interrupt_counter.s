.data
m:      .word 10
t:      .word 500
count:  .word 0

.text
.globl _start
_start:
    la t0, handler
    csrw utvec, t0

    li t1, 0x10
    csrrs zero, uie, t1

    li t1, 0x1
    csrrs zero, ustatus, t1

    li t0, 0xFFFF0018
    lw t1, 0(t0)
    lw t2, t
    add t1, t1, t2
    li t0, 0xFFFF0020
    sw t1, 0(t0)

loop:
    wfi
    lw t0, count
    lw t1, m
    blt t0, t1, loop

    li a7, 10
    ecall

handler:
    addi sp, sp, -4
    sw t0, 0(sp)

    li t0, 0xFFFF0018
    lw t1, 0(t0)
    lw t2, t
    add t1, t1, t2
    li t0, 0xFFFF0020
    sw t1, 0(t0)

    la t0, count
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)

    lw t0, 0(sp)
    addi sp, sp, 4
    uret
