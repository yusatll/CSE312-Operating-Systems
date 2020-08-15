#       CSE 312 Operating Systems
#       Homework 01
#       shell.asm
#       Ahmet Yusa Telli
#       151044092

.data
prompt: .asciiz "cse312-shell \$ "
address: .space 256
.text
.globl main

main:
	j print_prompt

after_prompt:
    li $v0, 8           # get user input
    la $a0, address		# user input
    syscall

    j fork

    j main


print_prompt:
	li $v0, 4           #print prompt
	la $a0, prompt
	syscall

	j after_prompt


fork:
	li $v0, 18			# fork
	syscall

	j main


final:
    li $v0, 10
    syscall             #end of program
