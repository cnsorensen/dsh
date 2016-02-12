// dsh.c

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

// Function prototpyes
char* toLowerCase( char* );
int isInt( char* );
int cmdnm( int );
int systat();
int dsh_exit();
int cd( char* );
int pwd();
int dsh_kill( int, int );
int man();
int dsh_fork( char**, int );

/************************
 * To do: 
 *      clean out excess printf's
 *      cmdn
 *      systat
 *      documentation
 */

// Remove newline
// Params: char* line - line to 


// Takes a string and converts it to all lowercase
// Params: string token to be converted
// Return: string of the token lowercased
char* toLowerCase( char* token )
{
    int i = 0;
    char c;

    // go through each letter in the string
    while( token[i] )
    {
        c = token[i];
        token[i] = tolower( c );
        i++;
    }

    // return the newly lower-cased token
    return token;
}

// Checks to see if a character is an int by comparing the ascii values
// is between 48 (0) and 57 (9). Also checks for negative number
// Param: char* num - character string to be check if an integer
// Return: -1 - not an int, 1 - it's an int
int isInt( char* num )
{
    int i = 0;

    // if it's a negative number, ignore the - sign
    if( num[0] == 45 )
    {
        i = 1;
    }


    // go through each decimal in the number passed in
    while( num[i] )
    {
        // if it's in the boundaries of an int
        if( num[i] < 48 || num[i] > 57 )
        {
            return -1;
        }
        
        i++;
    }

    // else, it's an int
    return 1;
}

/////////********IN PROGRESS*******///////////
// Return the command string (name) that started the process
// for a given id
// Usage: cmdnm <pid>
int cmdnm( int pid )
{
    FILE* fin;
    char* filename = "/proc/1/cmdline";
    char cmdnm[128] = {NULL};

    // open the file with the name of the process
    if( (fin = fopen( filename, "r" ) )  == NULL )
    {
        // if it fails
        return -1;
    }    

    // grab the pid's name
    fgets( cmdnm, sizeof( cmdnm ), (FILE*)fin );
    
    // print it to the console
    printf( "%s\n", cmdnm );

    // return to continue loop
    return 1;
}

// Send a signal to a process
// Usage: signal <signal_num> <pid>
// Return: 1, continue while loop
int dsh_signal( int signal_num, int pid )
{   
    // call the signal
    // flag is used to determine if kill was successful
    int flag;
    flag = dsh_kill( pid, signal_num );

    // bummer dude :(
    if( flag != 0 )
    {
        printf( "%s\n", "Signal failed." );
        return -1;
    }

    return 1;
}

