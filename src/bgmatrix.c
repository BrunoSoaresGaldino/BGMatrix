
#include "../include/bgmatrix.h"

BGM_error_t BGM_error;

struct BGM_matrix_t
{
    double   *elements;
    uint16_t lines;
    uint16_t columns;
};


BGM_matrix_t *BGM_create( uint16_t lines, uint16_t columns, double *elements )
{
    
    BGM_matrix_t *matrix;
    size_t        size = lines * columns * sizeof(double);
    
    if( ( matrix = malloc( sizeof(BGM_matrix_t) ) ) )
    {
        if( ( matrix->elements = malloc( size ) ) )
        {
            matrix->lines   = lines;
            matrix->columns = columns;
            
            if( elements )
            {
                memcpy( matrix->elements, elements, size );
            }
            else
            {
                memset( matrix->elements, 0, size );
            }
            
            return matrix;
        }
        
        free(matrix);
    }
    
    return NULL;
}


BGM_matrix_t *BGM_clone( BGM_matrix_t *matrix )
{

    return BGM_create( matrix->lines, matrix->columns, matrix->elements);   

}


void BGM_destroy( BGM_matrix_t *matrix )
{
    if( matrix )
    {
        free( matrix->elements );
        free( matrix );
    }
}

void BGM_destroyP( BGM_matrix_t ***matrix )
{
    BGM_destroy( **matrix );
}


double BGM_getElement( BGM_matrix_t *matrix, uint16_t i , uint16_t j )
{
    return *( matrix->elements + i * matrix->columns + j );
}


void BGM_setElement( BGM_matrix_t *matrix, uint16_t i , uint16_t j , double element )
{
    
    *( matrix->elements + i * matrix->columns + j ) = element;   
}


uint16_t BGM_lines( BGM_matrix_t *matrix )
{
    return matrix->lines;
}


uint16_t BGM_columns( BGM_matrix_t *matrix )
{
    return matrix->columns;
}



BGM_matrix_t *BGM_transposed( BGM_matrix_t *matrix )
{   
    
    BGM_matrix_t *new = BGM_clone( matrix );
    
    if( new )
    {
        BGM_swap( new->lines, new->columns );
        
        BGM_foreach( new, *(new->elements + i*new->columns + j ) = *( matrix->elements + j*matrix->columns + i ) );
    }
    
    return new;
}


BGM_matrix_t *BGM_negative( BGM_matrix_t *matrix )
{
    
    BGM_matrix_t *new = BGM_clone( matrix );
    
    if( new )
    {
        BGM_foreach( new, *(new->elements + i * new->columns + j ) *= -1.0 );
    }
    
    return new;
    
}


BGM_matrix_t *BGM_sum( BGM_matrix_t *a, BGM_matrix_t *b )
{
    BGM_matrix_t *c;
    
    if( a->lines != b->lines || a->columns != b->columns )
    {
        BGM_error = BGM_incorrectDimensions;
        return NULL;
    }
   
    if( ( c = BGM_clone( a ) ) )
    {
        BGM_foreach( c, *(c->elements + i * c->columns + j ) += *( b->elements + i * b->columns + j ) );
    }
   
    return c;
}


BGM_matrix_t *BGM_difference( BGM_matrix_t *a, BGM_matrix_t *b )
{
    BGM_matrix_t *c = NULL;
    
    if( a->lines != b->lines || a->columns != b->columns )
    {
        BGM_error = BGM_incorrectDimensions;
        return NULL;
    }
    
    if( ( c = BGM_clone( a ) ) )
    {
        BGM_foreach( c, *(c->elements + i * c->columns + j ) -= *( b->elements + i * b->columns + j ) );
    }
    
    return c;
}


BGM_matrix_t *BGM_scalarProduct( BGM_matrix_t *matrix, double k )
{
    
    BGM_matrix_t *new = BGM_clone( matrix );
    
    if( new )
    {
        BGM_foreach( new, *(new->elements + i * new->columns + j ) *= k );
    }
    
    return new;
}

