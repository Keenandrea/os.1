CC = gcc
CFLAGS = -g
TARGET = bt
OBJS = main.o flags.o utils.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
flags.o: flags.c flags.h
	$(CC) $(CFLAGS) -c flags.c
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c
clean:
	rm -f *.o $(TARGET)
