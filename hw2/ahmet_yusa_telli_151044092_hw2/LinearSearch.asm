#####################################
#       CSE 312 Operating Systems   #
#       Homework 02                 #
#       LinearSearch.asm            #
#       Ahmet Yusa Telli            #
#       151044092                   #
#####################################

.data

search: .asciiz "Linear Search \nValues : 5, 7, 12, 48, 1, 44, 33, 17\nEnter a number for Linear Search: "

find: .asciiz "\nThe Number found at index: "
nfind: .asciiz "\nThe Number NOT FOUND\n"
LSexit: .asciiz "Linear Search END "

array: .word 5, 7, 12, 48, 1, 44, 33, 17
newline: .asciiz "\n"

.text
.globl LinearSearch.asm

LinearSearch.asm:

	la $s1, array

	li $t7, 0
	li $t2, 1
	li $t3, 8   # number of elements 

	li $v0, 4
	la $a0, search
	syscall

	li $v0, 4
	la $a0, newline
	syscall


	li $v0, 23
	syscall

 

searchLoop:
	beq $t7, $t3, nfound
	lw $t4, 0($s1)

	beq $t6, $t4, found

	addi $s1, $s1, 4
	addi $t7, $t7, 1

	j searchLoop

found:
	li $v0, 4
	la $a0, find
	syscall

	li $v0, 1
	move $a0, $t7
	syscall

	li $v0, 4
	la $a0, newline
	syscall

	j finish

nfound:
	beq $t5, $t2, finish

	li $v0, 4
	la $a0, nfind
	syscall

	li $v0, 4
	la $a0, newline
	syscall


finish:
	li $v0,20
	la $a0,LSexit
	syscall
