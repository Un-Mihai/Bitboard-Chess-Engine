#include "Attacks.hpp"
#include "core/Types.hpp"
#include "movegen/MoveGen.hpp"
#include "core/Rules.hpp"
#include "core/Board.hpp"
#include "core/Pieces.hpp"

#include <cstdlib>


inline void unmake_move(U32 move){
    int source_square = get_move_source_square(move);
    int target_square = get_move_target_square(move);

    int opponent_side = side;

    // change the side to the one who made the move
    side = side ^ 1;

    Game_history::State past_state = game_history.pop_state();
    castle = past_state.castle_rights;
    enpassant = past_state.enpassant_square;
    moves_remaining = past_state.fifty_move_counter;

    int captured_piece = past_state.captured_piece;
    int moving_piece = board.get_piece(target_square);


    // remove the promoted piece and place the pawn instead
    if (is_move_promotion(move)){
        int promoted_piece = board.remove_and_get_piece(target_square);
        moving_piece = Pieces::P + 6*side;
        
        board.set_piece(target_square, moving_piece);

        clearBit(pieces_bitboards[promoted_piece], target_square);
        setBit(pieces_bitboards[moving_piece], target_square);
    }

    // restore board and bitboards for the moving piece
    board.move_piece(target_square, source_square, moving_piece);

    moveBit(pieces_bitboards[moving_piece], target_square, source_square);
    moveBit(occupancies_bitboards[side], target_square, source_square);

    if (is_move_capture(move)){
        // restore en passant captured pawn
        if (is_move_enpassant(move)){
            // target + 8 for white and  target - 8 for black
            int captured_square =  target_square + 8 - 16*side;
            board.set_piece(captured_square, captured_piece);
            setBit(pieces_bitboards[captured_piece], captured_square);
            setBit(occupancies_bitboards[opponent_side], captured_square);
        }
        else{
            board.set_piece(target_square, captured_piece);
            setBit(pieces_bitboards[captured_piece], target_square);
            setBit(occupancies_bitboards[opponent_side], target_square);
        }
    } 
   

    if (is_move_king_castle(move)){
        // calculate rook position based on king (works for both sides)
        int rook_target_square = source_square + 3;
        int rook_source_square = source_square + 1;

        // move the rook in board and bitboard
        int rook = board.remove_and_get_piece(rook_source_square);
        board.set_piece(rook_target_square, rook);

        moveBit(pieces_bitboards[rook], rook_source_square, rook_target_square);
        moveBit(occupancies_bitboards[side], rook_source_square, rook_target_square);
    }

    if (is_move_queen_castle(move)){
        // calculate rook position based on king (works for both sides)
        int rook_target_square = source_square - 4;
        int rook_source_square = source_square - 1;

        // move the rook in board and bitboard
        int rook = board.remove_and_get_piece(rook_source_square);
        board.set_piece(rook_target_square, rook);

        moveBit(pieces_bitboards[rook], rook_source_square, rook_target_square);
        moveBit(occupancies_bitboards[side], rook_source_square, rook_target_square);
    }
    
    occupancies_bitboards[both] = occupancies_bitboards[white] | occupancies_bitboards[black];
}

inline bool make_move(U32 move){
    int source_square = get_move_source_square(move);
    int target_square = get_move_target_square(move);
    bool is_capture = is_move_capture(move);
    bool is_enpassant = is_move_enpassant(move);
    bool is_double_push = is_move_double_pawn_push(move);
    bool is_king_castle = is_move_king_castle(move);
    bool is_queen_castle = is_move_queen_castle(move);
    bool is_promotion = is_move_promotion(move);
    int moving_piece = board.get_piece(source_square);
    int captured_piece;
    int opponent_side = side ^ 1;

    if (is_capture){
        // handle enpassant
        if (is_enpassant){
            // target + 8 for white and  target - 8 for black
            int captured_square =  target_square + 8 - 16*side;
            captured_piece = board.remove_and_get_piece(captured_square);
            clearBit(pieces_bitboards[captured_piece], captured_square);
            clearBit(occupancies_bitboards[opponent_side], captured_square);
        }
        else{
            captured_piece = board.get_piece(target_square);
            clearBit(pieces_bitboards[captured_piece], target_square);
            clearBit(occupancies_bitboards[opponent_side], target_square);
        }
    } 
    else captured_piece = Pieces::no_piece;

    // move the piece on the board (captured pieces are overwritten, except for en passant)
    board.move_piece(source_square, target_square, moving_piece);
    
    // update the bitboards
    moveBit(pieces_bitboards[moving_piece], source_square, target_square);
    moveBit(occupancies_bitboards[side], source_square, target_square);

    // handle promotion
    if (is_promotion){
        int promotion_type = get_promotion_type(move);
        int promoted_piece = promotion_pieces[side][promotion_type];

        // update pieces bitboards
        clearBit(pieces_bitboards[moving_piece], target_square);
        setBit(pieces_bitboards[promoted_piece], target_square);

        // place the promoted piece on the board
        board.set_piece(target_square, promoted_piece);
        // the pawn has been replaced with a same side piece, so occupancies arrays stay intact
    }

    // handle castle
    if (is_king_castle){
        // calculate rook position based on king (works for both sides)
        int rook_source_square = source_square + 3;
        int rook_target_square = source_square + 1;

        // move the rook in board and bitboard
        int rook = board.remove_and_get_piece(rook_source_square);
        board.set_piece(rook_target_square, rook);

        moveBit(pieces_bitboards[rook], rook_source_square, rook_target_square);
        moveBit(occupancies_bitboards[side], rook_source_square, rook_target_square);
    }

    if (is_queen_castle){
        // calculate rook position based on king (works for both sides)
        int rook_source_square = source_square - 4;
        int rook_target_square = source_square - 1;

        // move the rook in board and bitboard
        int rook = board.remove_and_get_piece(rook_source_square);
        board.set_piece(rook_target_square, rook);

        moveBit(pieces_bitboards[rook], rook_source_square, rook_target_square);
        moveBit(occupancies_bitboards[side], rook_source_square, rook_target_square);
    }

    occupancies_bitboards[both] = occupancies_bitboards[side] | occupancies_bitboards[opponent_side];

    // save curent state
    game_history.push_state(castle, enpassant, captured_piece, moves_remaining);

    // update rules
    enpassant = is_double_push ? source_square - 8 + 16*side : Squares::no_square;

    if (moving_piece == Pieces::P || moving_piece == Pieces::p || is_capture)
        moves_remaining = 100;
    else moves_remaining --;

    //update castle rights
    castle &= castle_rights_mask[source_square];
    castle &= castle_rights_mask[target_square];
    
    int king_type = Pieces::K + 6*side;
    side = opponent_side;

    // make sure the move is legal (king not in check)
    int king_square = first_lsb(pieces_bitboards[king_type]);
    if (is_square_attacked(king_square, side)){
        unmake_move(move);
        return false;
    }
    return true;
}