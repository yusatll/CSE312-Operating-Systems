#########################################
#       CSE 312 Operating Systems       #
#       Homework 03                     #
#       BinarySearch.asm                #
#       Ahmet Yusa Telli                #
#       151044092                       #
#########################################

.data
.align 2

start: .asciiz "-> Binary Search START <- \nValues : 4, 5, 7, 8, 9, 11, 13, 14, 15, 16 \nEnter a number for Binary Search : "

find: .asciiz "\nThe Number FOUND\n"
nfind: .asciiz "\nThe Number NOT FOUND\n"
BSEnd: .asciiz "-> Binary Search END <-\n"
newline: .asciiz "\n"

array: .word 4, 5, 7, 8, 9, 11, 13, 14, 15, 16
.text 
.globl BinarySearch.asm

BinarySearch.asm:
	la $s1, array
	li $s7, 9    # number of elements 
	li $s6, 2	 # bolme
	li $s5, 0	 # counter
	li $s4, 0
	li $s2, 4	 # address 4
	li $s3, 5	 # limit = size / 2 + 1

	li $v0, 4
	la $a0, start
	syscall

	li $v0, 23		# READ TO t6
	syscall




searchMid:
	beq $s5, $s3, nfound	# sonuna geldik mi
	div $s7, $s7, $s6		# size / 2 ve bolum = s3 --- s3 s7 olmalı
	mul $t4, $s2, $s7		# t4 = size/2 * 4

	
	add $s1, $s1, $t4		# s1 = t4
	lw $s4, 0($s1)

	addi $s5, $s5, 1
	
	beq $s4, $t6, found 	# bulduk mu
	bgt $s4, $t6, decr		# s4 > t6 jump
	ble $s4, $t6, incr		# s4 < t6 jump
	j searchMid



decr:
	sub $s1, $s1, $t4
	mul $t4, $t4, $zero

	j searchMid

incr:
	addi $s7, $s7, 1
	j searchMid

found:
	li $v0, 4
	la $a0, find
	syscall
	j finish


nfound:
	li $v0, 4
	la $a0, nfind
	syscall
	j finish

	

finish:
	li $v0, 40
	la $a0, BSEnd
	syscall