// dsh.c
// 
// Program 1: Diagnostic Shell
// CSC 456 - Operating Systems
// South Dakota School of Mines and Technology
// Dr. Jeff S. McGough
//
// Author: Christine N. Sorensen
// 2/12/16
//
// Diognostic shell. A process identification program and a simple shell.
// Purpose: To intoduce unix environment, system calls, signal, the proc
//          system, harvard commas, and for/exec system calls.
//
// Bugs:
//  need to add second installment of the program
//      add hb - DONE
//      pthread it up - DONE
//      | - DONE
//      < and >
//      (( and ))
//  Clean up some
//

#include "header.h"


// Split the instructions (used in pipelining)
int splitInstructions( char* line )
{
    int return_val = 1;

    const char delim[2] = "|";
	char* instruction;
	char* instructions[64];

    // extract the first instruction
    instruction = strtok( line, delim );

    // collect the instructions
	int i = 0;
    while( instruction != NULL )
    {
        // add to list of tokens
        instructions[i] = instruction;
        i++;

        // get the next word
        instruction = strtok( NULL, delim );
    }
	
	// go through each instruction
	int j;
	for( j = 0; j < i; j++ )
	{
        // perform each instruction
		return_val = dsh( instructions[j] );

		// if an exit was given, exit the program
		if( return_val == 0 )
		{
			return return_val;
		}		
	}

	return return_val;
}

// Handles the user input
// Param: char* line - the command the user submitted
// Return: 1 - continue while loop. 0 - end program
int dsh( char* line )
{
    // delimiters
    const char delim[3] = " \t";
    //const char redirectDelim[3] = "<>";
    //const char remoteDelim[3] = "()"; 

    // string to hold each token
	// 
    char* token = NULL;
 
    // array to hold the tokens
    char* tokens[64];
    int i = 0;
    
    // extract the first word
    token = strtok( line, delim );

    // collect the words
    while( token != NULL )
    {
        // add to list of tokens
        tokens[i] = token;
        i++;

        // get the next word
        token = strtok( NULL, delim );
    }

    // call the appropriate function
    // check for the correct number of parameters
    // if there are more parameters than needed, it will ignore the excess
    // lowercase the command
    char* command = toLowerCase( tokens[0] );
    
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
                return_val = cmdnm( tokens[1] );
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
    else if( strcmp( command, "hb" ) == 0 )
    {
        if( num_params >= 4 )
        {
            int i1 = isInt( tokens[1] );
            int i2 = isInt( tokens[2] );
            if( i1 != -1 && i2 != -1 )
            {
                return_val = dsh_hb( atoi(tokens[1]), atoi(tokens[2]), tokens[3] );    
            }           
        }
        else
        {
            return_val = -1;
        }
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
    
    // return val determines whether to continue the dsh loop
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
        char* line2 = removeNewLine( line );

        // call the dsh function on the command line
        flag = splitInstructions( line2 );
    }

    return 1;
}
