
#ifndef _DSH_H_
#define _DSH_H_

#define _GNU_SOURCE
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

// Function prototpyes
// Extra functions
char* toLowerCase( char* );
int isInt( char* );
char* removeNewLine( char* );

// dsh commands
int cmdnm( char* );
void* p_cmdnm( void* );
int systat();
void* p_systat( void* );
int dsh_exit();
int cd( char* );
int pwd();
int dsh_kill( int, int );
int dsh_fork( char**, int );
int dsh_signal( int, int );
int dsh_hb( int, int, char* );
void* p_hb( void* );
int dsh( char* );
int splitInstructions( char* );

#endif
