/** **********************************************************************
* @file
* @brief contains PuzzleWord implementation
*************************************************************************/
#include "strands.h"



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Default constructor of PuzzleWord. sets the coordinate field to 
 *  nullptr.
 *
 *  @par Example
 *  @verbatim

    PuzzleWord pw;

    @endverbatim
************************************************************************/
PuzzleWord::PuzzleWord() {
	coordinates = nullptr;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Constructor that takes in a c++ string for the word and a 2d array
 *  for the coordinates of the word which gets copied into the coordinates
 *  field.
 *
 *  @param[in] w the word of the puzzle word
 *  @param[in] coor_original a 2d array marking the path that the word
 *  covers on the board. 
 *
 *  @par Example
 *  @verbatim

    // the board is
	// coke
	// soap
	// bomb

	int coors[3][4] = {
		{0,0,0,0},
		{0,2,0,0},
		{1,0,3,4}
	};

	PuzzleWord bomb1( "bomb", coors );

    @endverbatim
************************************************************************/
PuzzleWord::PuzzleWord( string w, int ** coor_original ) {

	word = w;

	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	if( coordinates == nullptr ) {
		std::cerr << "Failed to allocate memory\n";
		exit ( 1 );
		return;
	}

	// deep copy of coordinates
	memcpy( coordinates[0], coor_original[0], width * height * sizeof( int ) );

}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Copy constructor for the PuzzleWord struct. Allows passing by value.
 *
 *  @param[in] other the puzzleword to copy.
 *
 *  @par Example
 *  @verbatim

	int coors[3][4] = {
		{0,0,0,0},
		{0,2,0,0},
		{1,0,3,4}
	};

    PuzzleWord bomb1( "bomb", coors );

	PuzzleWord bomb2( bomb1 );

    @endverbatim
************************************************************************/
PuzzleWord::PuzzleWord( PuzzleWord & other ) {
	word = other.word;
	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	memcpy( coordinates[0], other.coordinates[0], width * height * sizeof(int) );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Moves the word and coordinate of a puzzle word into another word. The 
 *  moved puzzle word no longer contains the original data.
 *
 *  @param[in,out] other the puzzleword to move data from.
 *
 *  @par Example
 *  @verbatim

	int coors[3][4] = {
		{0,0,0,0},
		{0,2,0,0},
		{1,0,3,4}
	};

    PuzzleWord temp( "bomb", coors );

	PuzzleWord bomb;

	bomb.move( temp );

    @endverbatim
************************************************************************/
void PuzzleWord::move( PuzzleWord & other ) {
	word.swap( other.word );
	coordinates = other.coordinates;
	other.coordinates = nullptr;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Move constructor for the PuzzleWord struct.
 *
 *  @param[in,out] other the puzzleword to move data to.
************************************************************************/
PuzzleWord::PuzzleWord( PuzzleWord && other ) {
	move( other );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Swaps the word and coordinates field between two PuzzleWord structs.
 *
 *  @param[in,out] a the first PuzzleWord.
 *  @param[in,out] b the second PuzzleWord.
 *
 *  @par Example
 *  @verbatim

	int coors[3][4] = {
		{0,0,0,0},
		{0,2,0,0},
		{1,0,3,4}
	};

	int coors2[3][4] = {
		{0,0,0,0},
		{0,0,0,0},
		{1,2,3,4}
	};

    PuzzleWord bomb1( "bomb", coors );

	PuzzleWord bomb2( "bomb", coors2 );

	swap( bomb1, bomb2 );

    @endverbatim
************************************************************************/
void swap( PuzzleWord & a, PuzzleWord & b ) {
	a.word.swap( b.word );
	swap( a.coordinates, b.coordinates );
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Copy assignment for the PuzzleWord class.
 *
 *  @param[in] other the puzzleword to assign from.
 *
 *  @returns a reference to the copied puzzle word.
 *
 *  @par Example
 *  @verbatim

    int coors[3][4] = {
		{0,0,0,0},
		{0,2,0,0},
		{1,0,3,4}
	};

    PuzzleWord bomb1( "bomb", coors );

	PuzzleWord bomb2 = bomb1;

    @endverbatim
************************************************************************/
PuzzleWord& PuzzleWord::operator=(PuzzleWord& other) {
	// only perform the operation if the two structs are different
	if( this != &other ) {
		// create use the copy constructor then swap the temporary
		PuzzleWord temp( other );
		swap( *this, temp );
	}
	return *this;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Move assignment for the PuzzleWord class.
 *
 *  @param[in,out] other the puzzleword to assign from.
 *
 *  @returns a reference to the puzzle word the data moved to.
************************************************************************/
PuzzleWord& PuzzleWord::operator=(PuzzleWord&& other) {
	if( this != &other )
		move( other );
	return *this;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Deconstructor for the PuzzleWord struct. Frees up the coordinates field.
************************************************************************/
PuzzleWord::~PuzzleWord() {
	free_2d( coordinates );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Checks if the locations of two words on a NYT Strands board overlap
 *  at all.
 *
 *  @param[in] other the puzzle word to check for overlap with
 *
 *  @returns true if the words overlap, false if not
 *
 *  @par Example
 *  @verbatim

    // check if word overlaps with words in our working solution
	// if so it cannot be part of the solution
	bool overlapping = false;
	
	for( auto idx : indicies ) {
		if( found_words[i].overlap( found_words[ idx ] ) ) {
			overlapping = true;
			break;
		}
	}
	

	if( overlapping ) continue;

    @endverbatim
************************************************************************/
bool PuzzleWord::overlap( PuzzleWord & other ) {
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int c1 = coordinates[y][x];
			int c2 = other.coordinates[y][x];
			// if both coordinates are non-zero there is an overlap
			if ( c1 != 0 && c2 != 0 ) return true;
		}
	}
	return false;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Checks if two puzzle words share the exact same letters on the exact
 *  same positions.
 *
 *  @param[in] coors 2d array which marks which letters on the board are used
 *  on another puzzle word. Can either be a 2d array of ints like the coordinates
 *  field or a boolean array.
 *
 *  @returns true if the puzzle words share the exact same locations, false
 *  if not.
 *
 *  @par Example
 *  @verbatim

	// the board is
	// coke
	// soap
	// bomb

	vector<PuzzleWord> matches;

	bool hint_coors[3][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,1}
	};

    for ( auto & pw : found_words )
		if( pw.total_overlap( hint_coors ) ) matching.emplace_back( pw );

    @endverbatim
************************************************************************/
template <typename T>
bool PuzzleWord::total_overlap ( T ** coors ) {
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int c1 = coordinates[y][x] ? 1 : 0;
			int c2 = coors[y][x] ? 1 : 0;
			// every coordinate must match
			// otherwise return false
			if ( c1 != c2 ) return false;
		}
	}
	return true;
}

template bool PuzzleWord::total_overlap<int>( int ** coors ); 
template bool PuzzleWord::total_overlap<bool>( bool ** coors ); 


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Compares two puzzle words based on the length of the word.
 *
 *  @param[in] other the second puzzle word to compare.
 *
 *  @returns true if the puzzle word calling the function is longer,
 *  else false.
 *
 *  @par Example
 *  @verbatim

    PuzzleWord longer, shorter;
	longer.word = "longer";
	shorter = "shrt";

	if( longer.comp( shorter ) )
		cout << "longer is longer\n";
	else
		cout << "shorter is shorter or they are the same length\n";

    @endverbatim
************************************************************************/
bool PuzzleWord::comp( const PuzzleWord & other ) const {
	return word.size() > other.word.size();
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Compares two puzzle words based on the length of the word.
 *
 *  @param[in] lhs the puzzle word on the left.
 *  @param[in] rhs the puzzle word on the right
 *
 *  
 *
 *  @returns true if the left hand side is longer, else false.
 *
 *  @par Example
 *  @verbatim

    PuzzleWord longer, shorter;
	longer.word = "longer";
	shorter = "shrt";

	if( longer < shorter )
		cout << "longer is longer\n";
	else
		cout << "shorter is shorter or they are the same length\n";

    @endverbatim
************************************************************************/
bool operator < ( const PuzzleWord & lhs, const PuzzleWord & rhs ) {
	return lhs.comp( rhs );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Get the c string representation of the word.
 *
 *  @returns the a c string version of the word field.
************************************************************************/
char * PuzzleWord::get_word() {
	return (char * ) word.c_str();
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Gets which letter of the word is at position (x,y) on the board.
 *
 *  @param[in] x the x position of the board to check.
 *  @param[in] y the y position of the board to check.
 *
 *  @returns the number position of the letter of the word at (x,y). Returns
 *  0 if that position is not in the word.
************************************************************************/
int PuzzleWord::get_coordinate( int x, int y ) {
	return coordinates[y][x];
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Print the path the word takes on the board to some ostream.
 *
 *  @param[in,out] out the ostream to print to
 *
 *  @par Example
 *  @verbatim

    std::cout << "There are multiple instances of " << word << " on the board\n\n";
	for( size_t i = 0; i < candidate_indicies.size(); i++ ) {
		std::cout << "Location # " << i << "\n";
		found_words[ candidate_indicies[i] ].print_coors( std::cout );
		std::cout << "\n";
	}

    @endverbatim
************************************************************************/
void PuzzleWord::print_coors ( ostream & out ) {
	out << right;
	for( int y = 0; y < height; y++ ) {
		for( int x = 0; x < width; x++ ) {
			out << setw(3) << coordinates[y][x];
		}
		out << "\n";
	}
}