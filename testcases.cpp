#include "strands.h"
#include "../catch_amalgamated.hpp"

using namespace Catch;

set<string> words;
LetterNode wordtree;

TEST_CASE("StrandsBoard") {
	vector<string> b = {
		"cats",
		"dogs",
		"moms"
	};

	StrandsBoard sb( b );

	REQUIRE( sb.width == 4 );
	REQUIRE( sb.height == 3 );

	for( int i = 0; i < sb.height; i++ )
		REQUIRE( !strcmp( sb.board[i], b[i].c_str() ) );

	words.insert("cats");
	words.insert("moms");
	words.insert("dogs");
	solve_strands( sb, words, wordtree ); 
}

TEST_CASE("todays") {
	get_words(words, wordtree);
	vector<string> b = {
		"rconna",
		"qoksav",
		"ueebir",
		"tnhaoi",
		"rejnpn",
		"aruods",
		"nsiorh",
		"eysaus"
	};

	StrandsBoard sb( b );
	sb.max_words = 10;

	solve_strands( sb, words, wordtree );
}

TEST_CASE("find_all_words") {
	get_words(words, wordtree);
	vector<PuzzleWord> found;
	vector<string> b = {
		"rconna",
		"qoksav",
		"ueebir",
		"tnhaoi",
		"rejnpn",
		"aruods",
		"nsiorh",
		"eysaus"
	};
	StrandsBoard sb(b);

	int nirvana_points[][2] = {
		{0,0},
		{1,0},
		{1,1},
		{2,1},
		{3,2},
		{3,3},
		{3,4},
		{4,5},
		{5,5}
	};

	for( auto np : nirvana_points )
		sb.used[ np[1] ][ np[0] ] = true;

	std::cout << "looking for words\n";
	find_all_words( found, sb, words, wordtree );
	std::cout << "finished looking for words\n";
	
	/*
	for ( auto & puzzle_word : found )
		std::cout << puzzle_word.word << "\n";
	*/

	find_solution_from_words( found, sb );

}

TEST_CASE("find_all_words - todays") {
	get_words(words, wordtree);
	vector<PuzzleWord> found;
	vector<string> b = {
		"euttel",
		"cnorke",
		"bckluy",
		"saceth",
		"maipct",
		"andwio",
		"ynnsma",
		"oaieto"
	};
	StrandsBoard sb(b);

	int nirvana_points[][2] = {
		{0,0},
		{1,0},
		{1,1},
		{2,1},
		{3,2},
		{3,3},
		{3,4},
		{4,5},
		{5,5}
	};

	/*
	for( auto np : nirvana_points )
		sb.used[ np[1] ][ np[0] ] = true;
	*/

	std::cout << "looking for words\n";
	find_all_words( found, sb, words, wordtree );
	std::cout << "finished looking for words\n";
	
	/*
	for ( auto & puzzle_word : found )
		std::cout << puzzle_word.word << "\n";
	*/

	find_solution_from_words( found, sb );

}
