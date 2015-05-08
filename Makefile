FILES= 		stacksmash.tex aims.tex literature.tex stacksmash.bib
FILES+= 	passwd.c

USE_LATEXMK= 	yes
USE_BIBLATEX= 	yes

.PHONY: all
all: stacksmash.pdf passwd

stacksmash.pdf: ${FILES} llncs

ifeq (${MAKE},gmake)
CFLAGS= 	-Wall -g -fno-stack-protector -z execstack
else
CFLAGS= 	-Wall -g -D_GNU_SOURCE -fno-stack-protector -z execstack
endif
LDLIBS= 	-lcrypt

passwd: passwd.o

.PHONY: clean
clean:
	${RM} passwd.o passwd


INCLUDE_MAKEFILES=makefiles
include ${INCLUDE_MAKEFILES}/tex.mk
