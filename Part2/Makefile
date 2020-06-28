all: asm

clear:
	rm asm

asm: table.h table.c mips_funct_asm.h mips_funct_asm.c mips_assembler.c
	gcc -std=c99 -pedantic -g -o asm table.c mips_funct_asm.c mips_assembler.c
	
