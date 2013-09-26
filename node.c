#include "node.h"
static int self_socket;
static char IP[] = {"127.0.0.1"};
static char ack[] = {"0000 ACK"};

static int self_port;
static int peer_port;
static int mode;
static int bind_fd;
static int val;
static int forward;
static char peer_data[DATA_LEN];
static char *arg[MAX_ARGS];
static char *line;
static struct sockaddr_in self_addr, peer_addr;
static socklen_t peer_len;
static struct timeval current_time;
/*
 * function declaration
 */
static int node_send(char *, int);
static void send_msg(void);
static int need_discard(int);
static int set_mode(char *, char*);
static void setup(void);
static void print_info(int, char, int);
static void resend_all(void);
static int check(int, char **);
static void get_input_from_user(void);
static pthread_mutex_t mut_window;
/* functions that are related to the lock */
static void get_window_lock()
{
	pthread_mutex_lock(&mut_window);
}

static void release_window_lock()
{
	pthread_mutex_unlock(&mut_window);
}

static void initial_window_lock()
{
	pthread_mutex_init(&mut_window, NULL);
}


static int node_send(char *msg, int send_mode)
{
	int ret = sendto(self_socket, msg, strlen(msg), MSG_DONTWAIT, 
			(struct sockaddr*)&peer_addr, sizeof(peer_addr));
	return ret;
}
/*
 * this function is executed in the thread
 */


static void get_input_from_user()
{
	size_t len;
	while(1) {
		if (new_transmission()) {
			print_prompt();
			getline(&line, &len, stdin);
		
			if (strncmp(line, "\n", 1) == 0)
				continue;
			*(line + strlen(line) -1) = '\0';
			buf_pkt(line);
			forward = 0;
			send_msg_thread_create(send_msg);
			pthread_join(send_msg_thread, NULL);
		}
	}
	thread_exit();	
}
static void send_msg()
{
	packet *current_pkt;
	
	get_window_lock();
	
	while (is_win_buff_available() && !new_transmission() && need_more_transimission()) {
		if (base_seq == next_seq) {
			start_timer(TIMEOUT_VAL);
		}
		current_pkt = get_pkt();
		node_send(current_pkt->mesg, SEND_MODE);
		print_info(current_pkt->num, current_pkt->msg_c, SEND_MODE);
	}
	release_window_lock();
	
	thread_exit();
}




/*
 * mode:
 * 	0: -p
 *	1: -d
 * val:
 *	if mode == 0
 *		we need to generate a random number
 *		then compare it with val, if val is
 *		larger than Rand, then return true
 *		val is between 0 to 100
 */
static int need_discard(int i)
{
	int ret = 0;
	int Rand;
	if (mode) {
		if (!(i % val))
			ret = 1;
	} else {
		Rand = rand() % 100;
		if (Rand < val)
			ret = 1;
	}
	return ret;
}

static int set_mode(char *m, char *v)
{
	int i;
	if (strcmp(m, "-d") == 0) {
		mode = 1;
		val = atoi(v);
		if (val == 0)
			return 0;
		return 1;
	}
	if(strcmp(m, "-p") == 0) {
		mode = 0;
		if (v[0] != '.')
			return 0;
		val = atoi(v + 1);
		for (i = 0; i < 1; i++)
			val = val * 10;
		return 1;
	}
	return 0;
}

			
static void setup()
{
	self_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (self_socket < 0) {
		syserr(errno);
		exit(0);
	}
	self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons(self_port);	
	self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind_fd = bind(self_socket, (struct sockaddr *) &self_addr,
			sizeof(self_addr));
	if (bind_fd < 0) {
		syserr(errno);
		close(self_socket);
		exit(0);
	}	
	memset(&peer_addr, 0, sizeof(struct sockaddr_in));
	peer_addr.sin_family = AF_INET;
	peer_addr.sin_port = htons(peer_port);
	inet_pton(AF_INET, IP, &peer_addr.sin_addr);
	peer_len = sizeof(peer_addr);
	
}
/*
 * print mode:
 * 	in this assignment, we need to implement 
 * 	seven different kinds of outputs
 */

static void print_info(int i, char msg, int print_mode)
{
	
	gettimeofday(&current_time, NULL); 
	printf("[%ld.%ld] ", current_time.tv_sec, current_time.tv_usec);
	switch (print_mode) {
		case ACK_MODE:
			printf("ACK%d received, window moves to %d.\n", i, i + 1);  
			break;
		case SEND_MODE:
			printf("packet%d %c sent.\n", i, msg);
			break;
		case DISCARD_MODE:
			printf("packet%d %c discarded.\n", i, msg);
			break;
		case TIMEOUT_MODE:
			printf("packet%d timeout.\n", i);
			break;
		case ACKSENT_MODE:
			printf("ACK%d sent, expecting packet%d.\n", i, i + 1);
			break;
		case RECV_MODE:
			printf("packet%d %c received.\n", i, msg);
			break;
		case ACKDISCARD_MODE:
			printf("ACK%d discarded.\n", i);
			break;
		default:
			break;
	}
}		