BGM_matrix_t *BGM_product( BGM_matrix_t *a,BGM_matrix_t *b )
{
    BGM_matrix_t *c = NULL;
    double        count;
    uint16_t      n;
    
    if( a->columns != b->lines )
    {
        BGM_error = BGM_incorrectDimensions;
        return NULL;
    }
    
    n = a->columns;
    
    if( (c = BGM_create( a->lines , b->columns, NULL ) ) )
    {
        BGM_foreach( c,
            
            count = 0;
            
            for( uint16_t k = 0; k < n ; k++ )
            {
                count += BGM_getElement( a, i, k ) * BGM_getElement( b , k, j);
            }

            BGM_setElement( c , i , j , count );

        );
    }
    
    return c;
    
}

double BGM_determinant( BGM_matrix_t *matrix )
{
    if( matrix->lines != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return 0.0;
    }
    
    if( BGM_isTriangular(matrix) )
    {
        return BGM_diagonalProduct(matrix);
    }
    else
    {
        for( int j = 0 ; j < matrix->columns ; j++ )
        {
            for( int i = j+1; i < matrix->lines; i++ )
            {
                double a = BGM_getElement(matrix,i,j);
                double b = BGM_getElement(matrix,i-1,j);
                int sign = ( ( b < 0 && a < 0 ) || ( b >= 0 && a >= 0 ) ) ? -1 : 1;
                
                if(a)
                {
                    for( int k = j; k < matrix->columns; k++ )
                    {
                        double c = BGM_getElement(matrix, i, k );
                        double d = BGM_getElement(matrix, i-1, k );
                        
                        BGM_setElement( matrix,i, k, d + sign * ( b/a * c ) );
                        
                        
                    }
                }
            }
        }
        
        return BGM_diagonalProduct(matrix);
    }

}

/*FUNÇÕES DE APOPIO*/

bool BGM_isUpperTriangular( BGM_matrix_t *matrix )
{   
    if( matrix->lines != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return false;
    }
    
    if( matrix->lines == 0 )
    {
        BGM_error = BGM_nullMatrix;
        return false;
    }
    
    if( matrix->lines == 1)
    {
        return false;
    }
    
    
    for( uint16_t i = 0 ; i < matrix->lines ; i++ )/*testa se é triangular superior*/
    {
        for( uint16_t j = i+1 ; j < matrix->columns; j++ )
        {
            /*se pelo menos um elemento acima da diagonal principal for != 0 a matriz não é triangular superior*/
            if(  *( matrix->elements + i * matrix->lines + j ) )
            {
                return false;
            }
        }
    }
    
    return true;
}

bool BGM_isLowerTriangular( BGM_matrix_t *matrix )
{   
    
    bool isTriangular = true;
    
    if( matrix->lines != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return false;
    }

    if( matrix->lines == 0 )
    {
        BGM_error = BGM_nullMatrix;
        return false;
    }
    
    if( matrix->lines == 1)
    {
        return false;
    }
    
    for( uint16_t i = 1 ; i < matrix->lines ; i++ )/*testa se é triangular inferior*/
    {
        for( uint16_t j = 0 ; j < i; j++ )
        {
            /*se pelo menos um elemento abaixo da diagonal principal for != 0 a matriz não é triangular inferior*/
            if(  *( matrix->elements + i * matrix->lines + j ) )
            {
                return false;
            }
        }
    }
    
    return true;
}

bool BGM_isTriangular( BGM_matrix_t *matrix )
{
    return BGM_isUpperTriangular(matrix) || BGM_isLowerTriangular(matrix);
}

double BGM_diagonalProduct( BGM_matrix_t *matrix )
{
    double product = 1.0;
    
    /*verifico se o o produto em determinant é nulo, caso seja, saio do loop 
    porque todo o produto da diagonal será anulado de qualquer forma*/
    for( uint16_t i = 0 ; i < matrix->lines && product != 0 ; i++ )
    {
        product *= *( matrix->elements + i * matrix->lines + i );
    }
    
    return product;
}