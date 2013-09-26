#include "packet.h"

static packet buffer[max_packet];

static int total;
unsigned int win_size;
unsigned int base_seq; //must be smaller than win_size
unsigned int next_seq;

static unsigned int index_down;
static unsigned int index_up; // must be smaller than max_packet
static unsigned int win_buff;
static int all_done;

/* if one holds this lock, he can modify the window buffer */


/* 
 * operations on the variable win_buff, which indicates 
 * how many packets that we can still transmit. It must
 * be protected, so here I introduce a mutex lock.
 */
void set_win_buff_zero()
{
	win_buff = 0;
}

void init_win_buff()
{
	win_buff = win_size;

}
/*
void increase_win_buff()
{
	win_buff++;
	if (win_buff > win_size) {
		err("the window buffer is larger than the window size!");
		err("logic error happens!\n");
		exit(0);
	}
}

int is_win_buff_empty()
{
	int ret;
	
	ret = (win_buff == win_size);
	return ret;
}
*/
int is_win_buff_available()
{
	int ret;
	ret = (win_size - (next_seq - base_seq)) > 0;	
	return ret;
}
/*
static unsigned int get_win_buff()
{
	return win_buff;
}
*/
/*
void decrease_win_buff()
{

	win_buff--;
	if (win_buff < 0) {
		err("the window buffer is less than 0!");
		err("logic error happens!\n");
		exit(0);
	}

}
*/

int new_transmission()
{
	int ret;
	ret = (index_down == index_up);
//	printf("up = %d, down = %d\n", index_up, index_down);
	//printf("base_seq %d next seq %d\n", base_seq, next_seq);
	return ret;
	
}
int need_more_transimission()
{
	return (next_seq != index_up);
}

void init_pkt()
{
	all_done = 0;
	total = 0;
	win_buff = 0;
	base_seq = 0;
	next_seq = 0;
	index_up = 0;
	index_down = 0;
	
}

static int buf_is_full()
{
	int ret = 0;
	if (index_up > index_down) {
		if ((index_up - index_down) == (max_packet - 1)) {
			ret = 1;
		}
	}
	else if (index_up < index_down) {
		if ((index_down - index_up) == 1) {
			ret = 1;
		}
	}
	return ret;
}
static void _msg(int msg_k)
{
	char *ret = (char *) malloc(LEN + 3);
	int i;
	int x = buffer[msg_k].num;
	int num;
	if (ret == NULL) {
		syserr(errno);
		exit(0);
	}
	memset(ret, 0, LEN + 3);
	for (i = 0; i < LEN; i++) {
		num = x % 10;
		x = x / 10;
		ret[LEN - i - 1] = num + 48;
	}
	
	*(ret + LEN) = 32;
	*(ret + LEN + 1) = buffer[msg_k].msg_c;
	//*(ret + LEN + 1) = c;
	*(ret + LEN + 2) = '\0';
	buffer[msg_k].mesg = ret;
}


void buf_pkt(char *usr_msg)
{
	int len = strlen(usr_msg);
	int i;
	for (i = 0; i < len; i++) {
		if (buf_is_full()) {
	//		err("The buffer is full!");
			printf("Packets %s will be discarded!\n", usr_msg + i);
			break;
		}			
		buffer[index_up].num = total;
		buffer[index_up].msg_c = *(usr_msg + i);
		buffer[index_up].ack = OCUPPIED;
		_msg(index_up);
		total = (total + 1) % ten_power(LEN);
		index_up = (index_up + 1) % max_packet;
	}

}

void del_pkt()
{
	free(buffer[index_down].mesg);
	buffer[index_down].ack = ACKED;
	index_down = (index_down + 1) % max_packet;
	if(index_down == index_up) {
//		next_seq--;
	} else {
		//increase_win_buff();
	}
	base_seq++;
}


packet *get_pkt()
{
	packet *pck = (buffer + next_seq);
	next_seq++;
	return pck;
}
packet *base_pkt() {
	return (buffer + base_seq);
}
/*
int main ()
{
	char test[]={"abcdefg"};
	init();
	buf_pkt(test);
	del_pkt(0);
	del_pkt(1);
	return 0;
}
*/





