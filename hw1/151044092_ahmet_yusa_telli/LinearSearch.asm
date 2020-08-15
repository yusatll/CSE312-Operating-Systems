#####################################
#       CSE 312 Operating Systems   #
#       Homework 01                 #
#       LinearSearch.asm            #
#       Ahmet Yusa Telli            #
#       151044092                   #
#####################################

.data
search: .asciiz "Values : 5, 7, 12, 48, 1, 44, 33\nEnter a number: "
success: .asciiz "\nThe Number found at index: "
failure: .asciiz "\nThe Number NOT FOUND"

array: .word 5, 7, 12, 48, 1, 44, 33
.text
.globl LinearSearch

LinearSearch:
    la $s1, array    

    li $t1, 0        
    li $t2, 1        
    li $t3, 20       

    li $v0, 4        
    la $a0, search
    syscall

    li $v0, 5        
    syscall

    move $t0, $v0    

    j loop


loop:
    beq $t1, $t3, not_found   

    lw $t4, 0($s1)         

    beq $t0, $t4, found    


checking:

    addi $s1, $s1, 4       
    addi $t1, $t1, 1       

    j loop


found:
    li $v0, 4           
    la $a0, success
    syscall

    li $v0, 1           
    move $a0, $t1
    syscall

    li $t5, 1          

    j checking



not_found:
    beq $t5, $t2, final  

    li $v0, 4           
    la $a0, failure
    syscall

    j final



final:
    j main             
