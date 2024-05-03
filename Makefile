CFLAGS = -mcpu=cortex-m0plus -mthumb -ffreestanding -Iexternal/hardware-regs/include ${EXTRA_CFLAGS}

SOURCES = startup.c
OBJS = $(patsubst %.c,%.o,${SOURCES})

${OBJS}: %.o: ${SOURCES}
	${CC} $^ -c ${CFLAGS}

.PHONY: clean
clean:
	rm -f ${OBJS}
