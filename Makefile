SRC = src/
OUT = out/
OBJ = obj/

COM = gcc
ARG = -O2 --debug
LINK = arg_hand.c -lm

all: arg_hand.o arg_hand.h
	$COM $ARG -o ${OUT}fifo ${SRC}fifo.c $LINK
	$COM $ARG -o ${OUT}lru ${SRC}lru.c $LINK
	$COM $ARG -o ${OUT}optimal ${SRC}optimal.c $LINK

arg_hand.o: arg_hand.c 
	$COM -o arg_hand.o -c arg_hand.c