/** **********************************************************************
* @file
* @brief contains all function prototypes and structures
*************************************************************************/
#ifndef _STRANDS_H_
#define _STRANDS_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>
#include <new>

using namespace std;

template <typename T>
T ** alloc_2d_arr( int width, int height, T fill = 0 );

template <typename T>
void free_2d( T * * & ptr );

/** **********************************************************************
*  @brief
*  Minimum length of a possible solution word.
************************************************************************/
const int MIN_WORD_LEN = 4;

struct PuzzleWord;
struct LetterNode;

/** **********************************************************************
*  @brief
*  Number of nodes that a LetterNode can hold. One for each latin letter
*  and one extra.
************************************************************************/
#define LETTER_NODE_MAX 27

/** **********************************************************************
*  @brief
*  Node in a prefix tree.
************************************************************************/
struct LetterNode {

	LetterNode * children [ LETTER_NODE_MAX ];
    /**< Children nodes. One for 26 letters of the alphabet plus one extra */

	LetterNode();
	~LetterNode();
	bool contains( string word );
	void insert( string word );
	static int char_to_index( char c );
	LetterNode * & operator []( char c );
};


/** **********************************************************************
*  @brief
*  Represents a single word on the Strands board.
************************************************************************/
struct PuzzleWord {
	
	string word;
    /**< The word on the board. */
	int ** coordinates;
    /**< A 2d array of the board representing the position of 
	each letter in the word on the board with the first letter being marked 1
	the second letter being marked 2, etc, and every position that isn't in 
	the word marked 0. */


	inline static int width;
    /**< Width of the Strands board. */

	inline static int height;
    /**< Height of the strands board. */

	PuzzleWord();
	PuzzleWord( PuzzleWord & other );
	PuzzleWord( PuzzleWord && other );
	void move( PuzzleWord & other );
	PuzzleWord& operator=(PuzzleWord& other);
	PuzzleWord& operator=(PuzzleWord&& other);
	PuzzleWord( string w, int ** coor_original );
	~PuzzleWord();
	
	friend void swap( PuzzleWord & a, PuzzleWord & b );

	bool overlap ( PuzzleWord & other );

	template <typename T>
	bool total_overlap ( T ** coors );

	void print_coors ( ostream & out );

	bool comp( const PuzzleWord & other ) const;

	char * get_word();
	int get_coordinate( int x, int y );

};

bool operator < ( const PuzzleWord & lhs, const PuzzleWord & rhs );

/** **********************************************************************
*  @brief
*  Represents the Strands board. Contains the text, words found on the board,
*  possible solutions to the puzzle, and more. This is the class that does most
*  of the work to solve and help solve the puzzle.
************************************************************************/
class StrandsBoard {
	
	public:
	
		void allocate();

		StrandsBoard( );

		StrandsBoard( int w, int h );

		StrandsBoard( vector<string> & board_string );

		~StrandsBoard( );

		void set_board( char * new_board );

		int size();

		bool in_bounds( int x, int y );

		char ** board;
    	/**< Text of the strands board. */

		bool ** used;
    	/**< A 2d array tracking what letters have been used already. */

		bool ** hint_coors;
    	/**< A 2d array representing which letters are in a hint. */

		LetterNode prefix_tree;
    	/**< Prefix tree of all english words with a length of at least 4 and
		no non-letters. */

		set<string> dictionary;
    	/**< Dictionary of all english words with a length of at least 4 and 
		no non-letters. */


		vector<PuzzleWord> found_words;
    	/**< Vector of all words on the board that don't overlap with the 
		used letters. */


		vector<vector<int>> solutions;
    	/**< Vector of vectors of ints representing a list of indicies to words
		that are solutions to the puzzle. */

		vector<PuzzleWord> possible_hints;
    	/**< Vector of words that match a hint. */


		void print_solutions( ostream & out = std::cout );
		
		void find_hint_matches( bool ** hint_coors, vector<PuzzleWord> & matching );
		
		void find_solution_from_words(); 

		void delete_word( string word );

		string curr_word();

		int linearize_coor( int x, int y );

		void find_all_words(); 

		// interface code
		PuzzleWord * get_found_word( int index );

		int get_found_words_amount();

		int get_solution_amount();

		char * get_solution( int index );
    	
		void set_used( int x, int y, int value );

    	void set_hint_coor( int x, int y, bool value );

    	PuzzleWord * get_hint( int index );

		void get_hints();

		int get_hints_amount();

	private: 

		int width;
    	/**< Width of the Strands board. */

		int height;
    	/**< Height of the Strands board. */

		string json_holder;
    	/**< JSON representation of a solution for interacting with JS. */

		char ** words;
    	/**< List of words in a solution. Not used. */

		int words_len;
    	/**< Number of words in the words array. Not used. */

		int curr_word_len;
    	/**< Length of the last word in the working solution. Not used.  */

		int max_words = 9;
    	/**< Maximum number of words in a solution. */


		void find_all_words_from_point( char * word_str, int ** coors, set<string> & already_found, int x, int y, int word_len ); 

		void find_solution_from_words_rec( vector<int> & indicies, int total_chars, int & depth, int max_depth );


};


/*
int PuzzleWord::width;
int PuzzleWord::height;
*/

// solve.cpp approach (deprecated)
void solve_strands( StrandsBoard & board, set<string> & words, LetterNode & wordtree );
void solve_strands_new_word ( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int chars_used = 0 );
void solve_strands_old_word( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int x, int y, int chars_used );


void get_words( set<string> & words, LetterNode & ln );

// main helpers
int get_valid_int( const char * prompt_message, const char * error_message );

bool attempt_valid_int( int & result );

bool get_board( vector<string> & board_string, int width, int height );

void find_word_start( PuzzleWord & pw, int & startx, int & starty );

void print_puzzle_word( PuzzleWord & pw, ostream & out = std::cout );


void menu();

void decramble_hint( StrandsBoard & board );

void print_words( vector<PuzzleWord> & found_words, ostream & out = std::cout );

#endif
