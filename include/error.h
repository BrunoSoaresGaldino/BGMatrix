
#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED



typedef enum { BGM_noError, BGM_incorrectDimensions, BGM_operationUndefined, BGM_nullMatrix } BGM_error_t;

extern BGM_error_t BGM_error;

#define BGM_CLEAR_ERROR                   ( BGM_error = 0 )
#define BGM_INCORRECT_DIMENSIONS          ( BGM_error == BGM_incorrectDimensions )
#define BGM_OPERATION_UNDEFINED           ( BGM_error == BGM_operationUndefined )
#define BGM_NULL_MATRIX                   ( BGM_error == BGM_nullMatrix)
#define BGM_NO_ERROR                      ( BGM_error == BGM_noError  )
#define BGM_ERROR                         ( BGM_error )

#endif