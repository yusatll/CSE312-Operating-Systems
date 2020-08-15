.data

LS: .asciiz "LinearSearch.asm"
BS: .asciiz "BinarySearch.asm"
Collatz: .asciiz "Collatz.asm"


ProgCounter1: .word 0
ProgCounter2: .word 0
ProgCounter3: .word 0

Prog1Exit: .word 0
Prog2Exit: .word 0
Prog3Exit: .word 0

Prog1Flag: .word 0
Prog2Flag: .word 0
Prog3Flag: .word 0


RunningProcess: .word 1 

.text
.globl main

main:

	li $v0, 18
	la $a0, Collatz
	syscall
	sw $a3, ProgCounter1

	li $v0, 18
	la $a0, LS
	syscall
	sw $a3, ProgCounter2

	li $v0, 18
	la $a0, BS
	syscall
	sw $a3, ProgCounter3


######### Prog 1 ############
Prog1Load:
	li $t0, 1
	sw $t0, RunningProcess

	la $t1, 0x10011000
	lw $t0, Prog1Flag
	bnez $t0, RegLoad
	j Prog1Run

Prog1Return:     
	lw $t0,Prog1Flag
	addi $t0, $t0, 1
	sw $t0, Prog1Flag
	j Prog2Load

Prog1Run:
	li $a0,20
	lw $v0,Prog1Exit
	beq $a0, $v0, ProgCheck 

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter1
	li $v0, 19
	syscall
	sw $a3, ProgCounter1

	sw $v0, Prog1Exit

	j RegStore




######### Prog 2 ############

Prog2Load:
	li $t0, 2
	sw $t0, RunningProcess

	la $t1, 0x10011100
	lw $t0, Prog2Flag
	bnez $t0, RegLoad
	j Prog2Run

Prog2Return:     
	lw $t0,Prog2Flag
	addi $t0, $t0, 1
	sw $t0, Prog2Flag
	j Prog3Load

Prog2Run:
	
	li $a0,20
	lw $v0,Prog2Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter2
	li $v0, 19
	syscall
	sw $a3, ProgCounter2
	
	sw $v0, Prog2Exit

	j RegStore



######### Prog 3 ############

Prog3Load:
	li $t0, 3
	sw $t0, RunningProcess

	la $t1, 0x10011200
	lw $t0, Prog3Flag
	bnez $t0, RegLoad
	j Prog3Run

Prog3Return:     
	lw $t0,Prog3Flag
	addi $t0, $t0, 1
	sw $t0, Prog3Flag
	j ProgCheck

Prog3Run:
	
	li $a0,20
	lw $v0,Prog3Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter3
	li $v0, 19
	syscall
	sw $a3, ProgCounter3
	
	sw $v0, Prog3Exit

	j RegStore


######### Others ###########

ProgCheck:
	lw $t0,Prog1Exit
	li $t1,20    
	beq $t0,$t1,jump1

	j Prog1Load

jump1:
	lw $t0,Prog2Exit
	li $t1,20    
	beq $t0,$t1,jump2

	j Prog2Load


jump2:
	lw $t0,Prog3Exit
	li $t1,20    
	beq $t0,$t1,finish

	j Prog3Load



RegLoad:
	lw $t0, 132($t1)
	mthi $t0 
	lw $t0, 136($t1)
	mtlo $t0

	lw $v0, 12($t1)
	lw $v1, 16($t1)
	lw $a0, 20($t1)
	lw $a1, 24($t1)
	lw $a2, 28($t1)
	lw $a3, 32($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)
	lw $t3, 48($t1)
	lw $t4, 52($t1)
	lw $t5, 56($t1)
	lw $t6, 60($t1)
	lw $t7, 64($t1)
	lw $s0, 68($t1)
	lw $s1, 72($t1)
	lw $s2, 76($t1)
	lw $s3, 80($t1)
	lw $s4, 84($t1)
	lw $s5, 88($t1)
	lw $s6, 92($t1)
	lw $s7, 96($t1)
	lw $t8, 100($t1)
	lw $t9, 104($t1)
	lw $k0, 108($t1)
	lw $k1, 112($t1)
	lw $gp, 116($t1)
	lw $sp, 120($t1)
	lw $s8, 124($t1)
	lw $ra, 128($t1)


	lw $t0, RunningProcess
	li $t2, 1
	beq $t0, $t2, Prog1Run
	li $t2, 2
	beq $t0, $t2, Prog2Run
	li $t2, 3
	beq $t0, $t2, Prog3Run

RegStore:
	mfhi $t0
	sw $t0, 132($t1)
	mflo $t0
	sw $t0, 136($t1)

	sw $v0, 12($t1)
	sw $v1, 16($t1)
	sw $a0, 20($t1)
	sw $a1, 24($t1)
	sw $a2, 28($t1)
	sw $a3, 32($t1)
	sw $t0, 36($t1)
	sw $t2, 44($t1)
	sw $t3, 48($t1)
	sw $t4, 52($t1)
	sw $t5, 56($t1)
	sw $t6, 60($t1)
	sw $t7, 64($t1)
	sw $s0, 68($t1)
	sw $s1, 72($t1)
	sw $s2, 76($t1)
	sw $s3, 80($t1)
	sw $s4, 84($t1)
	sw $s5, 88($t1)
	sw $s6, 92($t1)
	sw $s7, 96($t1)
	sw $t8, 100($t1)
	sw $t9, 104($t1)
	sw $k0, 108($t1)
	sw $k1, 112($t1)
	sw $gp, 116($t1)
	sw $sp, 120($t1)
	sw $s8, 124($t1)
	sw $ra, 128($t1)



	lw $t0, RunningProcess
	li $t2, 1
	beq $t0, $t2, Prog1Return
	li $t2, 2
	beq $t0, $t2, Prog2Return
	li $t2, 3
	beq $t0, $t2, Prog3Return


finish:   
	li $v0,21	# mk exit
	syscall
