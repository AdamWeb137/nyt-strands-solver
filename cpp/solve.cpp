#include "strands.h"

void solve_strands_old_word( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int x, int y, int chars_used ) {


	// base cases:
	// we have filled the whole board.
	/*if ( chars_used >= board.size() ) {
		if ( board.curr_word_len < 4 )
			return;
		//board.words[board.words_len][board.curr_word_len] = '\0';
		board.words_len++;
		board.print_solution( std::cout );
		board.words_len--;
		return;
	}*/

	// out of bounds or on used square
	if ( ( !board.in_bounds( x, y ) || board.used[y][x] || board.words_len >= board.max_words ) && chars_used != board.size() )
		return;

	// if we have a possible word

	/*if ( chars_used >= board.size() ) {
		//board.print_solution( std::cout );
		return;
	}*/

	board.used[y][x] = true;
	board.words[board.words_len][board.curr_word_len] = board.board[y][x];
	board.curr_word_len++;

	if( wordtree.contains( board.curr_word() ) ) {
	
		// we have filled the whole board.
		/*if ( chars_used >= board.size() - 1 ) {
			board.print_solution( std::cout );
			return;
		}*/

		if ( chars_used < board.size() - 1 ) {

			int moves[][2] = {
				{ 1, 1 },
				{ 1, -1 },
				{ -1, 1 },
				{ -1, -1 },
				{ 0, 1 },
				{ 1, 0 },
				{ -1, 0},
				{ 0, -1 }
			};

			for ( auto move : moves ) 
				solve_strands_old_word( board, words, wordtree, x + move[ 0 ], y + move[ 1 ], chars_used + 1 );

		} else
			solve_strands_old_word( board, words, wordtree, x, y, chars_used + 1 );

	}

	board.used[y][x] = false;
	board.curr_word_len--;
	board.words[board.words_len][board.curr_word_len] = '\0';	

	solve_strands_new_word( board, words, wordtree, chars_used );

}

void solve_strands_new_word ( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int chars_used ) {

	if ( chars_used > -1 && board.curr_word_len < 4) return;
	if( chars_used > -1 && !words.contains( board.curr_word() ) ) return;

	if ( chars_used == -1 ) chars_used = 0;


	//board.words[board.words_len][board.curr_word_len] = '\0';

	if ( board.words_len > 0 || board.curr_word_len > 0)
		board.words_len++;
	int curr_word_len = board.curr_word_len;

	board.print_solution( std::cout );

	if ( chars_used >= board.size() ) 
		board.print_solution( std::cout );
	// try to find a new word at every locaton
	else for( int ny = 0; ny < board.height; ny++ ) {
		for( int nx = 0; nx < board.width; nx++ ) {
			board.curr_word_len = 0;
			solve_strands_old_word( board, words, wordtree, nx, ny, chars_used );
		}
	}

	// change the words in the solution back here
	board.curr_word_len = curr_word_len;
	if( board.words_len > 0 )
		board.words_len--;

}

void solve_strands( StrandsBoard & board, set<string> & words, LetterNode & wordtree ) {
	solve_strands_new_word( board, words, wordtree, -1 );
}
