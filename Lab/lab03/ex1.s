.data
.word 2, 4, 6, 8 # stored in .data section
n: .word 9 

.text
main:
    add t0, x0, x0 # t0 = 0 (F(0))
    addi t1, x0, 1 # t1 = 1 (F(1))
    la t3, n # Load address of n into t3
    lw t3, 0(t3) # Load value of n from memory (t3 = 9)
fib:
    beq t3, x0, finish # if t3 == 0, go to finish
    add t2, t1, t0 # t2 = F(n) = t1 + t0, t2 holds F(n)
    mv t0, t1 # t0 = t1 (advance), t0 holds F(n−2)
    mv t1, t2 # t1 = t2 (advance), t1 holds F(n−1)
    addi t3, t3, -1 # decrement counter
    j fib # repeat
finish:
    addi a0, x0, 1 # syscall code for print integer
    addi a1, t0, 0 # result (Fibonacci number) in t0 → a1
    ecall # print a1
    addi a0, x0, 10 # syscall code for program exit
    ecall # terminate ecall # terminate program