////**********IN PROGRESS*******/////////
// Print out some process information using /proc/* files
// Print (to stdout) Linux version and system uptime
// Print memory usage information: memtotal and memfree
// Printn cpu information: vendor id through cache size
int systat()
{
    FILE* fin;
    char version[256];
    char uptime[64];

    // open the file containing the version of linux
    if( ( fin = fopen( "/proc/version", "r" ) ) == NULL )
    {
        return -1;
    }
    
    // get the version from the file
    fgets( version, sizeof( version ), (FILE*)fin );

    // Remove that newline
    size_t len = strlen( version );
    if( len > 0 && version[len-1] == '\n' )
    {
        version[--len] = '\0';    
    }
  
    // close the version file
    fclose( fin );
    
    // print that sob out!
    printf( "Linux version: %s\n", version );
    
    // open uptime file
    if( ( fin = fopen( "/proc/uptime", "r" ) ) == NULL )
    {
        return -1;
    }

    // get the uptime from the file
    fgets( uptime, sizeof( uptime ), (FILE*)fin );

    // Remove that newline
    len = strlen( uptime );
    if( len > 0 && uptime[len-1] == '\n' )
    {
        uptime[--len] = '\0';
    }

    // close the uptime file
    fclose( fin );

    // print uptime
    printf( "System uptime: %s\n", uptime );
    
    // print memory usage: memtotal and memfree
    // found in /proc/meminfo
    char* memtotal = NULL;
    char* memfree = NULL;
    char* line = NULL;
    int memFlag = 0;
    char* label = NULL;
    char* metric = NULL;

    if( ( fin = fopen( "/proc/meminfo", "r" ) ) == NULL )
    {
        return -1;
    }

    const char delim[3] = " \t";

    // get the entirty of meminfo
    while( fgets( line, sizeof( line ), (FILE*)fin ) )
    {
        // extract the labet
        label = strtok( line, delim );
        
        if( strcmp( label, "MemFree:" ) == 0 )
        {
            memFlag++;   
            
            // extract the value and the metric
            memfree = strtok( NULL, " \t" );
            metric = strtok( NULL, " \t" );
            
            // gaaahhhhh, take out that newline!!!!
            len = strlen( metric );
            if( len > 0 && metric[len-1] == '\n' )
            {
                metric[--len] = '\0';
            }   
            
            // Print the memory free to the console
            printf( "MemFree: %s %s\n", memfree, metric );
        }
        else if( strcmp( label, "MemTotal:" ) == 0 )
        {
            memFlag++;
            
            // grab the value and the metric
            memtotal = strtok( NULL, " \t" );
            metric = strtok( NULL, " \t" );
            
            // Remove that gd newline at the end of the metric
            len = strlen( metric );
            if( len > 0 && metric[len-1] == '\n' )
            {
                metric[--len] = '\0';
            }

            // print out the memory total
            printf( "MemTotal: %s %s\n", memtotal, metric );
        }
        if( memFlag == 2 )
        {
            break;
        }
    }

    // close the meminfo file
    fclose( fin );

    // print all of the meminfo (temporary)
    //printf( "Memory Info: %s\n", meminfo );
    //MemFree: and MemTotal:



    // print vendor id through cache size
    if( ( fin = fopen( "/proc/cpuinfo", "r" ) ) == NULL )
    {
        return -11;
    }

    /*while( fgets( line, sizeof( line ), (FILE*) fin ) )
    {
        printf( "%s\n", "green" );
        printf( "%s\n", line );
    }*/

    char line2[128];

    fgets( line2, sizeof( line2 ), (FILE*) fin );
    printf( "%s\n", line2 );


    return 1;
}

// Exit the program, program termination
// Return: 0 - end while loop
int dsh_exit()
{
    // zero to end loop
    return 0;
}

// The standard cd command
// Usage 1: cd <absolute_path>
// Usage 2: cd <relative_path>
// Params: char* path - directory to be changed into
// Return: 1 - continue while loop
int cd( char* path )
{
    int success;

    // change to the directory given
    success = chdir( (const char*) path );
    
    // Invalid path or unsuccessful change
    if( success == -1 )
    {
        // Print an error message
        printf( "%s\n", "Unsuccessful command. (Hint: Did you enter a valid path?)" );
    }
    
    return 1;
}

// Print the current working directory (the path from /)
// Return: 1 - continue while loop
int pwd()
{
    // get the current working directory
    char* cwd;
    cwd = (char*)get_current_dir_name();

    // print to console
    printf( "%s\n", cwd );

    free( cwd );

    return 1;
}

// Signals a process 
// Common signal values:
//  1 - Hangup
//  2 - Interrupt from keyboard
//  9 - KILL SIGNAL MUWHAHAHA
//  15 - Termination signal
//  17, 19, 23 - Stop the process
//
//  Params: pid - process to signal
//          signal_num - value to want to send it (see above for common ones)
//  Return: flag - the response from the kill, 0 for success, -1 for error
int dsh_kill( int pid, int signal_num )
{
    // signal pid. set return to flag to return
    int flag;
    flag = kill( pid, signal_num );

    // return the result from the kill process
    return flag;
}

//////////////************IN PROGRESS****************///////////////
//
int dsh_fork( char* args[], int num_params )
{

    ///////////DEBUGGING REASONS///////////
    /*
    for( int i = 0; i < num_params; i++ )
    {
        printf( "%s\n", args[i] );
    }*/
    ////////////////////////////////

    int c_pid;
    c_pid = fork();

    // child process
    if( c_pid == 0 )
    {
        execvp( args[0], args ); 
        exit(0);
    }
    // parent process
    else
    {
        int waiting;
        wait( &waiting );
    }

    return 1;
}

