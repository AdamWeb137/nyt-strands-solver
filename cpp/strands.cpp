#include "strands.h"

int main( int argc, char ** argv ) {
	menu();
	return 0;
}

int get_valid_int( const char * prompt_message, const char * error_message ) {

	int result;

	std::cout << prompt_message;
	while( !( cin >> result ) ) {

		cin.clear ();
 		cin.ignore ( std::numeric_limits<std::streamsize>::max (), '\n' );
		
		std::cout << "\n" << error_message << "\n" << prompt_message;

	}

	return result;

}

bool attempt_valid_int( int & result ) {
	if ( cin >> result ) return true;
	cin.clear ();
 	cin.ignore ( std::numeric_limits<std::streamsize>::max (), '\n' );
	return false;
}

bool get_board( vector<string> & board_string, int width, int height ) {
	vector<string> candidate;
	string row;
	for( int i = 0; i < height && cin >> row; i++ ) {
		if( static_cast<int>( row.size() ) != width ) return false;
		candidate.push_back(row);
	}
	board_string = candidate;
	return true;
}

void find_word_start( PuzzleWord & pw, int & startx, int & starty ) {
	for( starty = 0; starty < PuzzleWord::height; starty++ )
		for( startx = 0; startx < PuzzleWord::width; startx++ )
			if( pw.coordinates[starty][startx] == 1 ) return;
}


void print_puzzle_word( PuzzleWord & pw, ostream & out ) {
	int startx, starty;
	out << left;
	find_word_start( pw, startx, starty );
	out << setw( 20 ) << pw.word << "starting at (" << startx << ", " << starty << ")\n";
}

void print_words( vector<PuzzleWord> & found_words, ostream & out ) {
	for( auto it = found_words.rbegin(); it != found_words.rend(); it++ ) 
		print_puzzle_word( *it, out );
}

void StrandsBoard::print_solutions( ostream & out ) { 
	
	if( solutions.size() == 0 ) {
		out << "No solutions found\n\n";
		return;
	}

	for( size_t i = 0; i < solutions.size(); i++ ) {
		out << "Possible solution # " << i << "\n";
		for( auto idx : solutions[i] ) {
			print_puzzle_word( found_words[idx] );
		}
		out << "\n";
	}

}

void decramble_hint( StrandsBoard & board ) {

	vector<PuzzleWord> solutions;
	
	bool ** coors = alloc_2d_arr<bool>( PuzzleWord::width, PuzzleWord::height, 0 );

	vector<string> hint_string;
	do {
		std::cout << "insert bool array for hint\n\n";
	} while( !get_board( hint_string, PuzzleWord::width, PuzzleWord::height ) );

	for( int y = 0; y < PuzzleWord::height; y++ ) {
		for( int x = 0; x < PuzzleWord::width; x++ ) {
			coors[y][x] = static_cast<bool>( hint_string[y][x] - '0' );
		}
	}

	board.find_hint_matches( coors, solutions );

	std::cout << "Candidates for the hint are: \n";
	print_words( solutions );
	std::cout << "\n";

}

void StrandsBoard::delete_word( string word ) {
	vector<int> candidate_indicies;

	for( size_t i = 0; i < found_words.size(); i++ )
		if( found_words[i].word == word ) candidate_indicies.push_back( static_cast<int> ( i ) );

	if( candidate_indicies.empty() ) {
		std::cout << "There are no words on the board matching that\n";
		return;
	}

	int choice;

	if ( candidate_indicies.size() > 1 ) {
		std::cout << "There are multiple instances of " << word << " on the board\n\n";
		for( size_t i = 0; i < candidate_indicies.size(); i++ ) {
			std::cout << "Location # " << i << "\n";
			found_words[ candidate_indicies[i] ].print_coors( std::cout );
			std::cout << "\n";
		}
		std::cout << "Please select one to delete: ";
		cin >> choice;
		std::cout << "\n";
		if ( choice < 0 || choice > int( candidate_indicies.size() ) ) {
			std::cout << "Invalid option\n\n";
			return;
		}

	} else 
		choice = 0;

	for( int y = 0; y < height; y++ ) 
		for( int x = 0; x < width; x++ ) 
			if( found_words[ candidate_indicies[choice] ].coordinates[y][x] ) used[y][x] = true;


	PuzzleWord pw = found_words[ candidate_indicies[choice] ];
	for( auto it = found_words.begin(); it != found_words.end(); ) {
		if( it->overlap( pw ) ) {
			it = found_words.erase( it );
		} else it++;
	}

	std::cout << "The word has been deleted\n\n";

}

void menu() {
	
	int width = 6;
	int height = 8;

	std::cout << "Enter board width: ";
	cin >> width;
	std::cout << "\nEnter board height: ";
	cin >> height;
	std::cout << "\n";

	vector<string> board_string;

	do {
		std::cout << "Enter the board with whitespace seperating each row\n\n";
	} while ( !get_board( board_string, width, height ) );

	std::cout << "\n";

	StrandsBoard board( board_string );

	board.find_all_words();

	int option = -1;

	while( option != 5 ) {

		std::cout << "(1) - print all words in descending order of length\n"
			<< "(2) - try to find solution\n"
			<< "(3) - descramble a hint\n"
			<< "(4) - mark a word as used\n"
			<< "(5) - exit\n";
		std::cout << "Choose an option: ";
		cin >> option;
		std::cout << "\n";

		string word_to_delete;

		switch( option ) {
			case 1:
				print_words( board.found_words );
				std::cout << "\nThere are a total of " << board.found_words.size() << " possible words left\n\n";
				break;

			case 2:

				board.find_solution_from_words( );
				board.print_solutions( );

				break;

			case 3:

				decramble_hint( board );
				break;

			case 4:

				std::cout << "Type a word to mark as used: ";
				cin >> word_to_delete;
				std::cout << "\n";

				board.delete_word( word_to_delete );
				break;

			case 5:
				break;

			default:
				std::cout << "Improper option\n\n";
				break;

		}

	}


}