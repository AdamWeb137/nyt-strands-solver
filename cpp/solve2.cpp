#include "strands.h"

void StrandsBoard::find_all_words_from_point( char * word_str, int ** coors, set<string> & already_found, int x, int y, int word_len ) {

	// if we are out of bounds or if the word is not a prefix of any known word,
	// we return
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



void StrandsBoard::find_all_words() { 

	found_words.clear();

	PuzzleWord::height = height;
	PuzzleWord::width = width;

	set<string> already_found;

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

	delete [] word_str;
	free_2d<int>( coors );

}

void StrandsBoard::find_solution_from_words_rec( vector<int> & indicies, int total_chars, int & depth, int max_depth ) {
	
	// if we have searched too long or we have used too many characters
	if( depth >= max_depth || total_chars > size() ) return;

	if( total_chars == size() ) {
		solutions.emplace_back( indicies );
		return;
	}

	int last_index = indicies.empty() ? -1 : indicies.back();

	for( int i = last_index + 1; i < static_cast<int> ( found_words.size() ); i++ ) {

		// check if word would overlap
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
		find_solution_from_words_rec( indicies, total_chars + static_cast<int>( found_words[i].word.size() ), depth, max_depth );

		// remove word from solution list
		indicies.pop_back();

	}

}

void StrandsBoard::find_solution_from_words() { 

	vector<int> indicies;
	indicies.reserve( found_words.size() );

	solutions.clear();

	int chars_used = 0;
	for( int y = 0; y < height; y++ )
		for ( int x = 0; x < width; x++ )
			chars_used += static_cast<int>( used[y][x] );

	int recursion_call_number = 0;
	find_solution_from_words_rec( indicies, chars_used, recursion_call_number, int( 1e6 ) );

}


void StrandsBoard::find_hint_matches( bool ** coors, vector<PuzzleWord> & matching ) {
	for ( auto & pw : found_words )
		if( pw.total_overlap( coors ) ) matching.emplace_back( pw );
}
