CC=gcc
CFLAGS= -Og -Wall -fno-omit-frame-pointer 
LDLIBS= -lrt -lpthread
all: pingpong pagewalk funccalls

%.o: %.c
	echo $(CC) -o $@ $(CFLAGS)
	$(CC) -o $@ $< $(CFLAGS) $(LDLIBS)


clean:
	rm -f *.o

