#include "strands.h"

int LetterNode::char_to_index( char c ) {
	if( c > 'z' || c < 'a' ) return LETTER_NODE_MAX - 1;
	return static_cast<int> ( c - 'a' );
}

LetterNode * & LetterNode::operator []( char c ) {
	return children[ char_to_index( c ) ];
}

LetterNode::LetterNode() {
	memset( children, 0, LETTER_NODE_MAX * sizeof( LetterNode * ) );
}

LetterNode::~LetterNode( ) {
	for( auto & ln : children ) 
		if ( ln != nullptr ) delete ln;
}

bool LetterNode::contains( string word ) {
	LetterNode * ln = this;
	for( auto c : word ) {
		if( (*ln)[c] == nullptr ) return false;
		ln = (*ln)[c];
	}
	return true;
}

void LetterNode::insert( string word ) {
	LetterNode * ln = this;
	for( auto c : word ) {
		if ( (*ln)[c] != nullptr ) {
			ln = (*ln)[c];
			continue;
		}
		LetterNode * nln = new (nothrow) LetterNode;
		(*ln)[c] = nln;
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

void PuzzleWord::print_coors ( ostream & out ) {
	out << right;
	for( int y = 0; y < height; y++ ) {
		for( int x = 0; x < width; x++ ) {
			out << setw(3) << coordinates[y][x];
		}
		out << "\n";
	}
}
