.globl main

.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text
fun:
    addi t0, a0, 1 # Increment a0 by 1, save to t0
    sub t1, x0, a0 # Subtract a0 from 0, save to t1
    mul a0, t0, t1 # Multiply t0 and t1, use a0 for returning from fun
    jr ra # Return to caller, ra is the return address

main:
    # BEGIN PROLOGUE
    addi sp, sp, -20 # Allocate stack space: 5 words
    sw s0, 0(sp) # Save s0
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw ra, 16(sp) # Save return address
    # END PROLOGUE
    addi t0, x0, 0 #s0 is the sum; t0 is k
    addi s0, x0, 0 # Initialize sum to 0
    la s1, source # Load address of source array
    la s2, dest # Load address of dest array
loop:
    slli s3, t0, 2 #t0=k; s3=k*4 represents the offset for word access
    add t1, s1, s3 # t1 = address of source[k]
    lw t2, 0(t1) # Load source[k] into t2
    beq t2, x0, exit
    add a0, x0, t2 # Prepare argument for fun, a0 = source[k]
    addi sp, sp, -8 # Allocate stack space for fun
    sw t0, 0(sp) # Save t0 (k) to stack
    sw t2, 4(sp) # Save t2 (source[k]) to stack
    jal fun # Call fun with source[k], a0 as argument
    lw t0, 0(sp) # Restore t0 (k) from stack
    lw t2, 4(sp) # Restore t2 (source[k]) from stack
    addi sp, sp, 8 # Deallocate stack space for fun
    add t2, x0, a0 # t2 now holds the result from fun
    add t3, s2, s3 # t3 = address of dest[k]
    sw t2, 0(t3) # Store result in dest[k]
    add s0, s0, t2 # Add result to sum (s0)
    addi t0, t0, 1 # Increment k
    jal x0, loop # Repeat loop, back to the begining of the loop
exit:
    add a0, x0, s0
    # BEGIN EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    # END EPILOGUE
    jr ra # Return to caller, ra is the return address
