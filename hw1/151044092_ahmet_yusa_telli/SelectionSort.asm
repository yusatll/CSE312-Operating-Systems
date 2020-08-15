#########################################
#       CSE 312 Operating Systems       #
#       Homework 01                     #
#       SelectionSort.asm               #
#       Ahmet Yusa Telli                #
#       151044092                       #
#########################################


.data
listSize:  .asciiz "Enter size of list : "
sorted:    .asciiz "The sorted elements :"
        .align 2        
space:  .asciiz " "
            .align 2       
comma:  .asciiz ","
            .align 2        
arr:        .space 80      
.text
.globl SelectionSort


SelectionSort:
    li $v0, 4       
    la $a0, listSize
    syscall

    
    addi $v0, $zero, 5     
    syscall             
    add $s1, $zero, $v0         # $s1 = len


    la $s0, arr            

    add $a0, $zero, $s0    
    add $a1, $zero, $s1    


    jal save

    
    jal sort

    
    jal print   

    
    addi $v0, $zero, 10    
    syscall             

#----------------------------


save:
    addi $sp, $sp, -8       # stack yer al
    sw $s0, 0($sp)          # s0 ve s1 i stack tut
    sw $s1, 4($sp)          
    addi $t0, $zero, 0      
    add $s0, $zero, $t0     # $s0 = $t0 = 0

stuf:
    slt $t1, $s0, $a1       
    beq $t1, $zero, sreturn     

    addi $v0, $zero 5       
    syscall             

    addi $s1, $zero, 0      
    add $s1, $zero, $v0        

    add $t3, $zero, $s0         
    sll $t3, $t3, 2        
    add $t3, $t3, $a0       
    sw $s1, 0($t3)          

    addi $s0, $s0, 1        

    j   stuf

sreturn:
    lw $s0, 0($sp)          
    lw $s1, 4($sp)          
    addi $sp, $sp 8         
    jr $ra              

# -----sort--------------

sort:
    addi $sp, $sp, -28      
    sw $a0, 0($sp)          
    sw $a1, 4($sp)         
    sw $ra, 8($sp)          
    sw $s0, 12($sp)         
    sw $s1, 16($sp)         
    sw $s2, 20($sp)         
    sw $s3, 24($sp)        

    addi $s0, $zero, 0      
    add $t0, $zero, $a1     
    addi $t0, $t0, -1     



loop1:
    slt $t1, $s0, $t0       
    beq $t1, $zero, sort_and     

    add $s3, $zero, $s0         
    addi $t1, $s0, 1        
    add $s1, $zero, $t1         



loop2:
    slt $t1, $s1, $a1       
    beq $t1, $zero, if1       



if2: 
    
    add $t2, $zero, $s1         
    sll $t2, $t2, 2         
    add $t2, $t2, $a0       
    lw $t3, 0($t2)          

     
    add $t4, $zero, $s3      
    sll $t4, $t4, 2         
    add $t4, $t4, $a0       
    lw $t5, 0($t4)          

    slt $t1, $t3, $t5       
    beq $t1, $zero, loop3  
    add $s3, $zero, $s1     

loop3:
    addi $s1, $s1, 1        
    j loop2

if1:
    beq $s3, $s0, loop4    

    
    add $t2, $zero, $s3     
    sll $t2, $t2, 2         
    add $t2, $t2, $a0       
    lw $s2, 0($t2)          


    add $t3, $zero, $s0     
    sll $t3, $t3, 2         
    add $t3, $t3, $a0       
    lw $t6, 0($t3)          

    sw $t6, 0($t2)          

    
    sw $s2, 0($t3)          



loop4:
    addi $s0, $s0, 1        
    j loop1


sort_and:
    lw $a0, 0($sp)          
    lw $a1, 4($sp)          
    lw $ra, 8($sp)          
    lw $s0, 12($sp)         
    lw $s1, 16($sp)        
    lw $s2,  20($sp)       
    lw $s3, 24($sp)        
    addi $sp, $sp 28       
    jr $ra              



print:
    addi $sp, $sp, -4   
    sw $s0, 0($sp)      
    addi $t0, $zero, 0  
    add $s0, $zero, $t0   

    
    la $s3, sorted        
    add $a0, $zero, $s3   
    addi $v0, $zero, 4    
    syscall             

    
    la $s3, space        
    add $a0, $zero, $s3  
    addi $v0, $zero, 4 
    syscall            



printLoop:
    slt $t2, $s0, $a1  
    beq $t2, $zero, print_last  

    la $a0, arr             

    add $t3, $zero, $s0     
    sll $t3, $t3, 2         
    add $t3, $t3, $a0       

    lw $t4, 0($t3)          
    add $a0, $zero, $t4     

    addi $v0, $zero, 1      
    syscall             


    addi $t3, $a1, -1      
    beq $t3, $s0, print_last  



    la $s3, comma           
    add $a0, $zero, $s3     
    addi $v0, $zero, 4    
    syscall           



    la $s3, space          
    add $a0, $zero, $s3    
    addi $v0, $zero, 4     
    syscall             

    addi $s0, $s0, 1    

    j printLoop



print_last:
    lw $s0, 0($sp)         
    addi $sp, $sp 4  
    j main