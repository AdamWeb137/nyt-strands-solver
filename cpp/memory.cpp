/** **********************************************************************
* @file
* @brief contains functions for dynamic memory allocation and deletion
*************************************************************************/
#include "strands.h"

/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Allocates a 2d array of a given width and height.
 *
 *  @param[in] width the width of the array
 *  @param[in] height the height of the array
 *  @param[in] fill what to initialize each item of the array to.
 *
 *  @returns a 2d pointer if allocation was successful, or nullptr if not
 *
 *  @par Example
 *  @verbatim

   	board = alloc_2d_arr<char>( width + 1, height );
	used = alloc_2d_arr<bool>( width, height );
	hint_coors = alloc_2d_arr<bool>( width, height );

    @endverbatim
************************************************************************/
template <typename T>
T ** alloc_2d_arr ( int width, int height, T fill ) {

	// allocate array of pointers
	T ** ptr = new (nothrow) T * [ height ];

	if ( ptr == nullptr )
		return nullptr;


	// allocate actual space
	ptr[ 0 ] = new (nothrow) T [ width * height ];

	if( ptr[0] == nullptr ) {
		delete [] ptr;
		return nullptr;
	}
	
	for( int i = 1; i < height; i++ )
		ptr[i] = ptr[0] + i * width;

	
	// fill array
	for( int i = 0; i < width * height; i++ )
		ptr[0][i] = fill;

	return ptr;	

}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Frees a 2d array and sets the pointer to nullptr.
 *
 *  @param[in,out] ptr the 2d array.
 *
 *
 *  @par Example
 *  @verbatim

   	free_2d<char>( board );
	free_2d<char>( words );
	free_2d<bool>( used );
	free_2d<bool>( hint_coors );

    @endverbatim
************************************************************************/
template <typename T>
void free_2d ( T * * & ptr  ) {

	// don't attempt deletion if the ptr is null
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
