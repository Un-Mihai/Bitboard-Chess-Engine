#include "movegen/MoveGen.hpp"
#include "core/Types.hpp"
#include "core/Board.hpp"
#include "core/Logger.hpp"
#include "core/Rules.hpp"

void generate_moves(int side, MoveList& move_list){
    generate_pawn_captures(side, move_list);
    generate_pawn_pushes(side, move_list);
    
}

void generate_pawn_pushes(int side, MoveList& move_list){
    int source_square, target_square;
    
    U64 occupancy = occupancies_bitboards[both];
    U64 single_push_targets, double_push_targets, promotion_targets;

    if (side == white) {
        U64 pawns = pieces_bitboards[Pieces::P];

        // white pawns placed on rank 7 can promote
        U64 promoting_pawns = pawns & RANK_7;
        //select only the unoccupied squares for promoting
        promotion_targets = (promoting_pawns >> 8) & ~occupancy;

        while (promotion_targets) {
            //select and clear one propotion target square
            target_square = pop_lsb(promotion_targets);

            source_square = target_square + 8;
            //Queen promotion 
            move_list.add_move(encode_move(source_square, target_square, 11));
            //Rook promotion 
            move_list.add_move(encode_move(source_square, target_square, 10));
            // Bishop promotion 
            move_list.add_move(encode_move(source_square, target_square, 9));
            // Knight promotion 
            move_list.add_move(encode_move(source_square, target_square, 8));
        }

        // Quiet pawn pushes
        // all the pawns outside rank 7
        U64 quiet_pawns = pawns & ~RANK_7;
        
        // Single Push
        single_push_targets = (quiet_pawns >> 8) & ~occupancy;

        //copy the targets for double pushes
        U64 temp_single_targets = single_push_targets;

        // loop over the target squares for single pushes and add the coresponding move to the list
        while (temp_single_targets) {
            target_square = pop_lsb(temp_single_targets);
            move_list.add_move(encode_move(target_square + 8, target_square, 0));
        }

        // for double pushes, we check if the next square after the single push targets on rank 3 is also empty
        double_push_targets = ((single_push_targets & RANK_3) >> 8) & ~occupancy;
        while (double_push_targets) {
            target_square = pop_lsb(double_push_targets);
            move_list.add_move(encode_move(target_square + 16, target_square, 1));
        }
    } 
    else { // side = black
        U64 pawns = pieces_bitboards[Pieces::p];

        U64 promoting_pawns = pawns & RANK_2;
        promotion_targets = (promoting_pawns << 8) & ~occupancy;

        while (promotion_targets) {
            target_square = pop_lsb(promotion_targets);
            source_square = target_square - 8;
            move_list.add_move(encode_move(source_square, target_square, 11)); // Queen
            move_list.add_move(encode_move(source_square, target_square, 10)); // Rook
            move_list.add_move(encode_move(source_square, target_square, 9));  // Bishop
            move_list.add_move(encode_move(source_square, target_square, 8));  // Knight
        }

        U64 quiet_pawns = pawns & ~RANK_2;

        // Single Push
        single_push_targets = (quiet_pawns << 8) & ~occupancy;
        U64 temp_single_targets = single_push_targets;

        while (temp_single_targets) {
            target_square = pop_lsb(temp_single_targets);
            move_list.add_move(encode_move(target_square - 8, target_square, 0));
        }

        // Double Push
        double_push_targets = ((single_push_targets & RANK_6) << 8) & ~occupancy;
        while (double_push_targets) {
            target_square = pop_lsb(double_push_targets);
            move_list.add_move(encode_move(target_square - 16, target_square, 1));
        }
    }
}

void generate_pawn_captures(int side, MoveList& move_list){
    int source_square, target_square;

    // promoting pawns are pawns on rank 7 for white and 2 for black
    U64 promoting_pawns, normal_pawns;

    if (side == white){
        U64 white_pawns = pieces_bitboards[Pieces::P];
        promoting_pawns = white_pawns & RANK_7;
        normal_pawns = white_pawns & ~RANK_7;

        if (enpassant != Squares::no_square){
            U64 enpassant_attackers = pawn_attacks[black][enpassant] & white_pawns;
            while(enpassant_attackers){
                source_square = pop_lsb(enpassant_attackers);
                U32 move = encode_move(source_square, enpassant, 5);
                move_list.add_move(move);
            }
        }
    }
    else{
        U64 black_pawns = pieces_bitboards[Pieces::p];
        promoting_pawns = black_pawns & RANK_2;
        normal_pawns = black_pawns & ~RANK_2;

        if (enpassant != Squares::no_square){
            U64 enpassant_attackers = pawn_attacks[white][enpassant] & black_pawns;
            while(enpassant_attackers){
                source_square = pop_lsb(enpassant_attackers);
                U32 move = encode_move(source_square, enpassant, 5);
                move_list.add_move(move);
            }
        }
    }

    U64 pawn_captures;
    U64 enemy_pieces = occupancies_bitboards[1-side];

    // loop over the pawns that might promote
    while(promoting_pawns){
        source_square = pop_lsb(promoting_pawns);
        // calculate the occupied target squares
        pawn_captures = pawn_attacks[side][source_square] & enemy_pieces;

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
        pawn_captures = pawn_attacks[side][source_square] & enemy_pieces;

        while (pawn_captures){
            target_square = pop_lsb(pawn_captures);
            U32 move = encode_move(source_square, target_square, 4);
            move_list.add_move(move);
        }
    }
}