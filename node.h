#ifndef NODE_H_
#define NODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h> 

#include "packet.h"
#include "interface.h"
#include "mytimer.h"
#include "mythread.h"

#define DATA_LEN (LEN + 4)
#define ACK_MODE	0
#define DISCARD_MODE	1
#define SEND_MODE	2
#define TIMEOUT_MODE	3	
#define ACKSENT_MODE	4
#define RECV_MODE	5
#define ACKDISCARD_MODE	6
#define NormalSend_MODE	7
#define TRANSDONE_MODE	8


#define TIMEOUT_VAL	500000


#endif
