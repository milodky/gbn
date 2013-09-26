#ifndef INTERFACE_H_
#define INTERFACE_H_
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
#define MAX_ARGS 10

void print_prompt();
int parse(char *, char **, int);
void set_zero(char *, int);
int ten_power(int);
void decrease_one(char *, int);
void increase_one(char *, int);
void syserr(int);

void err(char *);


#endif
