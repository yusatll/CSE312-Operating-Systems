## choosing 3 out 4 programs randomly 
## and loading each program 3 times

.data

LS:   .asciiz "LinearSearch.asm"
BS:   .asciiz "BinarySearch.asm"
Collatz:   .asciiz "Collatz.asm"
Palindrome: .asciiz "Palindrome.asm"


Prog0Exit: .word 0
Prog1Exit: .word 0
Prog2Exit: .word 0
Prog3Exit: .word 0
Prog4Exit: .word 0
Prog5Exit: .word 0
Prog6Exit: .word 0
Prog7Exit: .word 0
Prog8Exit: .word 0

ProgCounter0: .word 0
ProgCounter1: .word 0
ProgCounter2: .word 0
ProgCounter3: .word 0
ProgCounter4: .word 0
ProgCounter5: .word 0
ProgCounter6: .word 0
ProgCounter7: .word 0
ProgCounter8: .word 0

Prog0Flag: .word 0
Prog1Flag: .word 0
Prog2Flag: .word 0
Prog3Flag: .word 0
Prog4Flag: .word 0
Prog5Flag: .word 0
Prog6Flag: .word 0
Prog7Flag: .word 0
Prog8Flag: .word 0

RunningProcess: .word 10

.text
.globl main

main:
	# load
	li $v0, 25
	syscall
	li $s0, 0
	li $s1, 1
	li $s2, 2
	li $s3, 3

	# choose first prog
loadSec1:
	beq $s6, $s0, loadLinear1
	beq $s6, $s1, loadBinary1
	beq $s6, $s2, loadCollatz1
	beq $s6, $s3, loadPali1





	# select first prog
loadLinear1:
	li $v0, 30
	la $a0, LS
	syscall
	la $k1, LS

	sw $a3, ProgCounter0
	sw $a3, ProgCounter1
	sw $a3, ProgCounter2

	j loadSec2

loadBinary1:
	li $v0, 30
	la $a0, BS
	syscall
	la $k1, BS

	sw $a3, ProgCounter0
	sw $a3, ProgCounter1
	sw $a3, ProgCounter2

	j loadSec2

loadCollatz1:
	li $v0, 30
	la $a0, Collatz
	syscall
	la $k1, Collatz

	sw $a3, ProgCounter0
	sw $a3, ProgCounter1
	sw $a3, ProgCounter2

	j loadSec2


loadPali1:
	li $v0, 30
	la $a0, Palindrome
	syscall
	la $k1, Palindrome

	sw $a3, ProgCounter0
	sw $a3, ProgCounter1
	sw $a3, ProgCounter2

	j loadSec2




	# choose second prog
loadSec2:
	beq $s7, $s0, loadLinear2
	beq $s7, $s1, loadBinary2
	beq $s7, $s2, loadCollatz2
	beq $s7, $s3, loadPali2

	# select second prog
loadLinear2:
	li $v0, 30
	la $a0, LS
	syscall
	la $k1, LS

	sw $a3, ProgCounter3
	sw $a3, ProgCounter4
	sw $a3, ProgCounter5

	j loadSec3

loadBinary2:
	li $v0, 30
	la $a0, BS
	syscall
	la $k1, BS

	sw $a3, ProgCounter3
	sw $a3, ProgCounter4
	sw $a3, ProgCounter5

	j loadSec3

loadCollatz2:
	li $v0, 30
	la $a0, Collatz
	syscall
	la $k1, Collatz

	sw $a3, ProgCounter3
	sw $a3, ProgCounter4
	sw $a3, ProgCounter5

	j loadSec3


loadPali2:
	li $v0, 30
	la $a0, Palindrome
	syscall
	la $k1, Palindrome

	sw $a3, ProgCounter3
	sw $a3, ProgCounter4
	sw $a3, ProgCounter5

	j loadSec3






	# choose third prog
loadSec3:
	beq $t8, $s0, loadLinear3
	beq $t8, $s1, loadBinary3
	beq $t8, $s2, loadCollatz3
	beq $t8, $s3, loadPali3
	
