#########################################
#       CSE 312 Operating Systems		#
#       Homework 01						#
#       ShowDivisibleNumbers.asm	    #
#       Ahmet Yusa Telli				#
#       151044092						#
#########################################

.data

one: .asciiz "Enter min number: "
two: .asciiz "Enter max number: "
three: .asciiz "Enter divide number: "
newline: .word '\n'

first: .space 4
second: .space 4
third: .space 4

.text
.globl ShowDivisibleNumbers

ShowDivisibleNumbers:
	li $v0, 4           
	la $a0, one
	syscall

	# read input
    li $v0, 5	
    syscall
    sw $v0, first	# save to memory


#-----------------------
	li $v0, 4           
	la $a0, two
	syscall

    li $v0, 5           
    syscall
    sw $v0, second	# save to memory
#-----------------------
	li $v0, 4           
	la $a0, three
	syscall

    li $v0, 5           
    syscall
    sw $v0, third	# save to memory


    lw $s7, first
    lw $s6, second
    lw $s5, third



divide:
    sle $s3, $s6, $s7
    bnez $s3, finish
    div $s2, $s7, $s5
    mfhi $s4
    beqz $s4, isDivisible
    

increase:
    addi $s7, $s7, 1
    j divide

isDivisible:
    li $v0, 1
    move $a0, $s7
    syscall

    li $v0, 11
    lw $a0, newline
    syscall

    j increase

finish:
    j main

