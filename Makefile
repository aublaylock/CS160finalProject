SRCS = main.c board.c alloweds.c solver.c hash.c
OBJS = $(SRCS:.c=.o)
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
