
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: PuzzleWord


char* EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_get_word_0(PuzzleWord* self) {
  return self->get_word();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_get_coordinate_2(PuzzleWord* self, int x, int y) {
  return self->get_coordinate(x, y);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_get_width_0(PuzzleWord* self) {
  return self->width;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_set_width_1(PuzzleWord* self, int arg0) {
  self->width = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_get_height_0(PuzzleWord* self) {
  return self->height;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord_set_height_1(PuzzleWord* self, int arg0) {
  self->height = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_PuzzleWord___destroy___0(PuzzleWord* self) {
  delete self;
}

// Interface: StrandsBoard


StrandsBoard* EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_StrandsBoard_0() {
  return new StrandsBoard();
}

StrandsBoard* EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_StrandsBoard_2(int width, int height) {
  return new StrandsBoard(width, height);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_find_solution_from_words_0(StrandsBoard* self) {
  self->find_solution_from_words();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_find_all_words_0(StrandsBoard* self) {
  self->find_all_words();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_set_board_1(StrandsBoard* self, char* new_board) {
  self->set_board(new_board);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_set_used_3(StrandsBoard* self, int x, int y, int value) {
  self->set_used(x, y, value);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_set_hint_coor_3(StrandsBoard* self, int x, int y, bool value) {
  self->set_hint_coor(x, y, value);
}

PuzzleWord* EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_found_word_1(StrandsBoard* self, int index) {
  return self->get_found_word(index);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_found_words_amount_0(StrandsBoard* self) {
  return self->get_found_words_amount();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_solution_amount_0(StrandsBoard* self) {
  return self->get_solution_amount();
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_solution_1(StrandsBoard* self, int index) {
  return self->get_solution(index);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_hints_0(StrandsBoard* self) {
  self->get_hints();
}

PuzzleWord* EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_hint_1(StrandsBoard* self, int index) {
  return self->get_hint(index);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard_get_hints_amount_0(StrandsBoard* self) {
  return self->get_hints_amount();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StrandsBoard___destroy___0(StrandsBoard* self) {
  delete self;
}

}

