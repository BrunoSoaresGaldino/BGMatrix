
#include <stdio.h>
#include "../include/bgmatrix.h"
#include "../include/defs.h"

int dbl_prec = 2;

void showMatrix( BGM_matrix_t *matrix )
{
    printf("%d x %d\n[\n", BGM_rows( matrix ) ,BGM_columns( matrix ) );
    
        BGM_foreachrow( matrix,
        {
            putchar(' ');

            BGM_foreachColumn( matrix , printf("%10.*lf ",dbl_prec,BGM_getElement( matrix , i , j ) ) );
        
            putchar('\n');  
        
        });
    
    puts("]");
    
}

int main( int argc, char *argv[ ] )
{
    
    BGM_matrix_t *m[3];
    
    double m1[ ] = 
    {
        2.0, 4.2,
       -0.3, 9.9
    };

    double m2[ ] = 
    {
         1,  3, -92,
         0, 32,   2,
        17,  8,   0
    };
        
    m[0] = BGM_create( 2, 2, m1 );
    m[1] = BGM_transposed( m[0] );
    m[2] = BGM_create( 3, 3, m2 );
    

    puts("\nM1");
    showMatrix(m[0]);
    puts("\n\nM2 = M1^t");
    showMatrix(m[1]);
    puts("\n\nM3 = M3");
    showMatrix(m[2]);

    printf("\n\nDeterminante de M1: %lf\n", BGM_determinant(m[0]) );
    printf("\nDeterminante de M2 = M1^t: %lf\n", BGM_determinant(m[1]) );
    printf("\nDeterminante de M3: %lf\n", BGM_determinant(m[2]) );
    
    return 0;
    
}