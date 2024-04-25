#include "strands.h"

PuzzleWord::PuzzleWord() {
	coordinates = nullptr;
}


PuzzleWord::PuzzleWord( string w, int ** coor_original ) {

	word = w;

	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	if( coordinates == nullptr ) {
		std::cerr << "Failed to allocate memory\n";
		exit ( 1 );
		return;
	}

	memcpy( coordinates[0], coor_original[0], width * height * sizeof( int ) );

}

PuzzleWord::PuzzleWord( PuzzleWord & other ) {
	word = other.word;
	coordinates = alloc_2d_arr<int> ( width, height, 0 );
	memcpy( coordinates[0], other.coordinates[0], width * height * sizeof(int) );
}

void PuzzleWord::move( PuzzleWord & other ) {
	word.swap( other.word );
	coordinates = other.coordinates;
	other.coordinates = nullptr;
}

PuzzleWord::PuzzleWord( PuzzleWord && other ) {
	move( other );
}


void swap( PuzzleWord & a, PuzzleWord & b ) {
	a.word.swap( b.word );
	swap( a.coordinates, b.coordinates );
}

PuzzleWord& PuzzleWord::operator=(PuzzleWord& other) {
	PuzzleWord temp( other );
	swap( *this, temp );
	return *this;
}

PuzzleWord& PuzzleWord::operator=(PuzzleWord&& other) {
	move( other );
	return *this;
}


PuzzleWord::~PuzzleWord() {
	free_2d( coordinates );
}

bool PuzzleWord::overlap( PuzzleWord & other ) {
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int c1 = coordinates[y][x];
			int c2 = other.coordinates[y][x];
			if ( c1 != 0 && c2 != 0 ) return true;
		}
	}
	return false;
}

template <typename T>
bool PuzzleWord::total_overlap ( T ** coors ) {
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int c1 = coordinates[y][x] ? 1 : 0;
			int c2 = coors[y][x] ? 1 : 0;
			if ( c1 != c2 ) return false;
		}
	}
	return true;
}

template bool PuzzleWord::total_overlap<int>( int ** coors ); 
template bool PuzzleWord::total_overlap<bool>( bool ** coors ); 

bool PuzzleWord::comp( const PuzzleWord & other ) const {
	return word.size() > other.word.size();
}

bool operator < ( const PuzzleWord & lhs, const PuzzleWord & rhs ) {
	return lhs.comp( rhs );
}


char * PuzzleWord::get_word() {
	return (char * ) word.c_str();
}

int PuzzleWord::get_coordinate( int x, int y ) {
	return coordinates[y][x];
}