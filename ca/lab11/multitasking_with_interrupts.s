.data
thread1_pc:   .word 0
thread1_val:  .word 0
thread2_pc:   .word 0
thread2_val:  .word 0
current_thread: .word 0
n:           .word 5
t:           .word 500

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

    jal thread1

thread1:
    li t0, 0
    la t1, n
    lw t1, 0(t1)

thread1_loop:
    mv a0, t0
    li a7, 1
    ecall

    addi t0, t0, 1
    blt t0, t1, thread1_loop

    jal exit

thread2:
    li t0, 0
    la t1, n
    lw t1, 0(t1)

thread2_loop:
    mv a0, t0
    li a7, 1
    ecall

    addi t0, t0, 1
    blt t0, t1, thread2_loop

    jal exit

handler:
    addi sp, sp, -16
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)

    la t0, current_thread
    lw t1, 0(t0)
    li t2, 1
    xor t1, t1, t2
    sw t1, 0(t0)

    li t0, 0xFFFF0018
    lw t1, 0(t0)
    lw t2, t
    add t1, t1, t2
    li t0, 0xFFFF0020
    sw t1, 0(t0)

    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    addi sp, sp, 16

    uret

exit:
    li a7, 10
    ecall