# select third prog
loadLinear3:
	li $v0, 30
	la $a0, LS
	syscall
	la $k1, LS

	sw $a3, ProgCounter6
	sw $a3, ProgCounter7
	sw $a3, ProgCounter8

	j cont

loadBinary3:
	li $v0, 30
	la $a0, BS
	syscall
	la $k1, BS

	sw $a3, ProgCounter6
	sw $a3, ProgCounter7
	sw $a3, ProgCounter8

	j cont

loadCollatz3:
	li $v0, 30
	la $a0, Collatz
	syscall
	la $k1, Collatz

	sw $a3, ProgCounter6
	sw $a3, ProgCounter7
	sw $a3, ProgCounter8

	j cont


loadPali3:
	li $v0, 30
	la $a0, Palindrome
	syscall
	la $k1, Palindrome

	sw $a3, ProgCounter6
	sw $a3, ProgCounter7
	sw $a3, ProgCounter8

	j cont








cont:
#################### Prog 0 ########
Prog0Load:
	li $t0, 10
	sw $t0, RunningProcess

	la $t1, 0x10011000
	lw $t0, Prog0Flag
	bnez $t0, RegLoad
	j Prog0Run

Prog0Return:     
	lw $t0,Prog0Flag
	addi $t0, $t0, 1
	sw $t0, Prog0Flag
	j Prog1Load

Prog0Run:
	li $a0,40
	lw $v0,Prog0Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter0
	li $v0, 19
	syscall
	sw $a3, ProgCounter0

	sw $v0, Prog0Exit

	j RegStore





#################### Prog 1 ########
Prog1Load:
	li $t0, 1
	sw $t0, RunningProcess

	la $t1, 0x10011100
	lw $t0, Prog1Flag
	bnez $t0, RegLoad
	j Prog1Run


Prog1Return:     
	lw $t0,Prog1Flag
	addi $t0, $t0, 1
	sw $t0, Prog1Flag
	j Prog2Load

Prog1Run:
	li $a0,40
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



	#################### Prog 2 ########
Prog2Load:
	li $t0, 2
	sw $t0, RunningProcess

	la $t1, 0x10011200
	lw $t0, Prog2Flag
	bnez $t0, RegLoad
	j Prog2Run

Prog2Return:     
	lw $t0,Prog2Flag
	addi $t0, $t0, 1
	sw $t0, Prog2Flag
	j Prog3Load

Prog2Run:
	li $a0,40
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




	#################### Prog 3 ########
Prog3Load:
	li $t0, 3
	sw $t0, RunningProcess

	la $t1, 0x10011300
	lw $t0, Prog3Flag
	bnez $t0, RegLoad

Prog3Run:
	li $a0,40
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


Prog3Return:     
	lw $t0,Prog3Flag
	addi $t0, $t0, 1
	sw $t0, Prog3Flag

	#################### Prog 4 ########
Prog4Load:
	li $t0, 4
	sw $t0, RunningProcess

	la $t1, 0x10011400
	lw $t0, Prog4Flag
	bnez $t0, RegLoad

Prog4Run:
	li $a0,40
	lw $v0,Prog4Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter4
	li $v0, 19
	syscall
	sw $a3, ProgCounter4

	sw $v0, Prog4Exit

	j RegStore


Prog4Return:     
	lw $t0,Prog4Flag
	addi $t0, $t0, 1
	sw $t0, Prog4Flag

	#################### Prog 5 ########
Prog5Load:
	li $t0, 5
	sw $t0, RunningProcess

	la $t1, 0x10011500
	lw $t0, Prog5Flag
	bnez $t0, RegLoad

Prog5Run:
	li $a0,40
	lw $v0,Prog5Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter5
	li $v0, 19
	syscall
	sw $a3, ProgCounter5

	sw $v0, Prog5Exit

	j RegStore


Prog5Return:     
	lw $t0,Prog5Flag
	addi $t0, $t0, 1
	sw $t0, Prog5Flag




	#################### Prog 6 ########
Prog6Load:
	li $t0, 6
	sw $t0, RunningProcess

	la $t1, 0x10011600
	lw $t0, Prog6Flag
	bnez $t0, RegLoad

