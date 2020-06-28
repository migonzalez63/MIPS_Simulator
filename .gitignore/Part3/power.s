.globl c_entry
c_entry: nop
#######################################################################
## YOUR MIPS CODE SHOULD APPEAR AFTER THE FOLLOWING TWO INSTRUCTIONS ##
#######################################################################
addi $t1, $zero, 13
addi $t2, $zero, 3

addi $t0, $zero, 1
	
# As long as $t2 != 1, we will multiply the contents in
# register $t1 with itself.
loop: beq $t2, $zero, done
mult $t0, $t1
mflo $t0
addi $t2, $t2, -1
j loop
##############################
j done

.globl done
done: nop
nop
