CC=gcc
FLAGS=-O2

TARGET=generation_GA
OBJS=generation_GA.o

$(TARGET): $(OBJS) 
	$(CC) $(FLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c
	$(CC) -c $<

clean:
	rm $(TARGET) $(OBJS)
