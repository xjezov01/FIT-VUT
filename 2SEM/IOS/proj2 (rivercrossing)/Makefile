CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LFLAGS = -lrt -lpthread

all : rivercrossing

debug : CFLAGS += -g
debug : all
    
rivercrossing : rivercrossing.o
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

clean :
	rm -f rivercrossing rivercrossing.o rivercrossing.out
	
%.o : %.c
	gcc $(CFLAGS) -o $@ -c $<
