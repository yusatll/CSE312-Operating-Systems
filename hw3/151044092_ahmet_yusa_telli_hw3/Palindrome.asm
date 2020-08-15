#####################################
#       CSE 312 Operating Systems   #
#       Homework 03                 #
#       Palindrome.asm	            #
#       Ahmet Yusa Telli            #
#       151044092                   #
#####################################


.data
.align 2

start: .asciiz " -> Palindrome START <- \n"
ask1: .asciiz "Enter a string for Palindrome or Not: \n"
find: .asciiz " -> Palindrome\n"
nfind: .asciiz " -> NOT Palindrome\n"
PEnd: .asciiz " -> Palindrome END <- \n"
newline: .asciiz "\n"

values: .asciiz "T8 - T9 "

string: .space 80

.text 
.globl Palindrome.asm

Palindrome.asm:
	li $v0, 4
	la $a0, start
	syscall
	li $t9, 100
	li $t8, 0
	li $t6, 99

	# load answers
	li $s0, 1	# yes
	li $s1, 2	# no

begin:

#	li $v0, 4
#	la $a0, values
#	syscall

#	move $a0, $t8
#	li $v0, 1
#	syscall
	
#	move $a0, $t9
#	li $v0, 1
#	syscall

	bgt $t9, $t8, read1			# 100 >= 0 t8	100 okuma
	beq $t9, $t8, ask_cont		# 100 == 0 t8	tek okuma
	blt $t6, $t8, exit			# 99 <  0 t8	exit



pali:
	add $t8, $t8, 1
	# print string
	#la $a0, string
	#li $v0, 4
	#syscall

	# print size
#	move $a0, $a1
#	li $v0, 1
#	syscall


	# copy string
	la $s7, string		# str1
	la $s6, string		# str2
	li $t7, 0


j go_end_B
	




check:
	bge $s7, $s6, found
	j loadbyte
cont:
	bne $s5, $s4, Nfound
	j forward
cont2:
	j check



forward:
	add $s7, $s7, 1
	sub $s6, $s6, 1
	j cont2



end_B:

	subu $s6, $s6, 1	# remove \0

	#move $a0, $s6
	#li $v0, 4
	#syscall

	j check



# 100 tane read
read1:
	li $v0, 26
	la $a0, string
	syscall

#	la $a0, string
#	li $v0, 4
#	syscall

	j pali


# tek tek read
read2:
#	li $v0, 4
#	la $a0, ask1
#	syscall

	# string oku
	li $v0, 27
	la $a0, string
	syscall
	j pali




go_end_B:
	lb $s5, ($s6)
	beqz $s5, end_B
	add $s6, $s6, 1
	j go_end_B


found:
	la $a0, string
	li $v0, 4
	syscall

	la $a0, find
	li $v0, 4
	syscall 
	j begin


loadbyte:
	lb $s5, ($s7)
	lb $s4, ($s6)
	j cont



Nfound:
	la $a0, string
	li $v0, 4
	syscall

	la $a0, nfind
	li $v0, 4
	syscall 
	j begin


finish:
	bleu $t9, $t8, ask_cont
	j begin



ask_cont:
	li $a1, 0
	li $v0, 28
	syscall

	beq $a1, $s0, read2
	beq $a1, $s1, exit


exit:
	li $v0, 40
	la $a0, PEnd
	syscall


