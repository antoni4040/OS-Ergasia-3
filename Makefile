INC_DIR = 	./include
OBJS1   =	mystation.o files.o
OBJS2 	=	bus.o
OBJS3   = 	station-manager.o
OBJS4	=	comptroller.o
SOURCE  =	./src/mystation.c ./src/comptroller.c ./src/files.c ./src/station-manager.c ./src/bus.c
HEADER  =	./include/mystation.h ./include/comptroller.h ./include/files.h ./include/station-manager.h ./include/bus.h
OUT1    =	mystation
OUT2    =	bus
OUT3    =	station-manager
OUT4    =	comptroller
CC      =	gcc
FLAGS   =   -pthread -Wall   -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

debug:      FLAGS    += -g
release:    FLAGS    += -O3

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

debug:      all
release:    all

$(OUT1): $(OBJS1)
	$(CC)   -lm -pthread    $(OBJS1) -o    $@

$(OUT2): $(OBJS2)
	$(CC)   -lm -pthread    $(OBJS2) -o    $@

$(OUT3): $(OBJS3)
	$(CC)   -lm -pthread    $(OBJS3) -o    $@

$(OUT4): $(OBJS4)
	$(CC)   -lm -pthread   $(OBJS4) -o    $@

mystation.o: ./src/mystation.c
	$(CC)   $(FLAGS)        ./src/mystation.c

comptroller.o: ./src/comptroller.c
	$(CC)   $(FLAGS)        ./src/comptroller.c

files.o: ./src/files.c
	$(CC)   $(FLAGS)        ./src/files.c

station-manager.o: ./src/station-manager.c
	$(CC)   $(FLAGS)        ./src/station-manager.c

bus.o: ./src/bus.c
	$(CC)   $(FLAGS)        ./src/bus.c

clean:
	rm -f $(OBJS1) $(OUT1) $(OBJS2) $(OUT2) $(OBJS3) $(OUT3) $(OBJS4) $(OUT4)

count:
	wc $(SOURCE) $(HEADER)

