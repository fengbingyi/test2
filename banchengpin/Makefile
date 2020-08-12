CC := gcc
CFLAGS := -c -o
TARGET ?= app 

OBJS += usr_app.o
OBJS += menu.o
OBJS += client.o

all:$(OBJS)
	$(CC)   $^ -o $(TARGET)
	$(CC) service.c  -lsqlite3 -o service 

%.o:%.c
	$(CC) $(CFLAGS) $@ $< 

clean:
	rm *.o $(TARGET)  service *.db


