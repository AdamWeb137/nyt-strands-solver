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

using namespace std;

template <typename T>
T ** alloc_2d_arr( int width, int height, T fill = 0 );

template <typename T>
void free_2d( T * * & ptr );

const int MIN_WORD_LEN = 4;

struct StrandsBoard {
	
	int width;
	int height;

	char ** board;

	char ** words;

	int words_len;

	int max_words = 8;

	int curr_word_len;

	bool ** used;

	void allocate();

	StrandsBoard( );

	StrandsBoard( int w, int h );

	StrandsBoard( vector<string> & board_string );

	~StrandsBoard( );

	int size();

	bool in_bounds( int x, int y );

	void print_solution( ostream & out );

	string curr_word();

	int linearize_coor( int x, int y ); 

};

struct LetterNode {
	LetterNode * children [ 256 ];
	LetterNode();
	~LetterNode();
	bool contains( string word );
	void insert( string word );
};

struct PuzzleWord {
	
	string word;
	int ** coordinates;

	inline static int width;
	inline static int height;

	PuzzleWord();
	PuzzleWord( PuzzleWord & other );

	void move( PuzzleWord & other );

	PuzzleWord& operator=(PuzzleWord& other);
	PuzzleWord& operator=(PuzzleWord&& other);

	PuzzleWord( string w, int ** coor_original );
	~PuzzleWord();
	
	friend void swap( PuzzleWord & a, PuzzleWord & b );

	bool overlap ( PuzzleWord & other );
	PuzzleWord( PuzzleWord && other );



};

bool operator < ( PuzzleWord & lhs, PuzzleWord & rhs );

/*
int PuzzleWord::width;
int PuzzleWord::height;
*/

void solve_strands( StrandsBoard & board, set<string> & words, LetterNode & wordtree );
void solve_strands_new_word ( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int chars_used = 0 );
void solve_strands_old_word( StrandsBoard & board, set<string> & words, LetterNode & wordtree, int x, int y, int chars_used );

void get_words( set<string> & words, LetterNode & ln );

void find_all_words_from_point( vector<PuzzleWord> & found_words, StrandsBoard & board, set<string> & dictionary, LetterNode & prefix_tree, char * word_str, vector<int> & coors, int x, int y, int word_len = 0 );

void find_all_words( vector<PuzzleWord> & found_words, StrandsBoard & board, set<string> & dictionary, LetterNode & prefix_tree );

void find_solution_from_words( vector<PuzzleWord> & found_words, StrandsBoard & board );

void find_solution_from_words_rec( vector<PuzzleWord> & found_words, StrandsBoard & board, vector<int> & indicies, int total_chars, int & depth, int max_depth = 0 );

#endif
