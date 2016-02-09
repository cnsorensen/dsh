// dsh.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

/************************
 * To do: 
 *      check int to see if greater than 9
 *      cmdn
 *      signal
 *      syscat
 */

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

/////////////////IN PROGRESS///////////////////
// Checks to see if a character is an int by comparing the ascii values
// is between 48 (0) and 57 (9)
int isInt( char* num )
{
    int i = 0;

    // go through each decimal in the number passed in
    while( num[i] )
    {
        // if it's in the boundaries of an int
        if( num[i] < 48 || num[i] > 57 )
        {
            printf( "%s\n", "Not an int" );
            return -1;
        }
        
        i++;
    }

    // else, it's an int
    printf( "%s is an Int\n", num );
    return 1;
}

/////////********IN PROGRESS*******///////////
// Return the command string (name) that started the process
// for a given id
// Usage: cmdnm <pid>
int cmdnm( int pid )
{
    return 1;
}

///********IN PROGRESS**********//////////
// Send a signal to a process
// Usage: signal <signal_num> <pid>
int signal( int signal_num, int pid )
{
    printf( "signum: %d\npid: %d\n", signal_num, pid );
    //int i = isInt( signal_num );
    return 1;
}

////**********IN PROGRESS*******/////////
// Print out some process information using /proc/* files
// Print (to stdout) Linux version and system uptime
// Print memory usage information: memtotal and memfree
// Printn cpu information: vendor id through cache size
int systat()
{
    return 1;
}

// Exit the program, program termination
int exit_dsh()
{
    // zero to end loop
    return 0;
}

// The standard cd command
// Usage 1: cd <absolute_path>
// Usage 2: cd <relative_path>
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
int pwd()
{
    // get the current working directory
    char* cwd;
    cwd = get_current_dir_name();
    
    // print to console
    printf( "%s\n", cwd );

    free( cwd );

    return 1;
}

// Print the manual for dsh
int man()
{
    printf( "%s\n", "cmdnm <pid> - return the name that started the process for the given id" );
    printf( "%s\n", "signal <signam_num> <pid> - send a signal to a process" );
    printf( "%s\n", "systat - print out some process information using /proc/* files" );
    /////print the other stuff here//
    printf( "%s\n", "exit - exit the program" );
    printf( "%s\n", "cd <path> - change directory" );
    printf( "%s\n", "pwd - print the current working directory" );

    return 1;
}

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
    int j;
    printf( "Print %d tokens\n", i );
    for( j = 0; j < i; j++ )
    {
        printf( "%s\n", tokens[j] );
    }
    ///////////////////////////////

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
        printf( "%s\n", "cmdn" );
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
                printf( "%s\n", "Usage: cmdnm <pid> (Hint: Are you passing in a valid pid?)" );
            }
        }
        else
        {
            printf( "%s\n", "Usage: cmdnm <pid>" );
            return_val = 1;
        }
    }
    else if( strcmp( command, "signal" ) == 0 )
    {
        printf( "%s\n", "signal" );
        // pass in the signal number and pic
        if( num_params >= 3 )
        {
            int i1 = isInt( tokens[1] );
            int i2 = isInt( tokens[2] );
            if( i1 != -1 && i2 != -1 )
            {
                return_val = signal( atoi(tokens[1]), atoi(tokens[2]) );
            }
        }
        else
        {
            printf( "%s\n", "Usage: signal <signam_num> <pid>" );
            return_val = 1;
        }
    }
    else if( strcmp( command, "systat" ) == 0 )
    {
        printf( "%s\n", "systat" );
        return_val = systat();
    }
    else if( strcmp( command, "exit" ) == 0 )
    {
        return_val = exit_dsh();
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
            printf( "%s\n", "Usage: cd <absolute_path>|<relative_path>" );
        }
    }
    else if( strcmp( command, "pwd" ) == 0 )
    {
        return_val = pwd();
    }
    else if( strcmp( command, "man" ) == 0 )
    {
        return_val = man();
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
