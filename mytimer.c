#include "mytimer.h"


/* time-out parameter */
struct itimerval p_timeout;
/* functions that are related to the timer */
void init_timer(void *function)
{
	memset(&p_timeout, 0, sizeof(p_timeout));
	p_timeout.it_value.tv_sec = 0;
	p_timeout.it_value.tv_usec = 0;
	p_timeout.it_interval.tv_sec = 0;
	p_timeout.it_interval.tv_usec = 0;
	signal(SIGALRM, function);

	/*
	p_timeout.it_interval.tv_sec = 0;
	p_timeout.it_interval.tv_usec = TIMEOUT_VAL;
	*/
}
/* the timer is in micro second */
void start_timer(int interval)
{
	p_timeout.it_value.tv_usec = interval;
	if (setitimer(ITIMER_REAL, &p_timeout, NULL))
		exit(errno);
}
void stop_timer()
{
	p_timeout.it_value.tv_usec = 0;
	if (setitimer(ITIMER_REAL, &p_timeout, NULL))
		exit(errno);
}

