
#include "../include/bgmatrix.h"

BGM_error_t BGM_error;

struct BGM_matrix_t
{
    double   *elements;
    uint16_t rows;
    uint16_t columns;
};

static bool BGM_checkDimensions( BGM_matrix_t *a, BGM_matrix_t *b, BGM_operation_t op )
{
    switch (op)
    {
        case add:
        case sub:

            if( a->rows != b->rows || a->columns != b->columns )
            {
                BGM_error = BGM_incorrectDimensions;
                return false;
            }
            break;
    
        case mul:

            if( a->columns != b->rows )
            {
                BGM_error = BGM_incorrectDimensions;
                return false;
            }
            break;
    }
    

    return true;
}

BGM_matrix_t *BGM_create( uint16_t rows, uint16_t columns, double *elements )
{
    
    BGM_matrix_t *matrix;
    size_t        size = rows * columns * sizeof(double);
    
    if( ( matrix = malloc( sizeof(BGM_matrix_t) ) ) )
    {
        if( ( matrix->elements = malloc( size ) ) )
        {
            matrix->rows   = rows;
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

    return BGM_create( matrix->rows, matrix->columns, matrix->elements );   

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


uint16_t BGM_rows( BGM_matrix_t *matrix )
{
    return matrix->rows;
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
        BGM_swap( new->rows, new->columns );
        BGM_foreach( new, *(new->elements + i * new->columns + j ) = *( matrix->elements + j * matrix->columns + i ) );
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
    
    if( !BGM_checkDimensions( a, b, add ) )
    {
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
    
    if( !BGM_checkDimensions( a, b, sub ) )
    {
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

BGM_matrix_t *BGM_product( BGM_matrix_t *a, BGM_matrix_t *b )
{
    BGM_matrix_t *c;
    double        count;
    uint16_t      n;
    
    if( !BGM_checkDimensions( a, b, mul )  )
    {
        return false;
    }
    
    n = a->columns;
    
    if( (c = BGM_create( a->rows , b->columns, NULL ) ) )
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
    double determinant;
    
    if( !(matrix = BGM_clone( matrix ) ) )
    {
        return 0.0;
    }

    if( matrix->rows != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return 0.0;
    }
    
    if( matrix->rows == 2 )
    {
        /*O determinante de uma matriz 2x2 é dado pela diferença do produto da diagonal principal com o produto da diagonal secundária*/
        
        return matrix->elements[0] * matrix->elements[3] - matrix->elements[1] * matrix->elements[2];
    }

    if( !BGM_isTriangular(matrix) )/*se a matriz não for triangular faz-se uma série de operações até transformála em uma*/
    {
        for( uint16_t j = 0; j < matrix->columns - 1; j++ )
        {
            double elementA = BGM_getElement( matrix, j, j );

            for( uint16_t i = j + 1; i < matrix->rows; i++  )
            {
                double elementB = BGM_getElement( matrix, i, j );
                double factor;

                if( elementB != 0 )/*se o elemento já é 0 não é necessário fazer nada*/
                {
                    factor = - elementB / elementA;
        
                    for( uint16_t col = 0; col < matrix->columns; col++ )
                    {
                        double a = BGM_getElement( matrix, j, col );
                        double b = BGM_getElement( matrix, i, col );
                        double c = factor * a + b;
                        
                        BGM_setElement( matrix, i, col , c );
                    }
                }
            }
        }
    }

    determinant = BGM_diagonalProduct(matrix);
    BGM_destroy( matrix );

    return determinant;
}

/*FUNÇÕES DE APOPIO*/

bool BGM_isUpperTriangular( BGM_matrix_t *matrix )
{   
    if( matrix->rows != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return false;
    }
    
    if( matrix->rows == 0 )
    {
        BGM_error = BGM_nullMatrix;
        return false;
    }
    
    if( matrix->rows == 1)
    {
        return false;
    }
    
    
    for( uint16_t i = 0 ; i < matrix->rows ; i++ )/*testa se é triangular superior*/
    {
        for( uint16_t j = i+1 ; j < matrix->columns; j++ )
        {
            /*se pelo menos um elemento acima da diagonal principal for != 0 a matriz não é triangular superior*/
            if(  *( matrix->elements + i * matrix->rows + j ) )
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
    
    if( matrix->rows != matrix->columns )
    {
        BGM_error = BGM_operationUndefined;
        return false;
    }

    if( matrix->rows == 0 )
    {
        BGM_error = BGM_nullMatrix;
        return false;
    }
    
    if( matrix->rows == 1)
    {
        return false;
    }
    
    for( uint16_t i = 1 ; i < matrix->rows ; i++ )/*testa se é triangular inferior*/
    {
        for( uint16_t j = 0 ; j < i; j++ )
        {
            /*se pelo menos um elemento abaixo da diagonal principal for != 0 a matriz não é triangular inferior*/
            if(  *( matrix->elements + i * matrix->rows + j ) )
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
    
    /*verifico se o o produto é nulo, caso seja, saio do loop 
    porque todo o produto da diagonal será anulado de qualquer forma*/
    for( uint16_t i = 0 ; i < matrix->rows && product != 0 ; i++ )
    {
        product *= *( matrix->elements + i * matrix->rows + i );
    }
    
    return product;
}

void BGM_scaleRow( BGM_matrix_t *matrix, double k,uint16_t row )
{
    for( uint16_t j = 0; j < matrix->columns; j++ )
    {
        *(matrix->elements + row *matrix->columns + j ) *= k;
    }
}

void BGM_addRows( BGM_matrix_t *matrix, uint16_t rowDest, uint16_t rowSource )
{
    for( uint16_t j = 0; j < matrix->columns; j++ )
    {
        *(matrix->elements + rowDest *matrix->columns + j ) += *(matrix->elements + rowSource *matrix->columns + j );
    }
}