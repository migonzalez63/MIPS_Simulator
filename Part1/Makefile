all: sim

clear: 
	rm sim

sim: table.h table.c mips_functions.h mips_functions.c mips_sim.c
	gcc -std=c99 -pedantic -g -o sim table.c mips_functions.c mips_sim.c
	