Prog6Run:
	li $a0,40
	lw $v0,Prog6Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter6
	li $v0, 19
	syscall
	sw $a3, ProgCounter6

	sw $v0, Prog6Exit

	j RegStore


Prog6Return:     
	lw $t0,Prog6Flag
	addi $t0, $t0, 1
	sw $t0, Prog6Flag


		#################### Prog 7 ########
Prog7Load:
	li $t0, 7
	sw $t0, RunningProcess

	la $t1, 0x10011700
	lw $t0, Prog7Flag
	bnez $t0, RegLoad

Prog7Run:
	li $a0,40
	lw $v0,Prog7Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter7
	li $v0, 19
	syscall
	sw $a3, ProgCounter7

	sw $v0, Prog7Exit

	j RegStore


Prog7Return:     
	lw $t0,Prog7Flag
	addi $t0, $t0, 1
	sw $t0, Prog7Flag


		#################### Prog 8 ########
Prog8Load:
	li $t0, 8
	sw $t0, RunningProcess

	la $t1, 0x10011800
	lw $t0, Prog8Flag
	bnez $t0, RegLoad

Prog8Run:
	li $a0,40
	lw $v0,Prog8Exit
	beq $a0, $v0, ProgCheck

	lw $a0, 20($t1)
	lw $t0, 36($t1)
	lw $t2, 44($t1)

	lw $a3, ProgCounter8
	li $v0, 19
	syscall
	sw $a3, ProgCounter8

	sw $v0, Prog8Exit

	j RegStore


Prog8Return:     
	lw $t0,Prog8Flag
	addi $t0, $t0, 1
	sw $t0, Prog8Flag



######## Check Programs ######

ProgCheck:
	lw $t0,Prog0Exit
	li $t1,40    
	beq $t0,$t1,jump1

	j Prog0Load


jump1:
	lw $t0,Prog1Exit
	li $t1,40    
	beq $t0,$t1,jump2

	j Prog1Load

jump2:
	lw $t0,Prog2Exit
	li $t1,40    
	beq $t0,$t1,jump3

	j Prog2Load


jump3:
	lw $t0,Prog3Exit
	li $t1,40    
	beq $t0,$t1,jump4

	j Prog3Load

jump4:
	lw $t0,Prog4Exit
	li $t1,40    
	beq $t0,$t1,jump5

	j Prog4Load

jump5:
	lw $t0,Prog5Exit
	li $t1,40    
	beq $t0,$t1,jump6

	j Prog5Load

jump6:
	lw $t0,Prog4Exit
	li $t1,40    
	beq $t0,$t1,jump7

	j Prog6Load

jump7:
	lw $t0,Prog4Exit
	li $t1,40    
	beq $t0,$t1,jump8

	j Prog7Load


jump8:
	lw $t0,Prog4Exit
	li $t1,40    
	beq $t0,$t1,finish

	j Prog8Load	


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
	li $t2, 10
	beq $t0, $t2, Prog0Run
	li $t2, 1
	beq $t0, $t2, Prog1Run
	li $t2, 2
	beq $t0, $t2, Prog2Run
	li $t2, 3
	beq $t0, $t2, Prog3Run
	li $t2, 4
	beq $t0, $t2, Prog4Run
	li $t2, 5
	beq $t0, $t2, Prog5Run
	li $t2, 6
	beq $t0, $t2, Prog6Run
	li $t2, 7
	beq $t0, $t2, Prog7Run
	li $t2, 8
	beq $t0, $t2, Prog8Run


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
	li $t2, 10
	beq $t0, $t2, Prog0Return
	li $t2, 1
	beq $t0, $t2, Prog1Return
	li $t2, 2
	beq $t0, $t2, Prog2Return
	li $t2, 3
	beq $t0, $t2, Prog3Return
	li $t2, 4
	beq $t0, $t2, Prog4Return
	li $t2, 5
	beq $t0, $t2, Prog5Return
	li $t2, 6
	beq $t0, $t2, Prog6Return
	li $t2, 7
	beq $t0, $t2, Prog7Return
	li $t2, 8
	beq $t0, $t2, Prog8Return

finish:
	li $v0,21	# mk exit
	syscall
