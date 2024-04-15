#include "strands.h"

PuzzleWord::PuzzleWord() {
	coordinates = nullptr;
}


PuzzleWord::PuzzleWord( string w, int ** coor_original ) {

	word = w;

	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	if( coordinates == nullptr ) {
		std::cerr << "Failed to allocate memory\n";
		exit ( 1 );
		return;
	}

	memcpy( coordinates[0], coor_original[0], width * height * sizeof( int ) );

}

PuzzleWord::PuzzleWord( PuzzleWord & other ) {
	word = other.word;
	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	memcpy( coordinates[0], other.coordinates[0], width * height * sizeof(int) );
}


PuzzleWord::PuzzleWord( PuzzleWord && other ) {
	word.swap( other.word );
	coordinates = other.coordinates;
	other.coordinates = nullptr;
}


void swap( PuzzleWord & a, PuzzleWord & b ) {
	a.word.swap( b.word );
	swap( a.coordinates, b.coordinates );
}

PuzzleWord& PuzzleWord::operator=(PuzzleWord& other) {
	word = other.word;
	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	memcpy( coordinates[0], other.coordinates[0], width * height * sizeof(int) );
	return *this;
}

PuzzleWord& PuzzleWord::operator=(PuzzleWord&& other) {
	word.swap( other.word );
	coordinates = other.coordinates;
	other.coordinates = nullptr;
	return *this;
}


PuzzleWord::~PuzzleWord() {
	//std::cout << "deconstructor called\n";
	free_2d( coordinates );
}

bool PuzzleWord::overlap( PuzzleWord & other ) {
	/*
	for( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			std::cout << coordinates[y][x] << " ";
		}
		std::cout << endl;
	}
	std::cout << "-\n";
	for( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			std::cout << other.coordinates[y][x] << " ";
		}
		std::cout << endl;
	}
	cin.get();
	*/
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int c1 = coordinates[y][x];
			int c2 = other.coordinates[y][x];
			if ( c1 != 0 && c2 != 0 ) return true;
		}
	}
	return false;
}

void find_all_words_from_point( vector<PuzzleWord> & found_words, StrandsBoard & board, set<string> & dictionary, LetterNode & prefix_tree, char * word_str, int ** coors, int x, int y, int word_len ) {

	// if we are out of bounds or if the word is not a prefix of any known word,
	// we return
	if ( !board.in_bounds( x, y ) || board.used[y][x] || !prefix_tree.contains( word_str ) ) return;

	int lin_coor = board.linearize_coor( x, y );

	word_str[ word_len ] = board.board[y][x];
	word_len++;
	coors[y][x] = word_len;
	board.used[y][x] = true;

	// insert a found word if it is a solution
	string curr_sol = string( word_str );
	if ( word_len >= MIN_WORD_LEN && dictionary.contains( curr_sol ) ) {
		found_words.emplace_back( curr_sol, coors );
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
		find_all_words_from_point( found_words, board, dictionary, prefix_tree, word_str, coors, x + move[0], y + move[1], word_len );

	// mark the node as unvisited
	word_len--;
	word_str[ word_len ] = '\0';
	board.used[y][x] = false;
	coors[y][x] = 0;

}



void find_all_words( vector<PuzzleWord> & found_words, StrandsBoard & board, set<string> & dictionary, LetterNode & prefix_tree ) {

	PuzzleWord::height = board.height;
	PuzzleWord::width = board.width;

	int ** coors = alloc_2d_arr<int>( board.width, board.height, 0 );
	if( coors == nullptr ) exit( 1 );

	// allocate c string for individual solutions
	char * word_str = new (nothrow) char [ board.size() + 1 ];
	if ( word_str == nullptr ) exit( 1 );
	for( int i = 0; i <= board.size(); i++ )
		word_str[i] = '\0';

	// get all words starting from each place on the board
	for( int y = 0; y < board.height; y++ )
		for ( int x = 0; x < board.width; x++ )
			find_all_words_from_point( found_words, board, dictionary, prefix_tree, word_str, coors, x, y, 0 );

	delete [] word_str;
	free_2d<int>( coors );

}

void find_solution_from_words_rec( vector<PuzzleWord> & found_words, StrandsBoard & board, vector<int> & indicies, int total_chars, int & depth, int max_depth ) {
	
	// if we have searched too long or we have used too many characters
	if( depth >= max_depth || total_chars > board.size() ) return;
	//if( total_chars > board.size() ) return;

	if( total_chars == board.size() ) {
		std::cout << "found solution!\n-----\n";
		for ( auto i : indicies ) 
			std::cout << found_words[i].word << "\n";
		std::cout << depth << "\n";
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
		find_solution_from_words_rec( found_words, board, indicies, total_chars + static_cast<int>( found_words[i].word.size() ), depth, max_depth );

		// remove word from solution list
		indicies.pop_back();

	}

}

void find_solution_from_words( vector<PuzzleWord> & found_words, StrandsBoard & board ) {

	vector<int> indicies;
	indicies.reserve( found_words.size() );
	// sort found words by length
	sort( found_words.begin(), found_words.end(), []( PuzzleWord & a, PuzzleWord & b ) { return a.word.size() > b.word.size(); } );

	int chars_used = 0;
	for( int y = 0; y < board.height; y++ )
		for ( int x = 0; x < board.width; x++ )
			chars_used += static_cast<int>( board.used[y][x] );

	int recursion_call_number = 0;
	find_solution_from_words_rec( found_words, board, indicies, chars_used, recursion_call_number, int( 1e6 ) );
	std::cout << recursion_call_number << "\n";
}
