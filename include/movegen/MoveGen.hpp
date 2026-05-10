#pragma once

#include "core/Types.hpp"
#include "utils/BitManipulation.hpp"

/*---------- ENCODING AND DECODING MOVES ------------/

Every move will be stored in 32 bits:

0-5  (source square)    - 64 possibilities
6-11  (target square)    - 64 possibilities
12-15  (flags):
 +------+-----------+---------+-----------+-----------+-----------------------+
 | code | promotion | capture | special 1 | special 0 |     kind of move      |
 +------+-----------+---------+-----------+-----------+-----------------------+
 |  0   |     0     |    0    |     0     |     0     | quiet moves           |
 |  1   |     0     |    0    |     0     |     1     | double pawn push      |
 |  2   |     0     |    0    |     1     |     0     | king castle           |
 |  3   |     0     |    0    |     1     |     1     | queen castle          |
 |  4   |     0     |    1    |     0     |     0     | captures              |
 |  5   |     0     |    1    |     0     |     1     | ep-capture            |
 |  8   |     1     |    0    |     0     |     0     | knight-promotion      |
 |  9   |     1     |    0    |     0     |     1     | bishop-promotion      |
 |  10  |     1     |    0    |     1     |     0     | rook-promotion        |
 |  11  |     1     |    0    |     1     |     1     | queen-promotion       |
 |  12  |     1     |    1    |     0     |     0     | knight-promo capture  |
 |  13  |     1     |    1    |     0     |     1     | bishop-promo capture  |
 |  14  |     1     |    1    |     1     |     0     | rook-promo capture    |
 |  15  |     1     |    1    |     1     |     1     | queen-promo capture   |
 +------+-----------+---------+-----------+-----------+-----------------------+

16-31  (move score bits)
*/

//ssss ssss ssss ssss f1f2f3f4 t1t2t3t4 t5t6s1s2 s3s4s5s6  - move structure

inline U32 encode_move(int source_square, int target_square, int flags, int score = 0){
    return  (source_square & 0x3FU) |
            ((target_square & 0x3FU) << 6) |
            ((flags & 0xFU) << 12) |
            ((score & 0xFFFFU) << 16);
}

inline int get_move_source_square(U32 move){
    return move & 0x3FU;
}

inline int get_move_target_square(U32 move){
    return (move >> 6) & 0x3FU;
}

inline int get_move_flags(U32 move){
    return (move >> 12) & 0xFU;
}

inline int get_promotion_type(U32 move){
    return (move >> 12) & 0x3U;
}

inline int get_move_score(U32 move){
    return (move >> 16) & 0xFFFFU;
}

inline U32 set_move_score(U32 move, int score){
    //clear the old score bits first
    return (move & 0xFFFFU) | ( (U32)(score & 0xFFFFU) << 16 );   
}

inline bool is_move_capture(U32 move){
    return move & 0x4000U;
}

inline bool is_move_promotion(U32 move){
    return move & 0x8000U;
}

inline bool is_move_enpassant(U32 move){
    return (move & 0xF000U) == 0x5000U;
}

inline bool is_move_double_pawn_push(U32 move){
    return (move & 0xF000U) == 0x1000U;
}

inline bool is_move_king_castle(U32 move){
    return (move & 0xF000U) == 0x2000U;
}

inline bool is_move_queen_castle(U32 move){
    return (move & 0xF000U) == 0x3000U;
}

/*---------------------------------------------------*/

void generate_moves(int side, MoveList& move_list);

template <int SIDE>
void generate_all_moves(MoveList& move_list);

//adds to the move list pawn pushes (promotions included)
template <int SIDE>
void generate_pawn_pushes(MoveList& move_list);

template <int SIDE>
void generate_pawn_captures(MoveList& move_list);

template <int SIDE>
void generate_king_moves(MoveList& move_list);

template <int SIDE>
void generate_knight_moves(MoveList& move_list);

//generates the moves for bishop, rook or queen
template <int SIDE, int PIECE_TYPE>
void generate_slider_piece_moves(MoveList& move_list);