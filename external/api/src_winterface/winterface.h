
#ifndef INCLUDE_GUARD_WINTERFACE
#define INCLUDE_GUARD_WINTERFACE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "winterface-config.h"

#ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>
#else
#include <netinet/in.h> //structure for storing address information
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#endif

// Datatypes for Package-Payload
//
#define TEST_STRING 199
#define TEST_SIMPLE_INT 200
#define TEST_SIMPLE_D 201
#define TEST_SIMPLE_F 202


#define CMD_SP 0
#define	CMD_PWM 1
#define CMD_MC 2


#define RESPONSE_SIZE 16
#define N_BUFFER_SIZE 32
#define RESERVED_SPACE 1
#define MAX_ARGC 12
#define DOUBLE_PRECISION 3
#define DOUBLE_PRECISION_PRINTF "%.3f"

typedef struct _header{
	uint32_t size;
}HEAD;

typedef struct _buffer{
	HEAD header;
	char payload[N_BUFFER_SIZE];
}BUF;

typedef struct _winterface{
#ifdef ESP32
	WiFiClient client; // = server.available();
#else
	int socket;
#endif

	int argc;
	char *argv[MAX_ARGC];
	BUF buf; 
	char *endl;
}WINT,*WINTP;

typedef struct _cmd{
//	int identifier;
//	int qnt_param;
	void (*function)(int, char**);
}CMD,*PCMD;


int netw_recv_header(WINTP netw);
void netw_send(WINTP netw);
int netw_recv(WINTP netw);

void netw_init( WINTP netw, int socket);
void netw_close(WINTP netw);
void netw_erase_buf();
void netw_add_i(int );
void netw_add_d(double );

int netw_int_len_helper(int x);

void run_test( WINTP, int  );
void netw_clear( WINTP netw );
void netw_clear_arg( WINTP netw);
void netw_clear_buf( WINTP netw);
void netw_init( WINTP netw, int socket);
void netw_analyze_arg( WINTP netw);
void netw_generate_arg(WINTP netw);
int netw_check_string_safety( WINTP netw , int length_to_add);
char* netw_add_int(WINTP netw, int val );
char* netw_add_double(WINTP netw, double val );
char* netw_add_string(WINTP netw, char* val );
void netw_set_argv( WINTP netw );
void netw_analyze_payload( WINTP netw);

int netw_handle_message(WINTP netw, PCMD arr);




// cmd stuff
//
void cmd_arr_init( PCMD );

void run_test_recv( WINTP, int );
void run_test_send( WINTP, int );
void netw_into_buf( void *data, int size,int type);

void printStatus(int,int);
void netw_analyze_payload( WINTP netw);


#endif
