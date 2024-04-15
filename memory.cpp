#include "strands.h"


template <typename T>
T ** alloc_2d_arr ( int width, int height, T fill ) {

	T ** ptr = new (nothrow) T * [ height ];

	if ( ptr == nullptr )
		return nullptr;


	ptr[ 0 ] = new (nothrow) T [ width * height ];

	if( ptr[0] == nullptr ) {
		delete [] ptr;
		return nullptr;
	}
	
	for( int i = 1; i < height; i++ )
		ptr[i] = ptr[0] + i * width;


	for( int i = 0; i < width * height; i++ )
		ptr[0][i] = fill;

	return ptr;	

}


template <typename T>
void free_2d ( T * * & ptr  ) {
	if ( ptr == nullptr )
		return;

	delete [] ptr[0];
	
	delete [] ptr;

	ptr = nullptr;
}

template char ** alloc_2d_arr<char>( int width, int height, char fill = '\0' );
template int ** alloc_2d_arr<int>( int width, int height, int fill = 0 );
template bool ** alloc_2d_arr<bool>( int width, int height, bool fill = false );
template void free_2d<bool>( bool ** & ptr );
template void free_2d<char>( char ** & ptr );
template void free_2d<int>( int ** & ptr );
