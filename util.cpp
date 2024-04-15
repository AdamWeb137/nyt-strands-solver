#include "strands.h"

LetterNode::LetterNode() {
	memset( children, 0, 256 * sizeof( LetterNode * ) );
}

LetterNode::~LetterNode( ) {
	for( auto & ln : children ) 
		if ( ln != nullptr ) delete ln;
}

bool LetterNode::contains( string word ) {
	LetterNode * ln = this;
	for( auto c : word ) {
		if( ln->children[(int)c] == nullptr ) return false;
		ln = ln->children[(int)c];
	}
	return true;
}

void LetterNode::insert( string word ) {
	LetterNode * ln = this;
	for( auto c : word ) {
		if ( ln->children[(int)c] != nullptr ) {
			ln = ln->children[(int)c];
			continue;
		}
		LetterNode * nln = new (nothrow) LetterNode;
		ln->children[(int)c] = nln;
		ln = nln;
	}
}


void get_words( set<string> & words, LetterNode & ln ) {

	ifstream wfile( "words" );

	if( !wfile.is_open() ) exit ( 1 );

	string temp;
	string plural;

	while ( wfile >> temp ) {

		// get rid of all punctuation
		temp.erase( remove_if( temp.begin(), temp.end(), ::ispunct ), temp.end() );
		// lowercase the word
		for( auto & c : temp )
			c = static_cast<char>( tolower( c ) );

		// dont bother with a short word
		if ( temp.size() < 3 ) continue;

		// create a naively spelled plural of a word
		// even if that does not make sense
		plural = temp + "s";
		words.insert( plural );
		words.insert( temp );
		ln.insert( plural );
		ln.insert( temp );
	}

	wfile.close();

}
