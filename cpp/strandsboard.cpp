/** **********************************************************************
* @file
* @brief contains StrandsBoard functions
*************************************************************************/
#include "strands.h"


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Default constructor. sets the width to 6 and height to 8, allocates
 *  all 2d arrays, and fills the dictionary.
 *
 *  @par Example
 *  @verbatim

    StrandsBoard board;

    @endverbatim
************************************************************************/
StrandsBoard::StrandsBoard() {
	width = 6;
	height = 8;
	allocate();
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Allocates 2d arrays, sets default values, and fills the dictionary and 
 *  prefix tree.
************************************************************************/
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


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Constructs a StrandsBoard with a given width and height
 *
 *  @param[in] width the width of the strands board
 *  @param[in] height the width of the strands board
 *  int.
 *
 *  @par Example
 *  @verbatim

	int width, height;
	cout << "Input board size (width height): ";
	cin >> width >> height;
    StrandsBoard small_board( width, height );

    @endverbatim
************************************************************************/
StrandsBoard::StrandsBoard( int w, int h ) {
	width = w;
	height = h;
	allocate();	
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Constructs a StrandsBoard from a text representation of its board.
 *
 *  @param[in] board_string a vector of strings where each string is a 
 *  row of the board.
 *
 *  @par Example
 *  @verbatim

    vector<string> board_string = {
		"coke",
		"soap",
		"bomb"
	};

	StrandsBoard board( board_string );

    @endverbatim
************************************************************************/
StrandsBoard::StrandsBoard( vector<string> & board_string ) {

	// get width and height from the size of the vector
	height = (int) board_string.size();
	width = 0;
	if ( !board_string.empty() )
		width = (int) board_string[0].size();

	allocate();

	// copy the text over
	for( int i = 0; i < height; i++ ) 
		strcpy( board[i], board_string[i].c_str() );

}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Frees all of the dynamically allocated member arrays.
************************************************************************/
StrandsBoard::~StrandsBoard() {
	free_2d<char>( board );
	free_2d<char>( words );
	free_2d<bool>( used );
	free_2d<bool>( hint_coors );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns the area of the board.
 *
 *  @returns width times the height.
 *
 *  @par Example
 *  @verbatim

	for( int i = 0; i < size(); i++ ) {
		cout << board[0][i];
	}
	cout << endl;

    @endverbatim
************************************************************************/
int StrandsBoard::size() {
	return width * height;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Checks whether a coordinate is on the board
 *
 *  @param[in] x the x position to check if valid
 *  @param[in] y the y position to check if valid
 *
 *  @returns true if (x,y) is inside the board bounds, false if not
 *
 *  @par Example
 *  @verbatim

	//inside of find_all_words_from_point we return if we are out of bounds
    if ( !in_bounds( x, y ) || used[y][x] 
	|| !prefix_tree.contains( word_str ) ) return;

    @endverbatim
************************************************************************/
bool StrandsBoard::in_bounds( int x, int y ) {
	return x >= 0 && x < width && y >= 0 && y < height;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns the last word in the words array.
 *
 *  @returns last word in the words array.
************************************************************************/
string StrandsBoard::curr_word() {
	return string( words[ words_len ] );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Converts a (x,y) coordinate to a single integer
 *
 *  @param[in] x the x position.
 *  @param[in] y the y position.
 *
 *  @returns a scalar representing the original (x,y) position
 *
 *  @par Example
 *  @verbatim

    int new_width = 
	get_valid_int( "Enter a new board width: ", "Please enter a number." );

    @endverbatim
************************************************************************/
int StrandsBoard::linearize_coor( int x, int y ) {
	return x + width * y;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Sets the text of the board given a 1d string with no whitespace.
 *  If the string's length is not the size() of the board, the function
 *  just returns.
 *
 *  @param[in] new_board c style string representing the text of the board.
 *
 *  @par Example
 *  @verbatim

	StrandsBoard board( 4, 3 );
	// coke
	// soap
	// bomb
	board.set_board( "cokesoapbomb" );

    @endverbatim
************************************************************************/
void StrandsBoard::set_board( char * new_board ) {

	// if the given string is not of the wrong size, return
	if( int( strlen( new_board ) ) != size() )
		return;

	// copy the string over
	for( int i = 0; i < height; i ++ )
		memcpy( board[i], new_board + i * width, width * sizeof( char ) );

}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Return a pointer to a word in the board.
 *
 *  @param[in] index the index of the word in the found_words
 *
 *  @returns a pointer to the puzzleword.
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_found_words_amount(); i++ ) {
		cout << get_found_word( i )->word << endl;
	}

    @endverbatim
************************************************************************/
PuzzleWord * StrandsBoard::get_found_word( int index ) {
	return &found_words[index];
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Get the number of words in the board.

 *  @returns the number of words in the board.
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_found_words_amount(); i++ ) {
		cout << found_words[i] << endl;
	}

    @endverbatim
************************************************************************/
int StrandsBoard::get_found_words_amount() {
	return static_cast<int>( found_words.size() );
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns the number of solutions to the board.

 *  @returns the number of solutions to the board.
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_solution_amount(); i++ ) {
		cout << "Indicies of solution # " <<  i << endl;
		cout << get_solution( i );
	}

    @endverbatim
************************************************************************/
int StrandsBoard::get_solution_amount() {
	return static_cast<int>( solutions.size() );
}
    

/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns a jsonified string of the list of indicies of the ith solution
 *
 *  @param[in] index the index of the solution to stringify.
 *
 *  @returns the json stringified representation of the solution.
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_solution_amount(); i++ ) {
		cout << "Indicies of solution # " <<  i << endl;
		cout << get_solution( i );
	}

    @endverbatim
************************************************************************/
char *  StrandsBoard::get_solution( int index ) {

	ostringstream json;

	// print the array into a json version of the array
	int ssize = static_cast<int>( solutions[index].size() );
	json << "[";
	for( int i = 0; i < ssize; i++ ) {
		
		json << solutions[index][i];

		if( i < ssize - 1 )
			json << ", ";

	}

	json << "]";

	json_holder = json.str();

	// return c string version of the array
	return ( char * ) json_holder.c_str();
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Sets a specific coordinate of the used value to on or off.
 *
 *  @param[in] x the x coordinate to set.
 *  @param[in] y the y coordinate to set.
 *  @param[in] value what to set the coordinate to. true or false.
************************************************************************/
void StrandsBoard::set_used( int x, int y, int value ) {
	used[y][x] = value;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Descrambles a hint using the hint_coors member as input and the
 *  possible_hints as the output.
************************************************************************/
void StrandsBoard::get_hints() {
	possible_hints.clear();
	find_hint_matches( hint_coors, possible_hints );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Get the number of possible hint solutions.
 *
 *  @returns  the number of possible hint solutions.
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_hints_amount(); i++ ) {
		cout << get_hint( i ) -> word << endl;
	}

    @endverbatim
************************************************************************/
int StrandsBoard::get_hints_amount() {
	return static_cast<int>( possible_hints.size() );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Sets a coordinate in the hint_coors member.
 *
 *  @param[in] x the x coordinate to set.
 *  @param[in] y the y coordinate to set.
 *  @param[in] value what to set the coordinate to. true or false.
************************************************************************/
void StrandsBoard::set_hint_coor( int x, int y, bool value ) {
	hint_coors[y][x] = value;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns a pointer to a hint solution at a given index in possible_hints.
 *
 *  @param[in] index the index of possible_hints to get the word from.
 *
 *  @returns a pointer to the puzzle word at possible_hints[index].
 *
 *  @par Example
 *  @verbatim

    for( int i = 0; i < get_hints_amount(); i++ ) {
		cout << get_hint( i ) -> word << endl;
	}

    @endverbatim
************************************************************************/
PuzzleWord * StrandsBoard::get_hint( int index ) {
	
	return &possible_hints[ index ];

}
