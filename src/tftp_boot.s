.data

.text
.globl tftp_boot
.type tftp_boot, %function
.extern vPortEnterCritical

tftp_boot:
    str x0, [sp]         
    bl vPortEnterCritical  
    ldr x0, [sp]           
    mov x30, x0            

    ldr x10, =valid_stack_pointer  
    mov sp, x10            
    mov lr, x30           

    br x30                

valid_stack_pointer: 
    .word 0x40000000       


