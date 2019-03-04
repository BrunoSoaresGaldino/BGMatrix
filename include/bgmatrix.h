
#ifndef BGMATRIX_H_INCLUDED
#define BGMATRIX_H_INCLUDED

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error.h"
#include "defs.h"

typedef struct  BGM_matrix_t BGM_matrix_t;

typedef enum { add = 0, sub, mul, scale, transpose } BGM_operation_t;

BGM_matrix_t   *BGM_create    ( uint16_t      rows, uint16_t columns, double *elements );//ok
BGM_matrix_t   *BGM_clone     ( BGM_matrix_t  *matrix );//ok
void            BGM_destroy   ( BGM_matrix_t  *matrix );//ok
void            BGM_destroyP  ( BGM_matrix_t ***matrix );//ok - utilizada por autoDestroy
double          BGM_getElement( BGM_matrix_t  *matrix, uint16_t i , uint16_t j );//ok
void            BGM_setElement( BGM_matrix_t  *matrix, uint16_t i , uint16_t j , double element );//ok
uint16_t        BGM_rows      ( BGM_matrix_t  *matrix );//ok
uint16_t        BGM_columns   ( BGM_matrix_t  *matrix );//ok


/*FUNÇÕES QUE IMPLEMENTAM AS OPERAÇÕES COM MATRIZES*/
BGM_matrix_t   *BGM_transposed   ( BGM_matrix_t *matrix );// ok
BGM_matrix_t   *BGM_negative     ( BGM_matrix_t *matrix );//ok
BGM_matrix_t   *BGM_sum          ( BGM_matrix_t *a, BGM_matrix_t *b );//ok
BGM_matrix_t   *BGM_difference   ( BGM_matrix_t *a, BGM_matrix_t *b );//ok
BGM_matrix_t   *BGM_scalarProduct( BGM_matrix_t *matrix, double k );//ok
BGM_matrix_t   *BGM_product      ( BGM_matrix_t *a,BGM_matrix_t *b );//ok
BGM_matrix_t   *BGM_inverse      ( BGM_matrix_t *a,BGM_matrix_t *b );//falta implementar
double          BGM_determinant  ( BGM_matrix_t *matrix );//ok


/*FUNÇÕES DE APOIO*/
bool   BGM_isUpperTriangular( BGM_matrix_t *matrix );
bool   BGM_isLowerTriangular( BGM_matrix_t *matrix );
bool   BGM_isTriangular     ( BGM_matrix_t *matrix );
double BGM_diagonalProduct  ( BGM_matrix_t *matrix );
void   BGM_scaleRow         ( BGM_matrix_t *matrix, double k, uint16_t row );
void   BGM_addRows          ( BGM_matrix_t *matrix, uint16_t rowDest, uint16_t rowSource );

#endif