#########################################
#       CSE 312 Operating Systems       #
#       Homework 01                     #
#       BinarySearch.asm                #
#       Ahmet Yusa Telli                #
#       151044092                       #
#########################################
.data
.align 2

start:      .asciiz "Enter increasing sorted positive integers (0 for exit) : \n"
takeList:     .asciiz "Enter a number (0 for exit) : \n"
findMsg:            .asciiz " - Found\n"
notMsg:             .asciiz " - Not Found\n"

.text
.globl BinarySearch


BinarySearch:
    li          $v0, 4              
    la          $a0, start  
    syscall                         

    li          $v0, 9              
    la          $a0, 4              
    syscall                         
    move        $s1, $v0            

    li          $s4, 0              


createList:
    li          $v0, 5              
    syscall                         
    move        $t1, $v0            
    blez        $v0, initializeList 

    li          $v0, 9              
    la          $a0, 4              
    syscall                         

    li          $t0, 4                
    mul         $t0, $s4, $t0         
    add         $t0, $t0, $s1         
    move        $s5, $t0              
    sw          $t1, ($t0)            
    addi        $s4  $s4, 1           

    j           createList             



initializeList:
    li          $v0, 4                
    la          $a0, takeList   
    syscall                           

    li          $s2, 0                


searching:
    li          $v0, 5                
    syscall                           
    move        $t1, $v0              
    blez        $v0, initSearch       

    li          $v0, 9                
    la          $a0, 4                
    syscall                           

    li          $t0, 4               
    add         $t2, $s4, $s2        
    mul         $t0, $t2, $t0        
    add         $t0, $t0, $s1        
    move        $s3, $t0             
    sw          $t1, ($t0)           
    addi        $s2, $s2, 1          

    j           searching           



initSearch:
    move        $t6, $s5             
    move        $t7, $s3             



search:
    move        $t5, $s5             
    beq         $t7, $t6, exit       


SS:

    move        $s6, $s1             
    move        $s7, $s5             
    move        $t0, $s4             
    move        $t9, $s4             
    li          $v1, 2               
    div         $t9, $v1             
    mflo        $t9                  
    add         $t9, $t9, $v1


highCh:
    li          $v1, 2               
    div         $t0, $v1             
    mflo        $v1                  
    mflo        $t0                  

    blez        $t0, savedStep   

    j           loopCheck            


lowerCh:
    li          $v1, 2               
    div         $t0, $v1             
    mflo        $v1                  
    mflo        $t0                  
    mfhi        $t1                  

    blez        $t0, failS        

    j           loopCheck            


failS:
    blez        $t1, false              


loopCheck:
    beq         $s6, $s7, false         
    blez        $t9, false              

    mul         $v1, $t0, 4          
    add         $t4, $s6, $v1        

    lw          $a1, ($t7)           
    lw          $a2, ($t4)           

    sub         $t9, $t9, 1          

    beq         $a2, $a1, true        
    sub         $t1, $a1, $a2        
    blez        $t1, search1     
    bgez        $t1, search2    


savedStep:
    mfhi        $t8                  
    bgtz        $t8, counter
    j           loopCheck            


counter:
    add         $t0, $v1, $t8        

    j           loopCheck            


search1:
    move        $s7, $t4             

    j           lowerCh           


search2:
    move        $s6, $t4             

    j           highCh          


ReSearch:
    sub         $t7, $t7, 4          

    j           search               


true:
    li          $v0, 1               
    lw          $a0, ($t4)           
    syscall                          

    li          $v0, 4               
    la          $a0, findMsg         
    syscall                          

    j           ReSearch        


false:
    li          $v0, 1                
    lw          $a0, ($t7)            
    syscall                           

    li          $v0, 4                
    la          $a0, notMsg           
    syscall                          

    j           ReSearch        



exit:
    j main                         
