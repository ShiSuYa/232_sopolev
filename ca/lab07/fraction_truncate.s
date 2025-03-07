    .data
one_double:  .double 1.0
ten_double:  .double 10.0

    .text
    .globl main

main:
    li a7, 5
    ecall
    mv t0, a0

    li a7, 5
    ecall
    mv t1, a0

    li a7, 5
    ecall
    mv t2, a0

    fcvt.d.w fa1, t0
    fcvt.d.w fa2, t1
    fdiv.d fa0, fa1, fa2

    mv a0, t2
    jal ra, frac_round

    li a7, 3
    ecall

    li a7, 10
    ecall

frac_round:
    la t3, one_double
    fld ft0, 0(t3)
    mv t0, a0
multiplier_loop:
    beqz t0, done_multiplier
    la t3, ten_double
    fld ft1, 0(t3)
    fmul.d ft0, ft0, ft1
    addi t0, t0, -1
    j multiplier_loop
done_multiplier:
    fmul.d ft1, fa0, ft0
    fcvt.w.d t1, ft1
    fcvt.d.w ft1, t1
    fdiv.d fa0, ft1, ft0
    ret
