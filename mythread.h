#ifndef MYTHREAD_H_
#define MYTHREAD_H_
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
#include<pthread.h>

#include "interface.h"
void input_thread_create(void *);
void send_msg_thread_create(void *);
void broadcast_msg_thread_create(void *);
void thread_wait();
void thread_exit();
extern pthread_t send_msg_thread;
extern pthread_t broadcast_msg_thread;
#endif
