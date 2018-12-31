
#include <stdio.h>
#include <ctype.h>


#include "../include/bgmatrix.h"
#include "../include/defs.h"



int dbl_prec = 2;

void showMatrix( BGM_matrix_t *matrix )
{
    printf("%d %d\n[\n", BGM_lines( matrix ) ,BGM_columns( matrix ) );
    
        BGM_foreachLine( matrix,
        {
            putchar(' ');

            BGM_foreachColumn( matrix , printf("%10.*lf ",dbl_prec,BGM_getElement( matrix , i , j ) ) );
        
            putchar('\n');  
        
        });
    
    puts("]");
    
}

int FindChar( FILE *file )
{
    int ch = 0;
    
    while( ( ch = fgetc(file) ) != EOF && !isupper( ch ) );
    
    return ch != EOF? ungetc(ch, file ), 1 : 0;
    
}


int main( int argc, char *argv[ ] )
{
    
    BGM_matrix_t *m[3];
    BGM_matrix_t *tmp, *prod;
    
    double vet[ ] = 
    {
        2.0, 4.2, -4.4,
       -0.3, 9.9, 19.0,
       -1.0, 6.3, 7.0
    };
        
    m[0] = BGM_create( 3, 3, vet );
    m[1] = BGM_transposed( m[0]);

    puts("Matriz Original");
    showMatrix(m[0]);
    puts("Matriz Transposta");
    showMatrix(m[1]);
    
    return 0;
    
}