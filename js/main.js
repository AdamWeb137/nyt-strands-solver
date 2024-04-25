
let INIT_LEVEL = 0;

Module["onRuntimeInitialized"] = () => {
    INIT_LEVEL++;
    if( INIT_LEVEL == 2 ) INIT_SOLVER();
}

window.addEventListener("load",()=>{
    INIT_LEVEL++;
    if( INIT_LEVEL == 2 ) INIT_SOLVER();
});

function INIT_SOLVER () {

    if( INIT_LEVEL != 2 ) return;

    Board.cpp_board = new Module.StrandsBoard();
    Board.cpp_board.set_board( Board.board_string );

    const edit_textarea = document.querySelector(" #edit-menu > textarea ");

    edit_textarea.value = 
    "leaesp\napcmae\nikorrr\nretqro\ntgntus\neieeir\nvremio\nskatrr";

    // document.querySelector("#edit-set-button").addEventListener("click", Board.attempt_edit_board );
    document.querySelector("#edit-text").addEventListener("input", Board.attempt_edit_board );

    document.querySelector("#found_words_btn").addEventListener("click", ()=>{
        Board.change_mode(0);
    });
    document.querySelector("#found_solutions_btn").addEventListener("click", ()=>{
        Board.change_mode(1);
    });
    document.querySelector("#edit_board_btn").addEventListener("click", ()=>{
        Board.change_mode(2);
    });
    document.querySelector("#descramble_hint_btn").addEventListener("click", ()=>{
        Board.change_mode(3); 
    });
    document.querySelector(".delete-word-btn > p").addEventListener("click", ()=>{
        Board.mark_selected_word_as_used();
    });

    Board.render_board();

}

window.addEventListener("beforeunload",()=>{
    Module.destroy( Board.cpp_board );
    Board.cpp_board = null;
})