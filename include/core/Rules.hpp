#pragma once

#include "Types.hpp"

//side to move
inline int side;

// the one and only square that can capture enpassant in current position
inline int enpassant = Squares :: no_square;

/*----------- castling rights ----------------

0001 - white king can castle to the king side
0010 - white king can castle to the queen side
0100 - black king can castle to the king side
1000 - black king can castle to the queen side

ex: 1010 means both kings can castle queen side only

--------------------------------------------*/
inline int castle;

enum{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8
};

// a field is the sum of rights remaining if the piece on the coresponding squared is moved/captured
// use castle & field to update catling rights
inline const int castle_rights_mask[64]={
    7 , 15, 15, 15, 3 , 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// counts how many moves until draw (if there are no captures or pawn moves)
inline int moves_remaining = 100;

// we use a stack to preserve game states for eventual move unmaking
struct Game_history{
    struct State{
        int castle_rights;
        int enpassant_square;
        int captured_piece;     // 0-12 according to Pieces namespace
        int fifty_move_counter;
    };

    State history_stack[512];       // 512 moves should be enough for any chess game
    int stack_ptr = 0;              // pointer to the next move state

    inline void push_state(int castle, int enpassant_square, int caputured_piece, int fifty){
        history_stack[stack_ptr].castle_rights = castle;
        history_stack[stack_ptr].enpassant_square = enpassant_square;
        history_stack[stack_ptr].captured_piece = caputured_piece;
        history_stack[stack_ptr].fifty_move_counter = fifty;
        stack_ptr ++;
    }

    inline State pop_state(){
        stack_ptr --;
        return history_stack[stack_ptr];
    }
}inline game_history;