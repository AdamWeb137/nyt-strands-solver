#include "strands.h"


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Finds all english words in a NYT Strands board by recursively
 *  finding every valid path on the board and checks if the path corresponds
 *  to a valid english word. If so, the word is added to the found_words
 *  vector. This function is a private function
 *  and does all of the actual work recursively. The public equivalient
 *  of this function is StrandsBoard::find_all_words.
 *
 *  @param[in, out] word_str the c string representing the current working
 *  word.
 *  @param[in, out] coors a 2d array of integers representing which spots
 *  on the board have been visited and in what order.
 *  @param[in, out] already_found keeps track of what words have been found
 *  on the board so far.
 *  @param[in] x the current x position of the working word on the board.
 *  @param[in] y the current y position of the working word on the board.
 *  @param[in] word_len the current length of the working word.
 *
 *
 *  @par Example
 *  @verbatim

    set<string> already_found;

	// allocate array to keep track of used spots on the board
	int ** coors = alloc_2d_arr<int>( width, height, 0 );
	if( coors == nullptr ) exit( 1 );

	// allocate c string for individual solutions
	char * word_str = new (nothrow) char [ size() + 1 ];
	if ( word_str == nullptr ) exit( 1 );
	for( int i = 0; i <= size(); i++ )
		word_str[i] = '\0';

	// get all words starting from each place on the board
	for( int y = 0; y < height; y++ )
		for ( int x = 0; x < width; x++ )
			find_all_words_from_point( word_str, coors, already_found, x, y, 0 );

    @endverbatim
************************************************************************/
void StrandsBoard::find_all_words_from_point ( 
	char * word_str, 
	int ** coors, 
	set<string> & already_found, 
	int x, 
	int y, 
	int word_len 
) {

	// if we are out of bounds or if the word is not a prefix of any known word,
	// or if the position is already in use in our working word,
	// we return from the function
	if ( !in_bounds( x, y ) || used[y][x] || !prefix_tree.contains( word_str ) ) return;

	word_str[ word_len ] = board[y][x];
	word_len++;
	coors[y][x] = word_len;
	used[y][x] = true;

	// insert a found word if it is a solution
	string curr_sol = string( word_str );
	if ( word_len >= MIN_WORD_LEN && dictionary.contains( curr_sol ) ) {


		int overlap = false;

		// ensure we don't find two of the same word with the same letters in the same places
		// however, it is fine if we find two of the same word but they share different shapes on the board
		if( already_found.contains( curr_sol ) ) {
			for( auto & pw : found_words ) {
				if( pw.word == curr_sol && pw.total_overlap( coors ) ) {
					overlap = true;
					break;
				}
			}
		}

		if( !overlap ) {

			found_words.emplace_back( curr_sol, coors );
			already_found.insert( curr_sol );

		}

	}

	int moves[][2] = {
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 },
		{ 1, 1 },
		{ 1, -1 },
		{ -1, 1 },
		{ -1, -1 }
	};

	for( auto move : moves )
		find_all_words_from_point( word_str, coors, already_found, x + move[0], y + move[1], word_len );

	// mark the node as unvisited
	word_len--;
	word_str[ word_len ] = '\0';
	used[y][x] = false;
	coors[y][x] = 0;

}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Clears the list of words previously found on the board and
 *  finds all english words in a NYT Strands board by recursively
 *  finding every valid path on the board and checks if the path corresponds
 *  to a valid english word. If so, the word is added to the found_words
 *  vector. This function is a public function that calls
 *  StrandsBoard::find_all_words_from_point to do all of the work.
 *  The list of words is then sorted in order of decreasing length.
 *
 *  @par Example
 *  @verbatim

	vector<string> board_string = {
		"bee",
		"cat",
		"dog"
	};

	StrandsBoard board( board_string );

	board.find_all_words();

	cout << "All words found: " << end;
	for( auto & puzzle_word : board.found_words )
		cout << puzzle_word.word << endl;

    @endverbatim
