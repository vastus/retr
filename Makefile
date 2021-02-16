CC = clang

SRCS = src/retr.c

OBJS = $(SRCS:.c=.o)

LIBS =

retr: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