static void resend_all()
{
	print_info(base_seq, base_pkt()->msg_c, TIMEOUT_MODE);
	get_window_lock();
	init_win_buff();
	//printf("timer starts here\n");
	start_timer(TIMEOUT_VAL);
	next_seq = base_seq;
	release_window_lock();
	send_msg_thread_create(send_msg);
	pthread_join(send_msg_thread, NULL);
	

}
static int check(int argc, char **arvg)
{

	if (argc != 6)
		return 0;
	self_port = atoi(arvg[1]);
	if (self_port <= 1024 || self_port >= 65535)
		return 0;
	peer_port = atoi(arvg[2]);
	if (peer_port <= 1024 || peer_port >= 65535)
		return 0;
	win_size = atoi(arvg[3]);
	if(win_size <= 0)
		return 0;
	init_win_buff();
	if(!set_mode(arvg[4], arvg[5]))
		return 0;
	return 1;	
}
/*
 * 
 *
 */





int main(int argc, char *arvg[]) 
{
	//struct timeval tv;
	fd_set node_set;
	int select_fd;
	int dat_len;
	int pkt_num = 0;
	char *msg;
	char expecting_num[LEN];
	char current_num[LEN];
	int i;
	unsigned int diff;
	int pkt_cnt = 0;
	set_zero(expecting_num, LEN);
	
	
	init_timer(resend_all);
	init_pkt();
	
	
	/* check whether the arguments are valid */
	if (!check(argc, arvg)) {
		err("Invalid arguments!");
		return 0;
	}
	/* setup all the sockets, binds and so on */
	setup();
	initial_window_lock();
	input_thread_create(get_input_from_user);
	while (1) {
		//tv.tv_sec = 1;
		//tv.tv_usec = 0;
		FD_ZERO(&node_set);     
		FD_SET(self_socket, &node_set);	
		select_fd = select(self_socket + 1, NULL, &node_set, NULL, NULL);
		if (select_fd == -1) {
			syserr(errno);
			exit(0);
		}
		if (select_fd == 0) {
			//err("No connection.");
			continue;
		}
	
		if (FD_ISSET(self_socket, &node_set) == 0) 
			continue;
		
		memset(peer_data, '\0', DATA_LEN);
		dat_len = recvfrom(self_socket, peer_data, DATA_LEN, 0, 
				(struct sockaddr*)&peer_addr, &peer_len);
		if (dat_len > 0) {
			parse(peer_data, arg, 1);
			pkt_num = atoi(arg[0]);
			msg = arg[1];
			pkt_cnt++;
			/* 
			 * *****************************************************
			 * *****************************************************
			 * ********** sender receives the ACK packet ***********
			 * *****************************************************
			 * *****************************************************
			 */
		//	printf("msg = %s, length of msg = %d\n", msg, strlen(msg));
			if (strncmp(msg, "ACK", 3) == 0) {
				
				/* decide whether we need to discard this ACK */
				if (need_discard(pkt_cnt)) { 
				 	print_info(pkt_num, *msg, ACKDISCARD_MODE);
				 	forward = 0;
				 	continue;
				}
				
				/* 
				 * first we should check whether it is the base
				 * packet, if so,then window should move forward
				 * one step and restart the timer, if not, just
				 * print a message and then discard this packets
				 * ---------------------------------------------
				 * only the thread could send the data packet
				 * here we need to consider the synchronization
			 	 */
				if (pkt_num >= base_seq) {
					get_window_lock();
					print_info(pkt_num, *msg, ACK_MODE);
					diff = pkt_num - base_seq;
					for (i = 0; i <= diff; i++)
						del_pkt();
					forward = 1;
					
					if (base_seq == next_seq) {
						stop_timer();
					}
					else {
						start_timer(TIMEOUT_VAL);
					}
					release_window_lock();
				
					send_msg_thread_create(send_msg);
					pthread_join(send_msg_thread, NULL);
				
				} else {
					forward = 0;
					//start_timer(TIMEOUT_VAL);
					print_info(base_seq - 1, *msg, ACK_MODE);
				}
				
				continue;
			}
			/*
			 * *****************************************************
			 * *****************************************************
			 * ************ receiver gets a new packet *************
			 * *****************************************************
			 * *****************************************************
			 */
			 
			if (need_discard(pkt_cnt)) {
			/* do some print, then just ignore this packet */	
				print_info(pkt_num, *msg, DISCARD_MODE);
				continue;
			}
			
			/* do some print, and send an ack back to the sender */
			

			if (strcmp(expecting_num, arg[0]) == 0) {
				/*
				 * the packet with the expecting sequence number
				 * is received, and the receiver also knows that 
				 * the window moves forward one step, so it adds
				 * 1 to expecting_num correspondingly
				 */
				
				strncpy(ack, expecting_num, LEN);
				print_info(atoi(expecting_num), *msg, ACKSENT_MODE);
				increase_one(expecting_num, LEN);
				
			} else if (atoi(expecting_num) == 0) {
				/* 
				 * the sequence number is 0, but now it received
				 * a packet with a sequence number other than 0,
				 * so just ignores it and does not send ack 
				 */
				print_info(pkt_num, *msg, DISCARD_MODE);
				continue;
			} else {
				/*
				 * although the receiver gets a packet, but it's
				 * not the one with the expecting sequence num.
				 * So after doing some print, we just ignore it
				 * and send an ACK back with expecting_num				 
				 */
				strncpy(current_num, expecting_num, LEN);
				decrease_one(current_num, LEN);
				strncpy(ack, current_num, LEN);
				print_info(atoi(current_num), *msg, ACKSENT_MODE);
			}
			node_send(ack, ACK_MODE);
		}		
	}
	thread_wait();
	free(line);
}