************************************************************************/
void StrandsBoard::find_all_words() { 

	// clear any words found previously
	found_words.clear();

	PuzzleWord::height = height;
	PuzzleWord::width = width;

	set<string> already_found;

	// allocate array to keep track of used spots on the board
	int ** coors = alloc_2d_arr<int>( width, height, 0 );
	if( coors == nullptr ) exit( 1 );

	// allocate c string for individual solutions
	char * word_str = new (nothrow) char [ size() + 1 ];
	if ( word_str == nullptr ) exit( 1 );
	for( int i = 0; i <= size(); i++ )
		word_str[i] = '\0';

	// get all words starting from each place on the board
	for( int y = 0; y < height; y++ )
		for ( int x = 0; x < width; x++ )
			find_all_words_from_point( word_str, coors, already_found, x, y, 0 );

	// sort the found words by length
	sort( found_words.begin(), found_words.end() );

	// free up dynamically allocated memory
	delete [] word_str;
	free_2d<int>( coors );

}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Recursively finds a solution to the NYT Strands board by finding 
 *  combinations of found words on the board that don't overlap and use
 *  every space on the board. The recursion will stop after a specified
 *  number of calls lest the function run forver. The solutions are stored
 *  in the solutions member function which contains vectors of indicies
 *  of the found_words member which contain the words of the solution.
 *  
 *  This is a private function that actually does the hard recursive work.
 *  This function is used by the public function 
 *  StrandsBoard::find_solution_from_words.
 *
 *  @param[in, out] indicies the vector of indicies linking to the words
 *  in the working solution via the found_words member.
 *  @param[in] total_chars the number of characters of the board used
 *  in the working solution.
 *  @param[in,out] depth the number of times the recursive function
 *  has been called so far.
 *  @param[in] max_depth the maximum number of times the recursive function
 *  may be called.
 *
 *
 *  @par Example
 *  @verbatim

    vector<int> indicies;
	indicies.reserve( found_words.size() );

	int recursion_call_number = 0;
	find_solution_from_words_rec ( 
		indicies, 
		0, 
		recursion_call_number, 
		int( 1e6 ) 
	);

    @endverbatim
************************************************************************/
void StrandsBoard::find_solution_from_words_rec ( 
	vector<int> & indicies, 
	int total_chars, 
	int & depth, 
	int max_depth 
) {
	
	// if we have searched too long or we have used too many characters
	if( depth >= max_depth || total_chars > size() ) return;

	// if we have used all the characters on the board, we have found a solution
	if( total_chars == size() ) {
		solutions.emplace_back( indicies );
		return;
	}

	// find the index of the last word used in our working solution
	// we only consider words with an index greater than this last word
	// in our working solution as to find combinations rather than permutations
	int last_index = indicies.empty() ? -1 : indicies.back();

	for( int i = last_index + 1; i < get_found_words_amount(); i++ ) {

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

		// add word to solution list
		indicies.push_back(i);

		depth++;
		find_solution_from_words_rec ( 
			indicies, 
			total_chars + static_cast<int>( found_words[i].word.size() ), 
			depth, 
			max_depth
		);

		// remove word from solution list
		indicies.pop_back();

	}

}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Recursively finds a solution to the NYT Strands board by finding 
 *  combinations of found words on the board that don't overlap and use
 *  every space on the board. The recursion will stop after a million
 *  number of calls lest the function run forver. The solutions are stored
 *  in the solutions member function which contains vectors of indicies
 *  of the found_words member which contain the words of the solution.
 *  The function StrandsBoard::find_all_words should be called before
 *  this one can actual do anything.
 *  
 *
 *  @par Example
 *  @verbatim

    vector<string> board_string = {
		"bee",
		"cat",
		"dog"
	};

	StrandsBoard board( board_string );

	board.find_all_words();

	board.find_solution_from_words();

	int i = 0;

	for( auto & solution : solutions ) {
		cout << "Solution: #" << ++i << endl;
		for( auto & idx : solution ) {
			cout << board.found_words[ idx ] << endl;
		}
	}

    @endverbatim
************************************************************************/
void StrandsBoard::find_solution_from_words() { 

	vector<int> indicies;
	indicies.reserve( found_words.size() );

	solutions.clear();

	// initialize the chars_used variable
	// however many characters are already marked as used
	int chars_used = 0;
	for( int y = 0; y < height; y++ )
		for ( int x = 0; x < width; x++ )
			chars_used += static_cast<int>( used[y][x] );

	// create a variable to keep track of number of recursive calls
	// we quit the recurssion after this variable reaches an arbitrary
	// large number such as a million.
	int recursion_call_number = 0;
	find_solution_from_words_rec ( 
		indicies, 
		chars_used, 
		recursion_call_number, 
		int( 1e6 ) 
	);

}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Finds all words in a NYT Strands board that could match a hint.
 *
 *  @param[in] coors a 2d boolean array representing positions where the
 *  hint has a letter.
 *  @param[out] matching a vector of words (PuzzleWords) that the hint could
 *  be.
 *
 *  @returns the grayscale value as a pixel
 *
 *  @par Example
 *  @verbatim

    vector<string> board_string = {
		"bee",
		"cat",
		"dog"
	};

	StrandsBoard board( board_string );

	bool hint_coors[3][3] = {
		{1,0,0},
		{0,1,1},
		{0,0,0}
	};

	vector<PuzzleWord> matching;

	board.find_hint_matches( hint_coors, matching );

	for( auto & pw : matching )
		cout << pw.word << endl; // bat and tab

    @endverbatim
************************************************************************/
void StrandsBoard::find_hint_matches( bool ** coors, vector<PuzzleWord> & matching ) {
	// loop through every found word and if it's coordinates match up with
	// the given coordinates, add a copy of it to the maching vector
	for ( auto & pw : found_words )
		if( pw.total_overlap( coors ) ) matching.emplace_back( pw );
}
