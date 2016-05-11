FILES= 		stacksmash.tex aims.tex literature.tex stacksmash.bib
FILES+= 	passwd.c

USE_LATEXMK= 	yes
USE_BIBLATEX= 	yes

.PHONY: all
all: stacksmash.pdf passwd

stacksmash.pdf: ${FILES} llncs

CFLAGS= 	-Wall -g -fno-stack-protector
LDFLAGS= 	-z execstack
ifneq (${MAKE},gmake)
CFLAGS+= 	-D_GNU_SOURCE
endif
LDLIBS= 	-lcrypt

passwd: passwd.o
exploit: exploit.o
payload: exploit
	./exploit > $@

shellcode.o: shellcode.s
shellcode: shellcode.o
	ld $^ -o $@

testshell: testshell.o

.PHONY: run_exploit
run_exploit: passwd exploit
	echo "0" | sudo tee /proc/sys/kernel/randomize_va_space
	touch master.passwd
	sudo chown root:root ./passwd
	sudo chmod +s ./passwd
	-./exploit | ./passwd
	echo "2" | sudo tee /proc/sys/kernel/randomize_va_space

.PHONY: debug_exploit
debug_exploit: passwd payload
	touch master.passwd
	sudo chown root:root ./passwd
	sudo chmod +s ./passwd
	gdb ./passwd


.PHONY: clean
clean:
	${RM} passwd.o passwd master.passwd
	${RM} exploit.o exploit payload
	${RM} shellcode.o shellcode
	${RM} testshell.o testshell


INCLUDE_MAKEFILES=makefiles
include ${INCLUDE_MAKEFILES}/tex.mk
