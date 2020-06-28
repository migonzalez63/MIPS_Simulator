# team07

Gonzalez, Miguel Angel <migonzalez@unm.edu>

All code written out is in C.

Part 1 and 2 will use the table.c and table.h files. Files found in each folder
  * table.c and table.h implement a table in which we can store a name  
    and value. It will also allow us to look up the item in the table.

Makefile will compile both Part 1 and 2 with the given names in the instructions,  
sim and asm, respectively. Makefile found on each folder

# Part  1
mips_sim.c, mips_functions.c and mips_functions.h compromise the
files for Part 1.
 * mips_sim.c reads in a file and saves the labels in the simulated memory  
    then it parses through each individual instruction and saves the inputs  
    necessary for each instruction in order to be exeuted. Labels will have 
    "addresses" saved to them in order to use the jump instructions.  
    The "addresses" will be the byte location in the file after the label is  
    defined. Thus, when we jump to a label, we will be jumping to its  
    location on file. and re-read the corresponding instructions.
 * mips_functions.h defines the 18 instructions that our simulator should  
    should be able to handle. mips_functions.c implements the proper  
    operations needed to execute the instructions and the proper inputs  
    needed.


# Part 2
mips_assembler.c, mips_funct_asm.h, and mips_funct_asm.c compromise the files  
for Part 2.
* mips_assmebler.c reads in a file and computes the address of labels (multiples
  of 4) and computes the appropiate format and opCode for each instruction. Will  
  save the addresses of each label in a table. Once we have finished computing  
  the machine instruction, it will print out a hexadecimal representation of it.
* mips_funct_asm.h defines the proper formats and mips_funct_asm.c implements
  the formats and applies them to their appropiate instructions.

# Part 3
power.s, power-qemu.txt, and power-sim.txt compromise the files for Part 3
* power.s modifies the given assembly file in order to take one register value  
  and raise it to the second registers power.
* power-qemu.txt holds the register contenst after running power.s on the qemu 
  mips simulator.
* power-sim.txt holds the register contents after running power.s on our  
  mips_sim program.

# Part 4
sort1.c, sort2.c, and sort-comparison.txt compromise the files for Part 4
* sort1.c contains a simple implementation of bubble sort on a hard coded array
* sort2.c contains a modified version of bubble sort on a hard coded array.
* sort-comparison.txt contains the insrtuction counts for sort1 and sort2.

# Part 5
win.s compromises the file for Part 5
* win.s contains a modified version of the given assembly file where we modify
  some registers in order to reach the win label. Looking at the assembly file,
  we see that only three register need to be intialized, $t3, $s0 or $16, and 
  $s1 or $17, since they are the only ones influencing the outcome.

