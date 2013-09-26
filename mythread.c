#include "mythread.h"

/* thread for sending data packet */
pthread_t send_msg_thread;
static pthread_t input_thread;
pthread_t broadcast_msg_thread;

/* functions that are related to the thread */
void input_thread_create(void *function)
{
	int temp;
	memset(&input_thread, 0, sizeof(send_msg_thread));
	temp = pthread_create(&input_thread, NULL, (void *) function, NULL);
	if(temp != 0) {
		syserr(errno);
		exit(errno);
	}
}

void send_msg_thread_create(void *function)
{
	int temp;
	memset(&send_msg_thread, 0, sizeof(send_msg_thread));
	temp = pthread_create(&send_msg_thread, NULL, (void *) function, NULL);
	if(temp != 0) {
		syserr(errno);
		exit(errno);
	}
}

void broadcast_msg_thread_create(void *function)
{
	int temp;
	memset(&send_msg_thread, 0, sizeof(broadcast_msg_thread));
	temp = pthread_create(&broadcast_msg_thread, NULL, (void *) function, NULL);
	if(temp != 0) {
		syserr(errno);
		exit(errno);
	}
}





void thread_wait()
{
        if(send_msg_thread != 0) {
                pthread_join(send_msg_thread, NULL);
        }
        if(send_msg_thread != 0) {
                pthread_join(input_thread, NULL);
        }
}

void thread_exit()
{
	pthread_exit(NULL);
}
