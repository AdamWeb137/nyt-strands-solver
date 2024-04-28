## NYT Strands Solver

NYT Strands Solver is a tool to help find words and solutions in the NYT Strands puzzle
and descramble hints. The program has two forms: a command-line interface and a webpage.
The code to solve the puzzle is written in C++ and the webpage uses WebAssembly to run
this code and JavaScript to handle rendering and user input.

To try the webpage application go to [adamweb137.github.io/nyt-strands-solver/](https://adamweb137.github.io/nyt-strands-solver/).

### Compilation

Compiling the will require using c++20.
First, move inside the cpp subdirectory. 

#### Command line interface

At the terminal run `g++ --std=c++20 solve2.cpp puzzleword.cpp strandsboard.cpp strands.cpp util.cpp memory.cpp -o strands.o`.

#### Webpage

Emscripten must be installed. Find where webidl_binder is installed and run
`/path/to/webidl_binder idl_interface.idl ../glue`. Then run `em++ --std=c++20 puzzleword.cpp memory.cpp solve2.cpp strandsboard.cpp util.cpp glue_wrapper.cpp -sALLOW_MEMORY_GROWTH  --preload-file words --post-js ../glue.js -o ../strands.js`.

