#include "movegen/MoveGen.hpp"
#include "core/Types.hpp"
#include "core/Board.hpp"
#include "core/Logger.hpp"
#include "core/Rules.hpp"

void generate_moves(int side, MoveList& move_list){
    if (side == white)
        generate_all_moves<white>(move_list);
    else 
        generate_all_moves<black>(move_list);
}

template <int SIDE>
void generate_all_moves(MoveList& move_list){
    generate_pawn_captures<SIDE>(move_list);
    generate_pawn_pushes<SIDE>(move_list);
    generate_king_moves<SIDE>(move_list);
    generate_knight_moves<SIDE>(move_list);
    generate_slider_piece_moves<SIDE,Pieces::B>(move_list);
    generate_slider_piece_moves<SIDE,Pieces::R>(move_list);
    generate_slider_piece_moves<SIDE,Pieces::Q>(move_list);
}

template <int SIDE>
void generate_pawn_pushes(MoveList& move_list){
    int source_square, target_square;
    
    U64 occupancy = occupancies_bitboards[both];
    U64 single_push_targets, double_push_targets, promotion_targets;

    constexpr int one_square_move = (SIDE == white) ? 8 : -8;
    constexpr int two_squares_move = (SIDE == white) ? 16 : -16;
    constexpr U64 promoting_rank = (SIDE == white) ? RANK_7 : RANK_2;

    constexpr int piece = (SIDE == white) ? Pieces::P : Pieces::p;
    U64 pawns = pieces_bitboards[piece];

    // --------------------- PROMOTION ------------------------

    // only pawns placed on promotion rank can promote
    U64 promoting_pawns = pawns & promoting_rank;

    //select only the unoccupied squares for promoting
    if constexpr (SIDE == white)
        promotion_targets = (promoting_pawns >> 8) & ~occupancy;
    else 
        promotion_targets = (promoting_pawns << 8) & ~occupancy;

    while (promotion_targets) {
        //select and clear one promotion target square
        target_square = pop_lsb(promotion_targets);

        source_square = target_square + one_square_move;
        //Queen promotion 
        move_list.add_move(encode_move(source_square, target_square, 11));
        //Rook promotion 
        move_list.add_move(encode_move(source_square, target_square, 10));
        // Bishop promotion 
        move_list.add_move(encode_move(source_square, target_square, 9));
        // Knight promotion 
        move_list.add_move(encode_move(source_square, target_square, 8));
    }

    // ----------------- Quiet pawn pushes -------------------------
    // pawns that can't promote
    U64 quiet_pawns = pawns & ~promoting_rank;
    
    // Single Push
    if constexpr (SIDE == white)
        single_push_targets = (quiet_pawns >> 8) & ~occupancy;
    else
        single_push_targets = (quiet_pawns << 8) & ~occupancy;

    //copy the targets for double pushes
    U64 temp_single_targets = single_push_targets;

    // loop over the target squares for single pushes and add the coresponding move to the list
    while (temp_single_targets) {
        target_square = pop_lsb(temp_single_targets);
        move_list.add_move(encode_move(target_square + one_square_move, target_square, 0));
    }

    // for double pushes, we check if the next square after the single push targets on rank 3/7 is also empty
    if constexpr (SIDE == white)
        double_push_targets = ((single_push_targets & RANK_3) >> 8) & ~occupancy;
    else double_push_targets = ((single_push_targets & RANK_6) << 8) & ~occupancy;

    while (double_push_targets) {
        target_square = pop_lsb(double_push_targets);
        move_list.add_move(encode_move(target_square + two_squares_move, target_square, 1));
    }
}

template <int SIDE>
void generate_pawn_captures(MoveList& move_list){
    int source_square, target_square;

    constexpr int piece = (SIDE == white) ? Pieces::P : Pieces::p;
    U64 pawns = pieces_bitboards[piece];

    // promoting pawns are pawns on rank 7 for white and 2 for black
    constexpr U64 promoting_rank = (SIDE == white) ? RANK_7 : RANK_2;

    U64 promoting_pawns = pawns & promoting_rank;
    U64 normal_pawns = pawns & ~promoting_pawns;

    if (enpassant != Squares::no_square){
            // get the pawns "attacked" by the enpassant square
            U64 enpassant_attackers = pawn_attacks[SIDE^1][enpassant] & pawns;
            while(enpassant_attackers){
                source_square = pop_lsb(enpassant_attackers);
                U32 move = encode_move(source_square, enpassant, 5);
                move_list.add_move(move);
            }
        }

    U64 pawn_captures;
    U64 enemy_pieces = occupancies_bitboards[SIDE^1];

    // loop over the pawns that might promote
    while(promoting_pawns){
        source_square = pop_lsb(promoting_pawns);
        // calculate the occupied target squares
        pawn_captures = pawn_attacks[SIDE][source_square] & enemy_pieces;

        while (pawn_captures){
            target_square = pop_lsb(pawn_captures);
            move_list.add_move(encode_move(source_square, target_square, 15)); // queen promotion capture
            move_list.add_move(encode_move(source_square, target_square, 14)); // rook promotion capture
            move_list.add_move(encode_move(source_square, target_square, 13)); // bishop pomotion capture
            move_list.add_move(encode_move(source_square, target_square, 12)); // knight promotion capture
        }
    }

    // loop over the rest of the pawns
    while (normal_pawns){
        source_square = pop_lsb(normal_pawns);
        // calculate the occupied target squares
        pawn_captures = pawn_attacks[SIDE][source_square] & enemy_pieces;

        while (pawn_captures){
            target_square = pop_lsb(pawn_captures);
            U32 move = encode_move(source_square, target_square, 4);
            move_list.add_move(move);
        }
    }
}

