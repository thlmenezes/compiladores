li s0, 0
li a7, 5
ecall
mv s0, a0
li s1, 1
WHILE_LOOP_0:
mv t2, s0
beqz t2, WHILE_LOOP_END_0
mul t0, s1, s0
mv s1, t0
li s2, 1
sub t1, s0, s2
mv s0, t1
j WHILE_LOOP_0
WHILE_LOOP_END_0:
mv t2, s1
li a7, 1
mv a0, t2
ecall