// dsh
// Handles the user input
// Param: char* line - the command the user submitted
// Return: 1 - continue while loop. 0 - end program
int dsh( char* line )
{
    // delimiters, split at spaces and tabs
    const char delim[3] = " \t";
    
    // string to hold each token
    char* token = NULL;
    
    // array to hold the tokens
    char* tokens[64];
    int i = 0;
    
    // extract the first word
    token = strtok( line, delim );
    
    // collect the words
    while( token != NULL )
    {
        // add to list of tokens after lowercasing them
        tokens[i] = toLowerCase( token );
        i++;

        // get the next word
        token = strtok( NULL,delim );
    }

    ///////////////ME DEBUGGING PRINT
/*    int j;
    printf( "Print %d tokens\n", i );
    for( j = 0; j < i; j++ )
    {
        printf( "%s\n", tokens[j] );
    }
    ///////////////////////////////
*/

    // call the appropriate function
    // check for the correct number of parameters
    // if there are more parameters than needed, it will ignore the excess
    char* command = tokens[0];
    
    // default to return successful
    int return_val = 1;

    // the number of parameters to check for
    int num_params = i;
    if( strcmp( command, "cmdnm" ) == 0 )
    {
        if( num_params >= 2 )
        {
            // check to see if pid is an int
            int flag = isInt( tokens[1] );
            if( flag != -1 )
            {
                // pass in the pid
                return_val = cmdnm( atoi(tokens[1]) );
            }
            else
            {
                return_val = -1;
            }
        }
        else
        {
            return_val = -1;
        }
    }
    else if( strcmp( command, "signal" ) == 0 )
    {
        // pass in the signal number and pic
        if( num_params >= 3 )
        {
            int i1 = isInt( tokens[1] );
            int i2 = isInt( tokens[2] );
            if( i1 != -1 && i2 != -1 )
            {
                return_val = dsh_signal( atoi(tokens[1]), atoi(tokens[2]) );
            }
        }
        else
        {
            return_val = -1;
        }
    }
    else if( strcmp( command, "systat" ) == 0 )
    {
        return_val = systat();
    }
    else if( strcmp( command, "exit" ) == 0 )
    {
        return_val = dsh_exit();
    }
    else if( strcmp( command, "cd" ) == 0 )
    {
        // pass in path
        if( num_params >= 2 )
        {
            return_val = cd( tokens[1] );
        }
        else
        {
            return_val = -1;
        }
    }
    else if( strcmp( command, "pwd" ) == 0 )
    {
        return_val = pwd();
    }
    else if( strcmp( command, "kill" ) == 0 )
    {
        if( num_params >= 3 )
        {
            int i1 = isInt( tokens[1] );
            int i2 = isInt( tokens[2] );
            if( i1 != -1 && i2 != -1 )
            {
                return_val = dsh_kill( atoi(tokens[1]), atoi(tokens[2]) );
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        // it it doesn't match one of the custom commands above,
        // fork it
        return_val = dsh_fork( tokens, num_params );
    }

    // if one of the custom commands failed, fork it
    if( return_val == -1 )
    {
        return_val = dsh_fork( tokens, num_params );
    }
    

    return return_val;
}

int main( int argc, char** argv )
{
    // flag to control the loop
    int flag = 1;
    
    // each line read in from the user
    char line[80];
   
    while( flag == 1 )
    {
        // print the shell name
        printf( "dsh> " );

        // get the line from the user
        fgets( line, sizeof(line), stdin );

        // remove that annoying newline character at the end
        // that fgets seems to have necessary to include
        size_t len = strlen( line );
        if( len > 0 && line[len-1] == '\n' )
        {
            line[--len] = '\0';
        }

        // call the dsh function
        flag = dsh( line );   
    }

    return 1;
}
