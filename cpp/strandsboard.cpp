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
	hint_coors = alloc_2d_arr<bool>( width, height );

	get_words( dictionary, prefix_tree );

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
	free_2d<bool>( hint_coors );
}


int StrandsBoard::size() {
	return width * height;
}

bool StrandsBoard::in_bounds( int x, int y ) {
	return x >= 0 && x < width && y >= 0 && y < height;
}


string StrandsBoard::curr_word() {
	return string( words[ words_len ] );
}

int StrandsBoard::linearize_coor( int x, int y ) {
	return x + width * y;
}

void StrandsBoard::set_board( char * new_board ) {

	if( strlen( new_board ) != size() )
		return;

	for( int i = 0; i < height; i ++ )
		memcpy( board[i], new_board + i * width, width * sizeof( char ) );

}


PuzzleWord * StrandsBoard::get_found_word( int index ) {
	return &found_words[index];
}


int StrandsBoard::get_found_words_amount() {
	return static_cast<int>( found_words.size() );
}


int StrandsBoard::get_solution_amount() {
	return static_cast<int>( solutions.size() );
}
    
char *  StrandsBoard::get_solution( int index ) {

	ostringstream json;
	int ssize = static_cast<int>( solutions[index].size() );
	json << "[";
	for( int i = 0; i < ssize; i++ ) {
		
		json << solutions[index][i];

		if( i < ssize - 1 )
			json << ", ";

	}

	json << "]";

	json_holder = json.str();

	return ( char * ) json_holder.c_str();
}


void StrandsBoard::set_used( int x, int y, int value ) {
	used[y][x] = value;
}


void StrandsBoard::get_hints() {
	possible_hints.clear();
	find_hint_matches( hint_coors, possible_hints );
}


int StrandsBoard::get_hints_amount() {
	return static_cast<int>( possible_hints.size() );
}


void StrandsBoard::set_hint_coor( int x, int y, bool value ) {
	hint_coors[y][x] = value;
}


PuzzleWord * StrandsBoard::get_hint( int index ) {
	
	return &possible_hints[ index ];

}
