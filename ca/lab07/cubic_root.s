    .data
zero_double:  .double 0.0
one_double:   .double 1.0
two_double:   .double 2.0

    .text
    .globl main

main:
    li a7, 7
    ecall
    fmv.d fa1, fa0

    li a7, 7
    ecall
    fmv.d fa2, fa0

    la t1, zero_double
    fld ft6, 0(t1)
    flt.d t0, fa1, ft6

    fsgnjx.d ft0, fa1, fa1

    la t1, zero_double
    fld ft1, 0(t1)

    fmv.d ft2, ft0

cuberoot_loop:
    fadd.d ft3, ft1, ft2
    la t1, two_double
    fld ft6, 0(t1)
    fdiv.d ft3, ft3, ft6

    fmul.d ft4, ft3, ft3
    fmul.d ft4, ft4, ft3

    fsub.d ft5, ft4, ft0
    fsgnjx.d ft5, ft5, ft5

    flt.d t1, fa2, ft5
    bnez t1, continue_loop
    j finish

continue_loop:
    flt.d t1, ft4, ft0
    bnez t1, set_low
    fmv.d ft2, ft3
    j cuberoot_loop

set_low:
    fmv.d ft1, ft3
    j cuberoot_loop

finish:
    beqz t0, positive_case
    fneg.d ft3, ft3
positive_case:
    fmv.d fa0, ft3
    li a7, 3
    ecall

    li a7, 10
    ecall
