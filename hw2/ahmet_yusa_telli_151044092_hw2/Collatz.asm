#####################################
#       CSE 312 Operating Systems   #
#       Homework 02                 #
#       Collatz.asm		            #
#       Ahmet Yusa Telli            #
#       151044092                   #
#####################################

.data

text: .asciiz "Collatz \nEnter a number: "
CEnd: .asciiz "Collatz END\n"

space: .asciiz " "
newline: .asciiz "\n"
.text
.globl Collatz.asm

Collatz.asm:
	li $t2, 1
	li $t4, 2
	li $t3, 3

	# print text
	li $v0, 4
	la $a0, text
	syscall

	# read int
	li $v0, 23
	syscall


collt:
	# div by 2
	beq $t6, $t2, finish	# num == 1 exit
	div $t6, $t4
	mfhi $s0			# kalan = hi
	mflo $s1			# bölüm = lo
	beqz $s0, even
	bgtz $s0, odd


even:
	move $t6, $s1

	li $v0, 1
	move $a0, $t6
	syscall

	li $v0, 4
    la $a0, space
    syscall

	j collt

odd:
	mult $t6, $t3	# num * 3
	mflo $s1
	addi $s1, $s1, 1	# s1 = s1 + 1
	move $t6, $s1		# t6 = s1

	# print
	li $v0, 1
	move $a0, $t6
	syscall


	li $v0, 4
    la $a0, space
    syscall

	j collt


finish:
	li $v0, 4
    la $a0, newline
    syscall

	li $v0, 20
	la $a0, CEnd
	syscall
