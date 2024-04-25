#ifndef _STRANDS_H_
#define _STRANDS_H_

#define __WEBASM__

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

const int MIN_WORD_LEN = 4;

struct PuzzleWord;
struct LetterNode;

#define LETTER_NODE_MAX 27
struct LetterNode {
	LetterNode * children [ LETTER_NODE_MAX ];
	LetterNode();
	~LetterNode();
	bool contains( string word );
	void insert( string word );
	static int char_to_index( char c );
	LetterNode * & operator []( char c );
};

struct PuzzleWord {
	
	string word;
	int ** coordinates;

	inline static int width;
	inline static int height;

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

		bool ** used;

		bool ** hint_coors;

		LetterNode prefix_tree;

		set<string> dictionary;

		vector<PuzzleWord> found_words;

		vector<vector<int>> solutions;

		vector<PuzzleWord> possible_hints;

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
		int height;

		string json_holder;

		char ** words;

		int words_len;

		int max_words = 9;

		int curr_word_len;

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

// solve2.cpp approach
void find_all_words_from_point( StrandsBoard & board, set<string> & dictionary, char * word_str, int ** coors, set<string> & already_found, int x, int y, int word_len = 0 );

void find_all_words( StrandsBoard & board );

void find_solution_from_words( StrandsBoard & board );

void find_solution_from_words_rec( StrandsBoard & board, vector<int> & indicies, int total_chars, int & depth, int max_depth = 0 );

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
