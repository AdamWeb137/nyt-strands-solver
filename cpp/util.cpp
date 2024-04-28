/** **********************************************************************
* @file
* @brief contains implementation of the LetterNode struct and get_words.
*************************************************************************/
#include "strands.h"



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Converts an ascii character into an index that the LetterNode struct
 *  can use
 *
 *  @param[in] c the char to convert to an index
 *
 *  @returns an index between 0 and LETTER_NODE_MAX-1 inclusive. If 
 *  the character is not between a-z, the index is LETTER_NODE_MAX - 1.
 *
 *  @par Example
 *  @verbatim

    children[ char_to_index( c ) ];

    @endverbatim
************************************************************************/
int LetterNode::char_to_index( char c ) {
	if( c > 'z' || c < 'a' ) return LETTER_NODE_MAX - 1;
	return static_cast<int> ( c - 'a' );
}



/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Returns the child node corresponding to the character c.
 *
 *  @param[in] c the character to index.
 *  @param[in] green the green part of the color
 *  @param[in] blue the blue part of the color
 *
 *  @returns the grayscale value as a pixel
 *
 *  @par Example
 *  @verbatim

    LetterNode * ln = this;
	for( auto c : word ) {
		if( (*ln)[c] == nullptr ) return false;
		ln = (*ln)[c];
	}
	return true;

    @endverbatim
************************************************************************/
LetterNode * & LetterNode::operator []( char c ) {
	return children[ char_to_index( c ) ];
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Constructor for the LetterNode struct. Sets all of the children to nullptr.
 *
 *  @par Example
 *  @verbatim

    LetterNode prefix_tree;

    @endverbatim
************************************************************************/
LetterNode::LetterNode() {
	memset( children, 0, LETTER_NODE_MAX * sizeof( LetterNode * ) );
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Deconstructs the LetterNode and all of its children.
************************************************************************/
LetterNode::~LetterNode( ) {
	for( auto & ln : children ) 
		if ( ln != nullptr ) delete ln;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Checks if a prefix is in the LetterNode prefix tree.
 *
 *  @param[in] word the prefix to check whether it is contained.
 *
 *  @returns true if the prefix is in the tree, false otherwise.
 *
 *  @par Example
 *  @verbatim

    // if we are out of bounds or if the word is not a prefix of any known word,
	// or if the position is already in use in our working word,
	// we return from the function
	if ( !in_bounds( x, y ) || used[y][x] 
	|| !prefix_tree.contains( word_str ) ) return;

    @endverbatim
************************************************************************/
bool LetterNode::contains( string word ) {
	LetterNode * ln = this;
	for( auto c : word ) {
		if( (*ln)[c] == nullptr ) return false;
		ln = (*ln)[c];
	}
	return true;
}


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Add a word to the prefix tree.
 *
 *  @param[in] word the word to add to the prefix tree.
 *
 *  @par Example
 *  @verbatim

	LetterNode ln;

    while ( wfile >> temp ) {

		// get rid of all punctuation
		temp.erase( remove_if( temp.begin(), temp.end(), ::ispunct ), temp.end() );
		// lowercase the word
		for( auto & c : temp )
			c = static_cast<char>( tolower( c ) );

		// dont bother with a short word
		if ( temp.size() < 3 ) continue;

		ln.insert( temp );
	}

    @endverbatim
************************************************************************/
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


/** **********************************************************************
 *  @author Adam Wood
 *
 *  @par Description
 *  Adds all words in a words file to a set of strings and to a LetterNode
 *  prefix tree.
 *
 *  @param[out] words the set of strings to add words to.
 *  @param[out] ln the LetterNode prefix tree to add words to.
 *
 *  @returns the grayscale value as a pixel
 *
 *  @par Example
 *  @verbatim

	LetterNode prefix_tree;
	set<string> dictionary;

    get_words( dictionary, prefix_tree );

    @endverbatim
************************************************************************/
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
		//words.insert( plural );
		words.insert( temp );
		//ln.insert( plural );
		ln.insert( temp );
	}

	wfile.close();

}