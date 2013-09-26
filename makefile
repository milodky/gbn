gbnnode: node.o mytimer.o mythread.o packet.o interface.o
	gcc -g -Wall  node.o mytimer.o mythread.o packet.o interface.o -o gbnnode -lpthread

node.o:node.c node.h mytimer.h mythread.h interface.h
	gcc -Wall -c node.c


mytimer.o:mytimer.c mytimer.h
	gcc -Wall -c mytimer.c

mythread.o:mythread.c mythread.h
	gcc -Wall -c mythread.c -lpthread
	
packet.o:packet.c packet.h
	gcc -Wall -c packet.c

interface.o:interface.c interface.h
	gcc -Wall -c interface.c
clean:
	rm *.o gbnnode
