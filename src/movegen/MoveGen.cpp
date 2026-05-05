#include "movegen/MoveGen.hpp"
#include "core/Types.hpp"
#include "core/Board.hpp"
#include "core/Logger.hpp"

void generate_moves(int side, MoveList& move_list){
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