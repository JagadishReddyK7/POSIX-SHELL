CC = g++
OBJ = obj
CFLAGS = -Wall -g -I${OBJ}

shell: ${OBJ}/shell.o ${OBJ}/shell_utils.o
	${CC} ${CFLAGS} ${OBJ}/shell.o ${OBJ}/shell_utils.o -o shell -lreadline

${OBJ}/shell.o: shell.cpp shell.h shell_utils.h
	mkdir -p ${OBJ}
	${CC} -c ${CFLAGS} shell.cpp -o ${OBJ}/shell.o -lreadline

${OBJ}/shell_utils.o: shell_utils.cpp shell_utils.h
	mkdir -p ${OBJ}
	${CC} -c ${CFLAGS} shell_utils.cpp -o ${OBJ}/shell_utils.o

clean:
	rm -rf ${OBJ}/*.o shell