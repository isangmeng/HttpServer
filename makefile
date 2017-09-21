default:
	gcc -o server *.c -pthread
test:
	gcc -o test test.c CPthreadPool.c linkTab.c Queue.c -pthread
testGdb:
	gcc -o testGdb test.c CPthreadPool.c linkTab.c Queue.c -pthread -g
