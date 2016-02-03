// dsh.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Return the command string (name) that started the process
// for a given id
// Usage: cmdnm <pid>
int cmdnm( int pid )
{
    return 1;
}

// Send a signal to a process
// Usage: signal <signal_num> <pid>
int signal( int signal_num, int pid )
{
    return 1;
}

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
    printf( "path changin here!\n" );
//    printf( "%s\n", path );
    //chdir( path );
    const char* path1 = "/home/student/1959287/";
    chdir( path1 );
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

    return 1;
}

// Print the manual for dsh
int man()
{
    printf( "%s\n", "cmdnm <pid> - return the name that started the process for the given id" );
    printf( "%s\n", "signal <signam_num> <pid> - send a signal to a process" );
    printf( "%s\n", "systat - print out some process information using /proc/* files" );
    //print the other stuff here//
    printf( "%s\n", "exit - exit the program" );
    printf( "%s\n", "cd <path> - change directory" );
    printf( "%s\n", "pwd - print the current working directory" );

    return 1;
}

int dsh( char* line )
{
    // space to tokenize
    const char space[2] = " ";
    // string to hold each token
    char* token = NULL;
    // array to hold the tokens
    char* tokens[64];
    int i = 0;

    // extract the first word
    token = strtok( line, space );
    
    // collect the words
    while( token != NULL )
    {
        // store each into the array
        tokens[i] = token;
        i++;
        // get the next word
        token = strtok( NULL, space );
    }

    // call the appropriate function
    // check for the correct number of parameters
    // if there are more parameters than needed, it will ignore the excess
    char* command = tokens[0];
    int return_val = 1;
    int num_params = i;
    if( strcmp( command, "cmdnm" ) == 0 )
    {
        printf( "%s\n", "cmdn" );
        if( num_params >= 2 )
        {
            // pass in the pid
            return_val = cmdnm( tokens[1] );
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
            return_val = signal( tokens[1], tokens[2] );
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
        printf( "%s\n", "exit" );
        return_val = exit_dsh();
    }
    else if( strcmp( command, "cd" ) == 0 )
    {
        printf( "%s\n", "cd" );
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
        printf( "%s\n", "pwd" );
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
