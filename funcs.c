// dsh_funcs.c
 
#include "header.h"

// Return the command string (name) that started the process
//  for a given id
// Usage: cmdnm <pid>
// Params: pid - the process id 
// Return: 1- success, continue looping, -1 - fail. fork it.
int cmdnm( char* pid )
{
    FILE* fin;
    //char filename[32] = {NULL};
    char filename[32];

    // create the filename with the pid
    strcat( filename, "/proc/" );
    strcat( filename, pid );
    strcat( filename, "/cmdline" );

    // holds the output of the cmdnm file
    //char cmdnm[128] = {NULL};
    char cmdnm[128];
    

    // open the file with the name of the process
    if( (fin = fopen( filename, "r" ) )  == NULL )
    {
        // if it fails
        return -1;
    }    

    // grab the pid's name
    fgets( cmdnm, sizeof( cmdnm ), (FILE*)fin );

    // print to output
    printf( "%s\n", cmdnm );

    // return to continue loop
    return 1;
}

// Send a signal to a process
// Usage: signal <signal_num> <pid>
// Return: 1, continue while loop
int dsh_signal( int signal_num, int pid )
{   
    // call the signal using the kill function
    dsh_kill( pid, signal_num );

    return 1;
}

// Print all of the stats of the system
//  Print out some process information using /proc/* files
//  Print (to stdout) Linux version and system uptime
//  Print memory usage information: memtotal and memfree
//  Printn cpu information: vendor id through cache size
// Usage: systat
// Params: nun
// Return: 1 for continue looping, -1 for an error
int systat()
{
    // used for all file opening
    FILE* fin;

    //** linux version **//
    // found in /proc/version
    char version[256];

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
   
    //** system uptime **//
    // found in /proc/uptime
    char uptime[64];

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
    
    //** memory usage: memtotal and memfree **//
    // found in /proc/meminfo
    char* memtotal = NULL;
    char* memfree = NULL;
    char line[256];
    int memFlag = 0;
    char* label = NULL;
    char* metric = NULL;

    // open up folder with the memory information
    if( ( fin = fopen( "/proc/meminfo", "r" ) ) == NULL )
    {
        return -1;
    }

    // delimiter to tokenize the memory info
    const char delim[3] = " \t";

    // get the entirty of meminfo one line at a time
    while( fgets( line, sizeof( line ), (FILE*)fin ) )
    {
        // extract the label
        label = strtok( line, delim );
            
        // if it's the memory free information
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

        // if it's the memory total information
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

        // stop reading the file once you get the two needed values
        if( memFlag == 2 )
        {
            break;
        }
    }

    // close the meminfo file
    fclose( fin );

    //** print vendor id through cache size **//
    // found in /proc/cpu info

    // count for number of lines needed to print
    int cpuFlag = 0;
    char linePrint[256];

    // delimiter to tokenize the memory info
    const char delim2[4] = " \t";

    // open the file with the cpu info
    if( ( fin = fopen( "/proc/cpuinfo", "r" ) ) == NULL )
    {
        return -1;
    }

    // get entirity of cpuinfo one line at a time
    while( fgets( line, sizeof( line ), (FILE*) fin ) )
    {
        // copy the read in line to a variable that will print it
        strcpy( linePrint, line );

        // extract the label
        label = strtok( line, delim2 ); 

        // compare the labels
        if( strcmp( label, "vendor_id" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }
        else if( strcmp( label, "cpu" ) == 0 )
        {
            if( strcmp( strtok( NULL, delim2 ), "family" ) == 0 ) 
            {
                printf( "%s", linePrint );
                cpuFlag++;
            }
        }
        // Note here: model name will short circuit here before seeing the next 
        // else-if to avoid printing a duplicate line with model
        else if( strcmp( label, "model" ) == 0 &&  strcmp( strtok( NULL, delim2 ), "name" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }
        else if( strcmp( label, "model" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }
        else if( strcmp( label, "stepping" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }
        else if( strcmp( label, "microcode" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }
        else if( strcmp( label, "cpu" ) == 0 && strcmp( strtok( NULL, delim2 ), "MGz" ) == 0 )
        {
                printf( "%s", linePrint );
                cpuFlag++;
        }
        else if( strcmp( label, "cache" ) == 0 && strcmp( strtok( NULL, delim2 ), "size" ) == 0 )
        {
            printf( "%s", linePrint );
            cpuFlag++;
        }

        // If you have printed all of the needed cpu infos, stop reading from file
        if( cpuFlag >= 8 )
        {
            break;
        }
    }

    // close the cpuinfo file
    fclose( fin );

    return 1;
}

// Exit the program, program termination
// Usage: exit
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
// Return: 1 - continue while loop, -1 for failure
int cd( char* path )
{
    int success;

    // change to the directory given
    success = chdir( (const char*) path );
    
    // Invalid path or unsuccessful change
    if( success == -1 )
    {
        return -1;
    }
    
    return 1;
}

// Print the current working directory (the path from /)
// Usage: pwd
// Params: nun
// Return: 1 - continue while loop
int pwd()
{
    // get the current working directory
    char* cwd;
    cwd = (char*)get_current_dir_name();

    // print to console
    printf( "%s\n", cwd );

    return 1;
}

// Signals a process 
// Common signal values:
//      1 - Hangup
//      2 - Interrupt from keyboard
//      9 - KILL SIGNAL MUWHAHAHA
//      15 - Termination signal
//      17, 19, 23 - Stop the process
//  Usage: kill pid signal_num
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

// Uses the fork command to signal the system with a command
//  Layout is similar to Dr. Jeff McGough's code which was taken from
//  "Advanced Linux Programming," by CodeSourcery LLC
//  Copyright (C) 2001 by New Riders Publishing
//
// Params: char* args[] - list of the commands from the user
// Return: 1 to continue for loop for the program
int dsh_fork( char* args[], int num_params )
{    
    // duplicate the process
    int c_pid;
    //pid_t c_pid;
    c_pid = fork();

    // array to hold the arguments
    char* ironman[num_params + 1];

    // add the null to the end in order for execvp to work
    int i;
    for( i = 0; i < num_params; i++ )
    {
        ironman[i] = args[i];
    }
    ironman[i] = NULL;
    

    // child process
    if( c_pid == 0 )
    {
        // execute the program
        execvp( args[0], ironman );
        // only returns when an erro occurs
        //printf( "printing if error occurs\n" ); 
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

// hb - heartbeat 
//  Print current time loop
// Usage: hb tinc tend tval
// Params:
//  tinc - time increments (intervals)
//  tend - time to end, how long to run
//  tval - the units in s, ms for seconds and miliseconds respectively
// Return: 1 for success, -1 for failure of syntax of the tval
int dsh_hb( int tinc, int tend, char* tval )
{

    // in linux, date +"%T" shows the time
    // date +"%T.%3N" shows milliseconds

    // holds the date command to be called
    // initializing it to just show the time
    char* timeArgs[2] = { "date", "+%T" };

    // if the values are in seconds
    if( strcmp( tval, "s" ) == 0 )
    {
        // convert the time increment to microseconds
        // 1 second == 1000000 microseconds
        tinc = tinc * 1000000;
    }

    // if the values are in milliseconds
    else if( strcmp( tval, "ms" ) == 0 )
    {
        // change it to display milliseconds
        timeArgs[1] = "+%T.%3N";

        // convert the time increment to microseconds
        // 1 millisecond == 1000 microseconds
        tinc = tinc * 1000;
    }

    // the user put in a wrong value for tval
    else
    {
        // FAIL!!
        return -1;
    }

    // run it the first time
    // this avoids that extra wait time at the end of this command
    if( tend > 0 )
    {
        dsh_fork( timeArgs, 2 );
    }

    // do the number of times specified to run
    int i;
    for( i = 1; i < tend; i++ )
    {
        // wait the incremented time
        // usleep takes in values for microseconds
        usleep( tinc );

        // call the date command
        dsh_fork( timeArgs, 2 );  
    }

    return 1;
}
