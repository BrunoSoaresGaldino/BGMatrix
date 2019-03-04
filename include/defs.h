
#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED


#define BGM_swap( a , b ) { typeof( a ) c ; c = (a) ; (a) = (b) ; (b) = c; }

#define BGM_foreach( matrix , ... ) { uint16_t _l = BGM_rows(matrix) ; uint16_t _c = BGM_columns(matrix) ; for( uint16_t i = 0 ; i < _l ; i++ ){\
for( uint16_t j = 0 ; j < _c ; j++ ){  __VA_ARGS__;} } }

#define BGM_foreachColumn( matrix, ... ) { uint16_t _c = BGM_columns(matrix) ; for( uint16_t j = 0 ; j < _c ; j++ ){  __VA_ARGS__;} }

#define BGM_foreachrow( matrix, ... ) { uint16_t _l = BGM_rows(matrix) ; for( uint16_t i = 0 ; i < _l ; i++ ){  __VA_ARGS__;} }

/*Esse macete eu aprendi no blog do Frederico Pissarra https://bitismyth.wordpress.com */
#define BGM_autoDestroy __attribute__(  (cleanup(BGM_destroyP) ) )

#endif