template <int SIDE>
void generate_king_moves(MoveList& move_list){
    int target_square;

    U64 occupancies = occupancies_bitboards[both];
    constexpr int piece = (SIDE == white) ? Pieces::K : Pieces::k;
    int king_position = first_lsb(pieces_bitboards[piece]);
    U64 king_targets = king_attacks[king_position];

    constexpr int king_castle = (SIDE == white) ? wk : bk;
    constexpr int queen_castle = (SIDE == white) ? wq : bq;
    constexpr int opposite_side = (SIDE == white) ? black : white;

    constexpr int b_square = (SIDE == white) ? Squares::b1 : Squares::b8;
    constexpr int c_square = (SIDE == white) ? Squares::c1 : Squares::c8;
    constexpr int d_square = (SIDE == white) ? Squares::d1 : Squares::d8;
    constexpr int e_square = (SIDE == white) ? Squares::e1 : Squares::e8;
    constexpr int f_square = (SIDE == white) ? Squares::f1 : Squares::f8;
    constexpr int g_square = (SIDE == white) ? Squares::g1 : Squares::g8;

    // ---------------- CASTLING ---------------------

    // check if castle is possible on king side
    if (castle & king_castle){
        if( !getBit(occupancies, f_square) && 
            !getBit(occupancies, g_square) && 
            !is_square_attacked(e_square, opposite_side) &&
            !is_square_attacked(f_square, opposite_side) &&
            !is_square_attacked(g_square, opposite_side) )
            move_list.add_move(encode_move(e_square, g_square, 2));
    }

    // check if castle is possible on queen side
    if (castle & queen_castle){
        if( !getBit(occupancies, d_square) && 
            !getBit(occupancies, c_square) && 
            !getBit(occupancies, b_square) && 
            !is_square_attacked(e_square, opposite_side) &&
            !is_square_attacked(d_square, opposite_side) &&
            !is_square_attacked(c_square, opposite_side) )
            move_list.add_move(encode_move(e_square, c_square, 3));
    }

    // ----------------- CAPTURES --------------
    U64 king_captures = king_targets & occupancies_bitboards[opposite_side];
    while (king_captures){
        target_square = pop_lsb(king_captures);
        move_list.add_move(encode_move(king_position, target_square, 4));
    }
    
    // ----------------- QUIET MOVES -----------------------
    U64 king_quiet_moves = king_targets & ~occupancies_bitboards[both];
    while (king_quiet_moves){
        target_square = pop_lsb(king_quiet_moves);
        move_list.add_move(encode_move(king_position, target_square, 0));
    }
}

template <int SIDE>
void generate_knight_moves(MoveList& move_list){
    int source_square, target_square;
    constexpr int piece = (SIDE == white) ? Pieces::N : Pieces::n;
    U64 knights = pieces_bitboards[piece];
    U64 friendly_occupancies = occupancies_bitboards[SIDE];
    U64 oponent_occupancies = occupancies_bitboards[SIDE^1];

    while (knights){
        source_square = pop_lsb(knights);
        U64 attacks = knight_attacks[source_square] & ~friendly_occupancies;

        // --------------- CAPTURES -----------------
        U64 knight_captures = attacks & oponent_occupancies;
        while (knight_captures){
            target_square = pop_lsb(knight_captures);
            move_list.add_move(encode_move(source_square, target_square, 4));
        }

        // --------------- QUIET MOVES -----------------
        U64 knight_quiet_targets = attacks & ~oponent_occupancies;
        while(knight_quiet_targets){
            target_square = pop_lsb(knight_quiet_targets);
            move_list.add_move(encode_move(source_square, target_square, 0));
        }
    }
}

template <int SIDE, int PIECE_TYPE>
void generate_slider_piece_moves(MoveList& move_list){
    int source_square, target_square;
    U64 attacks, captures, quiet_targets;

    const U64 both_occ = occupancies_bitboards[both];
    const U64 friendly_occ = occupancies_bitboards[SIDE];
    const U64 enemy_occ = occupancies_bitboards[SIDE ^ 1];

    //get the right pieces for each side
    U64 pieces = pieces_bitboards[PIECE_TYPE + 6*SIDE];

    while (pieces) {
        source_square = pop_lsb(pieces);

        // get the coresponding attacks based on the piece 
        if constexpr (PIECE_TYPE == Pieces::B || PIECE_TYPE == Pieces::b) {
            attacks = get_bishop_attacks(source_square, both_occ);
        }
        else if constexpr (PIECE_TYPE == Pieces::R || PIECE_TYPE == Pieces::r) {
            attacks = get_rook_attacks(source_square, both_occ);
        }
        else { // Queen
            attacks = generate_queen_attacks(source_square, both_occ);
        }

        // a piece can't attack same side
        attacks &= ~friendly_occ;

        // ----------------- CAPTURES ------------------
        captures = attacks & enemy_occ;
        while (captures) {
            target_square = pop_lsb(captures);
            move_list.add_move(encode_move(source_square, target_square, 4));
        }

        // ----------------- QUIET MOVES ------------------
        quiet_targets = attacks & ~both_occ;
        while (quiet_targets) {
            target_square = pop_lsb(quiet_targets);
            move_list.add_move(encode_move(source_square, target_square, 0));
        }
    }
}