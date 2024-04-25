
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_webidl_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_webidl_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_webidl_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = alignMemory(len, 8); // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_webidl_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy(array, view, offset) {
    offset /= view.BYTES_PER_ELEMENT;
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}

// Interface: VoidPtr

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};

// Interface: PuzzleWord

/** @suppress {undefinedVars, duplicate} @this{Object} */
function PuzzleWord() { throw "cannot construct a PuzzleWord, no constructor in IDL" }
PuzzleWord.prototype = Object.create(WrapperObject.prototype);
PuzzleWord.prototype.constructor = PuzzleWord;
PuzzleWord.prototype.__class__ = PuzzleWord;
PuzzleWord.__cache__ = {};
Module['PuzzleWord'] = PuzzleWord;
/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['get_word'] = PuzzleWord.prototype.get_word = function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_PuzzleWord_get_word_0(self));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['get_coordinate'] = PuzzleWord.prototype.get_coordinate = function(x, y) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  return _emscripten_bind_PuzzleWord_get_coordinate_2(self, x, y);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['get_width'] = PuzzleWord.prototype.get_width = function() {
  var self = this.ptr;
  return _emscripten_bind_PuzzleWord_get_width_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['set_width'] = PuzzleWord.prototype.set_width = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_PuzzleWord_set_width_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(PuzzleWord.prototype, 'width', { get: PuzzleWord.prototype.get_width, set: PuzzleWord.prototype.set_width });
/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['get_height'] = PuzzleWord.prototype.get_height = function() {
  var self = this.ptr;
  return _emscripten_bind_PuzzleWord_get_height_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['set_height'] = PuzzleWord.prototype.set_height = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_PuzzleWord_set_height_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(PuzzleWord.prototype, 'height', { get: PuzzleWord.prototype.get_height, set: PuzzleWord.prototype.set_height });

/** @suppress {undefinedVars, duplicate} @this{Object} */
PuzzleWord.prototype['__destroy__'] = PuzzleWord.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_PuzzleWord___destroy___0(self);
};

// Interface: StrandsBoard

/** @suppress {undefinedVars, duplicate} @this{Object} */
function StrandsBoard(width, height) {
  if (width && typeof width === 'object') width = width.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  if (width === undefined) { this.ptr = _emscripten_bind_StrandsBoard_StrandsBoard_0(width); getCache(StrandsBoard)[this.ptr] = this;return }
  if (height === undefined) { this.ptr = _emscripten_bind_StrandsBoard_StrandsBoard_1(width, height); getCache(StrandsBoard)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_StrandsBoard_StrandsBoard_2(width, height);
  getCache(StrandsBoard)[this.ptr] = this;
};

StrandsBoard.prototype = Object.create(WrapperObject.prototype);
StrandsBoard.prototype.constructor = StrandsBoard;
StrandsBoard.prototype.__class__ = StrandsBoard;
StrandsBoard.__cache__ = {};
Module['StrandsBoard'] = StrandsBoard;
/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['find_solution_from_words'] = StrandsBoard.prototype.find_solution_from_words = function() {
  var self = this.ptr;
  _emscripten_bind_StrandsBoard_find_solution_from_words_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['find_all_words'] = StrandsBoard.prototype.find_all_words = function() {
  var self = this.ptr;
  _emscripten_bind_StrandsBoard_find_all_words_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['set_board'] = StrandsBoard.prototype.set_board = function(new_board) {
  var self = this.ptr;
  ensureCache.prepare();
  if (new_board && typeof new_board === 'object') new_board = new_board.ptr;
  else new_board = ensureString(new_board);
  _emscripten_bind_StrandsBoard_set_board_1(self, new_board);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['set_used'] = StrandsBoard.prototype.set_used = function(x, y, value) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (value && typeof value === 'object') value = value.ptr;
  _emscripten_bind_StrandsBoard_set_used_3(self, x, y, value);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['set_hint_coor'] = StrandsBoard.prototype.set_hint_coor = function(x, y, value) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (value && typeof value === 'object') value = value.ptr;
  _emscripten_bind_StrandsBoard_set_hint_coor_3(self, x, y, value);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_found_word'] = StrandsBoard.prototype.get_found_word = function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_StrandsBoard_get_found_word_1(self, index), PuzzleWord);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_found_words_amount'] = StrandsBoard.prototype.get_found_words_amount = function() {
  var self = this.ptr;
  return _emscripten_bind_StrandsBoard_get_found_words_amount_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_solution_amount'] = StrandsBoard.prototype.get_solution_amount = function() {
  var self = this.ptr;
  return _emscripten_bind_StrandsBoard_get_solution_amount_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_solution'] = StrandsBoard.prototype.get_solution = function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return UTF8ToString(_emscripten_bind_StrandsBoard_get_solution_1(self, index));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_hints'] = StrandsBoard.prototype.get_hints = function() {
  var self = this.ptr;
  _emscripten_bind_StrandsBoard_get_hints_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_hint'] = StrandsBoard.prototype.get_hint = function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_StrandsBoard_get_hint_1(self, index), PuzzleWord);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['get_hints_amount'] = StrandsBoard.prototype.get_hints_amount = function() {
  var self = this.ptr;
  return _emscripten_bind_StrandsBoard_get_hints_amount_0(self);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
StrandsBoard.prototype['__destroy__'] = StrandsBoard.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_StrandsBoard___destroy___0(self);
};
