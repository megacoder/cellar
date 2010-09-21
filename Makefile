# vim: ts=8 sw=8

CCMODE	=32
CC	=ccache gcc -m${CCMODE} -std=gnu99
OPT	=-O0
# OPT	=-O2
OPT	=-O6
DEFS	=
INC	=-I.
INC	+=-I${HOME}/blast_rel
CFLAGS	=-g -pipe ${OPT} -Wall -Werror -Wextra -pedantic ${DEFS} ${INC}
LDFLAGS	=-g

.SUFFIXES: .lst

%.lst:	%.o
	objdump -dS $< >$@

all:	cellar cellar.lst

cellar.o: gcc-compat.h cellar.c

clean:
	${RM} *.o a.out core.* lint tags

distclean clobber: clean
	${RM} cellar

check:	cellar
	./cellar ${ARGS}
