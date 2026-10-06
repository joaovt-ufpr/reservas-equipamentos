CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
OBJS    = main.o lista.o equipamentos.o

reservas: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c lista.h equipamentos.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) reservas reservas.exe
