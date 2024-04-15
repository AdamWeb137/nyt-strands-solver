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

bool operator < ( PuzzleWord & lhs, PuzzleWord & rhs ) {
	return lhs.word.size() > rhs.word.size();
}
