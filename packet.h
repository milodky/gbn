#ifndef PACKET_H_
#define PACKET_H_
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
#include<pthread.h>

#include "interface.h"
#define max_packet 1000
/* use 4 bytes to represent a sequence number */
#define LEN 4

#define ACKED		0
#define SENT		1
#define OCUPPIED	2


#define need_start_timer	is_all_done

extern unsigned int win_size;
extern unsigned int base_seq;
extern unsigned int next_seq;

#define _abs(x) ((x) > 0 ? (x) : (-x))
/*
 * num: the sequence number
 * msg: the one byte message
 * ack: 
 		
 *	 0: has been acked, then this buffer becomes available again
 *	 1: has been sent, but still waiting for an ack
 *	 2: this buffer is ocuppied, but not sent yet
 */

typedef struct packet {
	
	int num;
	char msg_c;
	int ack;
	char *mesg;
}packet;

void init_pkt(void);
void buf_pkt(char *);
void del_pkt(void);
packet *get_pkt(void);
void decrease_win_buff(void);
void increase_win_buff(void);
void set_win_buff_zero(void);
void init_win_buff(void);
int is_win_buff_empty(void);
int is_win_buff_available(void);
int need_transmit(void);
int is_all_done();
packet *base_pkt(void);

int new_transmission(void);
int need_more_transimission(void);




#endif
