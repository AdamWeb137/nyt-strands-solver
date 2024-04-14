#include "strands.h"

StrandsBoard::StrandsBoard() {
	width = 6;
	height = 8;
	allocate();
}


void StrandsBoard::allocate() {
	if ( size() == 0 ) return;
	// allocate the board with width + 1 for null terminator
	board = alloc_2d_arr<char>( width + 1, height );
	int max_words = width * height / 3 + 1;
	// again, we add one for the null terminator
	words = alloc_2d_arr<char>( width * height + 1, max_words );
	// set number of words to zero
	words_len = 0;
	// and set the length of the first word to zero
	curr_word_len = 0;
	used = alloc_2d_arr<bool>( width, height );
}


StrandsBoard::StrandsBoard( int w, int h ) {
	width = w;
	height = h;
	allocate();	
}


StrandsBoard::StrandsBoard( vector<string> & board_string ) {
	height = (int) board_string.size();
	width = 0;
	if ( !board_string.empty() )
		width = (int) board_string[0].size();

	allocate();

	for( int i = 0; i < height; i++ ) 
		strcpy( board[i], board_string[i].c_str() );

}


StrandsBoard::~StrandsBoard() {
	free_2d<char>( board );
	free_2d<char>( words );
	free_2d<bool>( used );
}


int StrandsBoard::size() {
	return width * height;
}

bool StrandsBoard::in_bounds( int x, int y ) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

void StrandsBoard::print_solution( ostream & out ) {
	const char * LINE = "-----------\n";
	out << LINE;
	for ( int i = 0; i < words_len; i++ )
		out << words[i] << "\n\n";
	out << LINE;
}

string StrandsBoard::curr_word() {
	return string( words[ words_len ] );
}

int StrandsBoard::linearize_coor( int x, int y ) {
	return x + width * y;
}
