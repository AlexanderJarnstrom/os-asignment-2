SRC = src/
OUT = out/
OBJ = obj/

COM = gcc
ARG = --debug
LINK = ${SRC}arg_hand.c -lm

all: fifo lru optimal
	echo "Done"

fifo: ${OBJ}fifo.o ${OBJ}arg_hand.o ${SRC}arg_hand.h
	${COM} ${ARG} -o ${OUT}fifo ${OBJ}fifo.o ${LINK}

lru: ${OBJ}lru.o ${OBJ}arg_hand.o ${SRC}arg_hand.h
	${COM} ${ARG} -o ${OUT}lru ${OBJ}lru.o ${LINK}

optimal: ${OBJ}optimal.o ${OBJ}arg_hand.o ${SRC}arg_hand.h
	${COM} ${ARG} -o ${OUT}optimal ${OBJ}optimal.o ${LINK}

${OBJ}fifo.o: ${SRC}fifo.c
	${COM} -c ${SRC}fifo.c -o ${OBJ}fifo.o

${OBJ}lru.o: ${SRC}lru.c
	${COM} -c ${SRC}lru.c -o ${OBJ}lru.o

${OBJ}optimal.o: ${SRC}optimal.c
	${COM} -c ${SRC}optimal.c -o ${OBJ}optimal.o

${OBJ}arg_hand.o: ${SRC}arg_hand.c 
	${COM} -o ${OBJ}arg_hand.o -c ${SRC}arg_hand.c