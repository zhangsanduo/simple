Target: sm4
CC=gcc
CFALAGS=
objs=sm4.o run_sm4.o

cins=sm4.c run_sm4.c

head=sm4.h

all:$(Target)
	@echo " make done!"

$(Target):$(objs)
	$(CC) $< -o $^

%.o:%.c $(head)
	$(CC) -C $< -o $^


clean:
	rm -f $(Target) $(objs)