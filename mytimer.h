#ifndef MYTIMER_H_
#define MYTIMER_H_
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
#include "interface.h"


void init_timer(void *);
/* the timer is in micro second */
void start_timer(int);
void stop_timer();



#endif
