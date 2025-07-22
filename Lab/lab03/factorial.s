.globl factorial

.data
n: .word 8

.text
main:
    la t0, n # Load address of n
    lw a0, 0(t0) # Load value of n into a0
    jal ra, factorial # Call factorial function

    addi a1, a0, 0 # Store result in a1
    addi a0, x0, 1 # Prepare for print
    ecall # Print Result

    addi a1, x0, '\n' # Prepare for newline
    addi a0, x0, 11 # Print character syscall
    ecall # Print newline

    addi a0, x0, 10 # Prepare for exit
    ecall # Exit

factorial:
    # YOUR CODE HERE
    beq, a0, zero, exit
    # else: # Recursive case
    addi t0, a0, 0
    addi a0, a0, -1     # Prepare for next call of factorial
    addi sp, sp, -8
    sw t0, 0(sp)
    sw ra, 4(sp)
    jal factorial # Recursive call to factorial
    lw t0, 0(sp) # Restore t0
    lw ra, 4(sp) # Restore return address
    addi sp, sp, 8      # Restore sp
    mul a0, t0, a0 # Multiply result with n
    ret  # Return to caller
exit:
    addi a0, a0, 1      # Return 1, base case for factorial
    ret # Return to caller