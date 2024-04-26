class Board {

    static board_string = "leaespapcmaeikorrrretqrotgntuseieeirvremioskatrr";
    static width = 6;
    static height = 8;
    static board_changed = true;
    static found_solution = false;
    static found_hint = false;

    // for editting board
    static board_nums = [];

    // for showing a single word or a bunch of solutions
    static solution_nums = [];

    static num_classes = ["board-regular","board-used","board-hint"];
    static word_classes = ["board-regular","board-used", "color-2", "color-3","color-4","color-5","color-6","color-7","color-8","color-9"];
    
    // modes: 0 = find words, 1 = solutions, 2 = edit, 3 = hint
    static mode = 2;

    static cpp_board;

    static selected_word;
    static selected_puzzle_word = null;

    static selected_solution;

    static clear_solutions_board() {
        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {
                Board.solution_nums[y][x] = 0;
            }
        }
    }

    static set_used() {
        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {
                Board.cpp_board.set_used( x, y, Board.board_nums[y][x] == 1 ? 1 : 0 );
                Board.cpp_board.set_hint_coor( x, y, Board.board_nums[y][x] == 2 ? 1 : 0 );
            }
        }
    }

    static initilize() {
        for( let i = 0; i < Board.height; i++ ) {
            Board.board_nums.push( new Int8Array( Board.width ) );
            Board.solution_nums.push( new Int8Array( Board.width )  );
        }
    }

    static size() {
        return Board.width * Board.height;
    }

    static render_board() {

        const board = document.querySelector("#board");

        let letter_positions;
        if( Board.selected_puzzle_word != null )
            letter_positions = Array( Board.selected_puzzle_word.get_word().length );

        board.innerHTML = "";

        let class_list = Board.mode == 2 ? Board.num_classes : Board.word_classes; 
        let nums = Board.mode == 2 ? Board.board_nums : Board.solution_nums;

        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {
                let board_char = Board.board_string[ x + Board.width * y ];
                let char_div = document.createElement("DIV");
                char_div.innerHTML = `<p>${board_char.toUpperCase()}</p>`;

                if( Board.selected_puzzle_word != null && Board.selected_puzzle_word.get_coordinate( x, y ) != 0 ) {
                    letter_positions[ Board.selected_puzzle_word.get_coordinate( x, y ) - 1 ] = [x,y];
                }

                char_div.classList.add( class_list[ nums[y][x] ] );
                
                if( Board.mode == 2 ) {
                    
                    char_div.classList.add("hover");
                
                    char_div.addEventListener("click",e=>{

                        char_div.classList.remove("board-used");
                        char_div.classList.remove("board-hint");
                        char_div.classList.remove( "board-regular" );


                        Board.board_nums[y][x] = (Board.board_nums[y][x] + 1) % 3;

                        if( Board.board_nums[y][x] == 1 ) {
                            char_div.classList.add("board-used");
                        }

                        else if( Board.board_nums[y][x] == 2 ) {
                            char_div.classList.add("board-hint");
                        }

                        else {
                            char_div.classList.add( "board-regular" );
                        }

                        Board.board_changed = true;


                    });


                }
                
                board.appendChild(char_div);
            }
        }

        // we are done if we are not rendering lines between letters
        if( Board.selected_puzzle_word == null ) return;

        let board_width = board.offsetWidth;
        let board_height = board.offsetHeight;

        let letter_width = board_width / Board.width;
        let letter_height = board_height / Board.height;
        let svg_el = document.createElementNS("http://www.w3.org/2000/svg", "svg");
        
        svg_el.setAttribute('xmlns', 'http://www.w3.org/2000/svg');

        svg_el.setAttribute("width", String( board_width ));
        svg_el.setAttribute("height", String( board_height ));


        for( let i = 0; i < letter_positions.length - 1; i++ ) {
            let line_el = document.createElementNS('http://www.w3.org/2000/svg','line');

            let xpos1 = letter_positions[i][0];
            let ypos1 = letter_positions[i][1];

            line_el.setAttribute("x1", String( Math.round( xpos1 * letter_width + letter_width / 2.0 ) ));
            line_el.setAttribute("y1", String( Math.round( ypos1 * letter_height + letter_height / 2.0 ) ));


            let xpos2 = letter_positions[i+1][0];
            let ypos2 = letter_positions[i+1][1];

            line_el.setAttribute("x2", String( Math.round( xpos2 * letter_width + letter_width / 2.0 ) ));
            line_el.setAttribute("y2", String( Math.round( ypos2 * letter_height + letter_height / 2.0 ) ));

            line_el.setAttribute("stroke","var(--strands-blue)");

            svg_el.appendChild( line_el );
            
        }
        
        board.appendChild( svg_el );

    }

    static attempt_edit_board( event ) {

        // if the user tries to push the set board button when its invisible, lets just not
        if( Board.mode != 2 ) return;

        const edit_textarea = document.querySelector(" #edit-menu > textarea ");
        let candidate_text = edit_textarea.value.replaceAll(" ","").replaceAll("\n","");
        candidate_text = candidate_text.toLowerCase();

        for( let char of candidate_text ) {
            if( !char.match(/[a-z]/i) ) {
                // alert("Board has invalid characters");
                return;
            }
        }

        if( candidate_text.length != Board.size() ) {
            // alert("Board does not have enough characters");
            return;
        }

        // dont change anything if not neccesary
        if( candidate_text == Board.board_string ) return;

        Board.board_string = candidate_text;
        Board.board_changed = true;

        Board.cpp_board.set_board( Board.board_string );

        Board.render_board();

    }


    static change_mode( new_mode ) {
        
        if( new_mode == Board.mode ) return;

        Board.selected_puzzle_word = null;

        if( Board.board_changed ) {
            
            Board.show_warning("Recursively finding words. This may take a second.");

            Board.set_used();
            Board.cpp_board.find_all_words();

            Board.found_solution = false;
            Board.found_hint = false;

            Board.hide_warning();
        
        }
        

        const uis = [ 
            document.querySelector("#words-menu"), 
            document.querySelector("#solutions-menu"), 
            document.querySelector("#edit-menu"), 
            document.querySelector("#hint-menu") 
        ];


        const btns = [
            document.querySelector("#found_words_btn"),
            document.querySelector("#found_solutions_btn"),
            document.querySelector("#edit_board_btn"),
            document.querySelector("#descramble_hint_btn")
        ];

        const display_type = [
            "block",
            "block",
            "block",
            "block"
        ];

        const menu_text = [
            "Possible Words Remaining",
            "Possible Solutions",
            "Edit Board",
            "Possible Hint Meanings"
        ];

        const edit_menu = document.querySelector("#edit-menu");

        for( let ui of uis ) {
            ui.style.display = "none";
        }

        uis[new_mode].style.display = display_type[new_mode];

        for( let btn of btns ) {
            btn.classList.remove("selected_option");
        }

        btns[new_mode].classList.add("selected_option");

        document.querySelector("#menu-title").innerHTML = menu_text[new_mode];

        Board.mode = new_mode;

        Board[`mode_${new_mode}`]();

        Board.board_changed = false;

    }


    static show_word( puzzle_word ) {

        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {

                Board.solution_nums[y][x] = (puzzle_word == null || puzzle_word.get_coordinate( x, y ) == 0) ? 0 : 1;

            }
        }

        Board.render_board();

    }

    static show_warning( message ) {
        const warning = document.querySelector("#cover");
        warning.style.display = "flex";
        warning.querySelector("p").innerText = message;
    }

    static hide_warning() {
        document.querySelector("#cover").style.display = "none";
    }

    static mode_0() {

        const words_div = document.querySelector("#words-list");

        words_div.innerHTML = "";

        let word_num = Board.cpp_board.get_found_words_amount();
        for( let i = 0; i < word_num; i++ ) {


            Board.create_word_element( words_div, i == 0, Board.cpp_board.get_found_word( i ), 0 );

            // let word_div = document.createElement("P");

            // let puzzle_word = Board.cpp_board.get_found_word( i );

            // word_div.innerHTML = puzzle_word.get_word();

            // if( i == 0 ) {
            //     Board.selected_word = word_div;
            //     word_div.classList.add("selected-word");
            //     Board.selected_puzzle_word = puzzle_word;
            //     Board.show_word( puzzle_word );
            // }

            // word_div.addEventListener("click",e=>{

            //     if( Board.mode != 0 || e.target == Board.selected_word ) return;

            //     Board.selected_word.classList.remove("selected-word");

            //     Board.selected_word = e.target;

            //     Board.selected_puzzle_word = puzzle_word;

            //     Board.selected_word.classList.add("selected-word");

            //     Board.show_word( puzzle_word );


            // });

            // words_div.appendChild(word_div);

        }

    }


    static show_solution( indicies ) {

        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {
                Board.solution_nums[y][x] = 0;
            }
        }

        let word_num = 1;
        
        for( let i = 0; i < indicies.length; i++ ) {
            let idx = indicies[i];
            let puzzle_word = Board.cpp_board.get_found_word( idx );

            for( let y = 0; y < Board.height; y++ ) {
                for( let x = 0; x < Board.width; x++ ) {

                    if( puzzle_word.get_coordinate( x, y ) == 0 ) continue;
                    Board.solution_nums[y][x] = word_num;

                }
            }

            word_num++;
            if( word_num >= 10 )
                word_num = 1;

        }

        Board.render_board();

    }

    static mode_1() {

        if( !Board.found_solution )
            Board.show_warning("Recursively finding solutions. This will take a minute.");

        setTimeout( ()=>{

        if( !Board.found_solution )
            Board.cpp_board.find_solution_from_words();

        Board.found_solution = true;

        Board.hide_warning();

        Board.clear_solutions_board();

        const solutions_div = document.querySelector("#solutions-list");

        solutions_div.innerHTML = "";

        let solutions_num = Board.cpp_board.get_solution_amount();
        if( solutions_num == 0 ) {
            solutions_div.innerHTML = "<p class=\"no-solution\">No solutions found.</p>";
            Board.show_word( null );
        }


        for( let i = 0; i < solutions_num; i++ ) {

            let sol_div = document.createElement("DIV");

            let solution_indicies = JSON.parse( Board.cpp_board.get_solution(i) );

            for( let j = 0; j < solution_indicies.length; j++ ) {


                Board.create_word_element( sol_div, i == 0 && j == 0, Board.cpp_board.get_found_word( solution_indicies[j] ), 1 );

                // let index =  solution_indicies[j];
                // let p_child = document.createElement("P");
                // let puzzle_word = Board.cpp_board.get_found_word( index );
                // p_child.innerText = puzzle_word.get_word();

                // sol_div.appendChild( p_child );

                // if( i == 0 && j == 0 ) {
                //     Board.selected_word = p_child;
                //     p_child.classList.add("selected-word");
                //     Board.selected_puzzle_word = puzzle_word;
                //     Board.show_word( puzzle_word );
                // }

                // p_child.addEventListener("click",e=>{

                //     if( Board.mode != 1 || p_child == Board.selected_word ) return;
    
                //     Board.selected_word.classList.remove("selected-word");
    
                //     Board.selected_word = p_child;

                //     Board.selected_puzzle_word = puzzle_word;
    
                //     Board.selected_word.classList.add("selected-word");
    
                //     Board.show_word( puzzle_word );
    
                // });

            }

            // if( i == 0 ) {
            //     Board.selected_solution = sol_div;
            //     sol_div.classList.add("selected-solution");
            //     Board.show_solution( solution_indicies );
            // }

            // sol_div.addEventListener("click",e=>{

            //     if( Board.mode != 1 || sol_div == Board.selected_solution ) return;

            //     Board.selected_solution.classList.remove("selected-solution");

            //     Board.selected_solution = sol_div;

            //     Board.selected_solution.classList.add("selected-solution");

            //     Board.show_solution( solution_indicies );


            // });

            solutions_div.appendChild(sol_div);

        }

        }, 1000 );


    }


    static mode_2() {

        Board.render_board();

    }


    static create_word_element( parent_element, set_immediately, puzzle_word, current_mode ) {

        let word_div = document.createElement("P");

        word_div.innerHTML = puzzle_word.get_word();

        if( set_immediately ) {
            Board.selected_word = word_div;
            word_div.classList.add("selected-word");
            Board.selected_puzzle_word = puzzle_word;
            Board.show_word( puzzle_word );
        }

        word_div.addEventListener("click",e=>{

            if( Board.mode != current_mode || word_div == Board.selected_word ) return;

            Board.selected_word.classList.remove("selected-word");

            Board.selected_word = word_div;

            Board.selected_puzzle_word = puzzle_word;

            Board.selected_word.classList.add("selected-word");

            Board.show_word( puzzle_word );

        });

        parent_element.appendChild(word_div);

    }

    static mode_3() {

        const hint_div = document.querySelector("#hints-list");

        hint_div.innerHTML = "";

        Board.clear_solutions_board();

        if( !Board.found_hint )
            Board.cpp_board.get_hints();

        Board.found_hint = true;

        let hint_num = Board.cpp_board.get_hints_amount();
        if( hint_num == 0 ) {
            hint_div.innerHTML = "<p class=\"no-solution\"> No solutions to the hint were found. </p>";
        }

        for( let i = 0; i < hint_num; i++ ) {

            Board.create_word_element( hint_div, i == 0, Board.cpp_board.get_hint( i ), 3 );

        }

        Board.render_board();

    }

    static mark_selected_word_as_used() {

        // we can only mark a word when we are not in editing mode
        if( Board.mode == 2 || Board.selected_puzzle_word == null ) return;


        // mark the locations in the selected word as used on the board
        for( let y = 0; y < Board.height; y++ ) {
            for( let x = 0; x < Board.width; x++ ) {
                if( !Board.selected_puzzle_word.get_coordinate( x, y ) ) continue;
                Board.board_nums[y][x] = 1;
            }
        }

        // set selected_puzzle_word to null as the pointer
        // will be invalided by the webassembly
        Board.selected_puzzle_word = null;

        document.querySelector("#list-filter").value = "";

        // refresh the word list
        Board.board_changed = true;

        // ensure we force a solution to be refound
        // if we are in the solutions mode
        Board.found_solution = false;

        // since there is only one hint
        // when we mark a hint as used
        // just go back to the edit menu
        if( Board.mode == 3 ) {
            Board.change_mode( 2 );
            return;
        }

        // find all of the words remaining
        Board.set_used();

        // only find remaining words in the word list mode
        // in solutions mode we don't want to invalidate pointers
        Board.cpp_board.find_all_words();

        // refresh the word list, solutions list, etc
        // by calling the function that sets it up
        Board[`mode_${Board.mode}`]();

    }

    static filter_words_list( ) {
        const words_list = document.querySelector("#words-list");
        // get the fiter lowercased
        let filter = document.querySelector("#list-filter").value;
        filter = filter.toLowerCase();

        for( let child of words_list.children ) {
            // show a child if the child's word contains the filter as a substring
            // or if there is no filter
            if ( filter.length == 0 || child.innerHTML.includes( filter ) ) {
                child.style.display = "block";
                continue;
            } 
            child.style.display = "none";
        }

    }

}

Board.initilize();