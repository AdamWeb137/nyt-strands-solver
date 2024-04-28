/** **********************************************************************
* @file
* @brief contains functions for the command line solver
*************************************************************************/
#include "strands.h"

/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Runs automatically upon running strands.o or strands.exe. Currently
 *  just wraps the menu function.
 *
 *  @returns 0 if the program ran succesfully.
************************************************************************/
int main ( ) {

	menu();

	return 0;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Prompt the user for an integer. Keep prompting until they input a valid int.
 *
 *  @param[in] prompt_message a string to print to prompt the user
 *  @param[in] error_message a string to print when the user inputs an invalid
 *  int.
 *
 *  @returns the user inputted int.
 *
 *  @par Example
 *  @verbatim

    int new_width = 
	get_valid_int( "Enter a new board width: ", "Please enter a number." );

    @endverbatim
************************************************************************/
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


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Attempts to read in an int from cin. Returns true and stores the int
 *  if the user inputs a valid int, otherwise the function clears cin
 *  and returns false.
 *
 *  @param[out] result the int to put the user input in

 *  @returns true if a valid int was inputted, false otherwise.
 *
 *  @par Example
 *  @verbatim

	int new_width;
	do { 
		cout << "Enter a new board width: " << endl;
    } while( !attempt_valid_int( new_width ) );

    @endverbatim
************************************************************************/
bool attempt_valid_int( int & result ) {
	if ( cin >> result ) return true;
	cin.clear ();
 	cin.ignore ( std::numeric_limits<std::streamsize>::max (), '\n' );
	return false;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Attempts to get a character reprentation of the Strands board. A board
 *  representation is valid if has dimensions of width by height.
 *
 *  @param[out] board_string a 2d array of characters that will represent
 *  the strands board.
 *  @param[in] width the width of the board
 *  @param[in] height the height of the board
 *
 *  @returns true if the user inputted a valid board, false if not
 *
 *  @par Example
 *  @verbatim

    vector<string> board_string;

	do {
		std::cout << "Enter the board with whitespace seperating each row\n\n";
	} while ( !get_board( board_string, width, height ) );

    @endverbatim
************************************************************************/
bool get_board( vector<string> & board_string, int width, int height ) {
	vector<string> candidate;
	string row;
	for( int i = 0; i < height && cin >> row; i++ ) {
		// if the row inputed is not of the correct width, return false
		if( static_cast<int>( row.size() ) != width ) return false;
		candidate.push_back(row);
	}
	board_string = candidate;
	return true;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Finds the (x,y) coordinate of the first letter of a word on the board.
 *
 *  @param[in] pw the puzzle word to find the first letter of
 *  @param[out] startx the x coordinate of the first letter
 *  @param[out] starty the y coordinate of the first letter
 *
 *  @par Example
 *  @verbatim

    int startx, starty;
	out << left;
	find_word_start( pw, startx, starty );
	out << setw( 20 ) << pw.word << "starting at (" 
		<< startx << ", " << starty << ")\n";

    @endverbatim
************************************************************************/
void find_word_start( PuzzleWord & pw, int & startx, int & starty ) {
	// do a linear search and stop when we find the first square
	for( starty = 0; starty < PuzzleWord::height; starty++ )
		for( startx = 0; startx < PuzzleWord::width; startx++ )
			if( pw.coordinates[starty][startx] == 1 ) return;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Prints out a word on the board and the word's starting location to 
 *  an ostream.
 *
 *  @param[in] pw the puzzle word to output.
 *  @param[in,out] out the ostream to output to.
 *
 *  @par Example
 *  @verbatim

    for( auto it = found_words.rbegin(); it != found_words.rend(); it++ ) 
		print_puzzle_word( *it, out );

    @endverbatim
************************************************************************/
void print_puzzle_word( PuzzleWord & pw, ostream & out ) {
	int startx, starty;
	out << left;
	find_word_start( pw, startx, starty );
	out << setw( 20 ) << pw.word << "starting at (" 
		<< startx << ", " << starty << ")\n";
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Print all of the words in a vector to some ostream.
 *
 *  @param[in] found_words the vector of words to print
 *  @param[in,out] out the ostream to print to
 *
 *
 *  @par Example
 *  @verbatim

    print_words( board.found_words );
	std::cout << "\nThere are a total of " 
		<< board.found_words.size() << " possible words left\n\n";

    @endverbatim
************************************************************************/
void print_words( vector<PuzzleWord> & found_words, ostream & out ) {
	for( auto it = found_words.rbegin(); it != found_words.rend(); it++ ) 
		print_puzzle_word( *it, out );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Print the solutions to a board to some ostream
 *
 *  @param[in,out] out the ostream to print to
 *
 *  @par Example
 *  @verbatim

    board.find_solution_from_words( );
	board.print_solutions( );

    @endverbatim
************************************************************************/
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


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Prompts a user for a bool array representing the letters used by a hint
 *  , finds words that match the hint, and prints those words to the screen.
 *  board.find_all_words should have been called beforehand.
 *
 *  @param[in,out] board the strands board
 *
 *
 *  @par Example
 *  @verbatim

	vector<string> board_string = {
		"coke",
		"soap",
		"bomb"
	};

	StrandsBoard board( board_string );

    decramble_hint( board );

    @endverbatim
************************************************************************/
void decramble_hint( StrandsBoard & board ) {

	vector<PuzzleWord> solutions;
	
	bool ** coors = alloc_2d_arr<bool>( 
		PuzzleWord::width, 
		PuzzleWord::height, 
		0 
	);

	// get the string representation of the bool array
	vector<string> hint_string;
	do {
		std::cout << "insert bool array for hint\n\n";
	} while( !get_board ( 
		hint_string, 
		PuzzleWord::width, 
		PuzzleWord::height
	) );

	// convert the 2d string into the 2d bool array
	for( int y = 0; y < PuzzleWord::height; y++ ) {
		for( int x = 0; x < PuzzleWord::width; x++ ) {
			coors[y][x] = static_cast<bool>( hint_string[y][x] - '0' );
		}
	}

	// find the hint matches and print them
	board.find_hint_matches( coors, solutions );

	std::cout << "\nCandidates for the hint are: \n";
	print_words( solutions );
	std::cout << "\n";

}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Deletes a word from a StrandsBoard instance's found_word vector. If
 *  multiple occurences of that word are found inside the found_word vector
 *  the user is prompted on which one to delete. After the word is deleted,
 *  the found_words is updated to only contain words which don't overlap
 *  with the deleted word.
 *
 *  @param[in] word the word to delete
 *
 *  @par Example
 *  @verbatim

    std::cout << "Type a word to mark as used: ";
	cin >> word_to_delete;
	std::cout << "\n";

	board.delete_word( word_to_delete );

    @endverbatim
************************************************************************/
void StrandsBoard::delete_word( string word ) {
	vector<int> candidate_indicies;

	// find words that match
	for( size_t i = 0; i < found_words.size(); i++ )
		if( found_words[i].word == word ) 
			candidate_indicies.push_back( static_cast<int> ( i ) );

	// return if there is nothing to delete
	if( candidate_indicies.empty() ) {
		std::cout << "There are no words on the board matching that\n";
		return;
	}

	int choice;

	// prompt the user which specific instance of the word to delete if necessary
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

	// set the letters used in the word as used on the board
	for( int y = 0; y < height; y++ ) 
		for( int x = 0; x < width; x++ ) 
			if( found_words[ candidate_indicies[choice] ].coordinates[y][x] ) used[y][x] = true;


	// delete overlapping words
	PuzzleWord pw = found_words[ candidate_indicies[choice] ];
	for( auto it = found_words.begin(); it != found_words.end(); ) {
		if( it->overlap( pw ) ) {
			it = found_words.erase( it );
		} else it++;
	}

	std::cout << "The word has been deleted\n\n";

}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Helps solve an NYT strands solver by allowing a user to find words
 *  on the board, try to find solutions, mark words as used on the board,
 *  and descramble a hint.
************************************************************************/
void menu() {
	
	int width = 6;
	int height = 8;

	// get the board width, height, and text
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

	// find all of the words in the board
	board.find_all_words();

	int option = -1;

	while( option != 5 ) {

		// get option from user
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
