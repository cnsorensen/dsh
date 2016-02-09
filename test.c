#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    const char c1 = 54;
    const char c2[2] = "i";
    const char c3[2] = "62";
    printf( "printme!" );
    printf( "%c %s %s\n", c1, c2, c3 );

    int i = 0;
    int j;

    while( c2[i] )
    {
        j = c2[i];
        printf( "%d\n", c2[i++] );
        printf( "Num: %d\n", j ); 
        if( j > 57 )
        {
            printf( "%s\n", "Aint nobody got time for dat" );
        }
    }

    //int j = strcmp( c1, c3 );
    //printf( "%d\n", j );

    return 0;
}
