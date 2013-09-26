#include "interface.h"
void print_prompt()
{
	printf("node> ");
}
int parse(char *line, char *args[], int n)
{
	//char *tok = NULL;
	char *p, *q;
	//int num;
	int i = 0;
	p = line;
	q = p;
	for (i = 0; i < n; i++) {
		
		p = index(p, ' ');
		if (p == NULL)
			break;
		args[i] = q;
		*p = '\0';
		p++;
		q = p;
	}
	args[i] = q;
	i++;
	return i;
}

void set_zero(char *seq, int len)
{
	memset(seq, 48, len);
}

int ten_power(int n)
{
	int i;
	int ret = 1;
	for(i = 0; i < n; i++)
		ret *= 10;
	return ret;
}

void increase_one(char *seq, int len)
{
	int num = atoi(seq);
	int i;
	int x;
	num++;
	
	if (num == ten_power(len))
		set_zero(seq, len);
	memset(seq, 0, len);
	for (i = 0; i < len; i++) {
		x = num % 10;
		num = num / 10;
		seq[len - i - 1] = x + 48;
	}
}
void decrease_one(char *seq, int len)
{
	int num = atoi(seq);
	int i;
	int x;
	num--;
	
	if (num == ten_power(len))
		set_zero(seq, len);
	memset(seq, 0, len);
	for (i = 0; i < len; i++) {
		x = num % 10;
		num = num / 10;
		seq[len - i - 1] = x + 48;
	}
}
void syserr(int err_num)
{
	printf("[ERROR: %s]\n",strerror(err_num));
}
void err(char *s)
{
	printf("[ERROR: %s]\n",s);
}
