.globl c_entry
c_entry: nop
##############################################
##      ADD NON-BRANCHING CODE HERE         ##
## (YOU SHOULD NOT MAKE ANY ASSUMPTIONS     ##
##  ABOUT THE INITIAL VALUES OF REGISTERS)  ##
##############################################

# We only need to modify $s0, $s1, and $t3, since those registers are the
# only ones affecting the program, all other registers will be set by the
# the result of using these registers 
addi $s0, $zero, 1
addi $s1, $zero, $zero
addi $t3, $zero, 2

addu $t2, $t3, $16
slt $t2, $17, $t2
beq $t2, $0, L5
nop
j L6
nop

L7: nop
addu $t3, $t3, $t3
addiu $16, $16, -1

L6: nop
slt $t4, $zero, $16
bne $t4, $zero, L7

nop
sll	$t2, $17, 1
slt	$t2, $t2, $t3
beq	$t2, $0, L8
nop
j win
nop
j L10
nop

L8: nop
j lose
nop
j L10
nop

L5: j lose
nop

L10: j done
nop

win: nop
addi $t0, $zero, 0xAB
j L10

lose: nop
addi $t0, $zero, 0xCD
j L10

.globl done
done: nop
