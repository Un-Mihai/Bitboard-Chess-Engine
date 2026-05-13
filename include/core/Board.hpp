#pragma once

#include "movegen/Attacks.hpp"
#include "Types.hpp"
#include "Pieces.hpp"


/*------------------FEN DEBUG POSITIONS---------------*/

inline const char* fen_empty_board = "8/8/8/8/8/8/8/8 w - - ";
inline const char* fen_start_position_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkQ - 0 1 ";
inline const char* fen_tricky_position_board = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
inline const char* fen_killer_position_board = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
inline const char* fen_cmk_position_board = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
inline const char* fen_move_generation_testing = "r3k2r/pPppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 ";
inline const char* fen_kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";


/*-----------------MAIN BITBOARDS---------------------*/

inline U64 empty_bitboard = 0ULL;

//Bitboards having 0 on specific columns and 1 in rest
inline const U64 empty_a_file = 18374403900871474942ULL;
inline const U64 empty_ab_file = 18229723555195321596ULL;
inline const U64 empty_h_file = 9187201950435737471ULL;
inline const U64 empty_gh_file = 4557430888798830399ULL;

//Bitboards having 1 on specific ranks and 0 in rest
inline const U64 RANK_2 = 71776119061217280ULL;
inline const U64 RANK_3 = 280375465082880ULL;
inline const U64 RANK_6 = 16711680ULL;
inline const U64 RANK_7 = 65280ULL;

//pieces bitboards
inline U64 pieces_bitboards[12] = {
    white_pawns, white_knights, white_bishops, white_rooks, white_queen, white_king,
    black_pawns, black_knights, black_bishops, black_rooks, black_queen, black_king
};

//occupancies bitboards (white pieces, black pieces, all pieces)
inline U64 occupancies_bitboards[3];



/*-----------BITBOARD MANIPULATION-----------------*/

//Print bitboard in visual and numeric format
void printBitboard(U64 bitboard);

//Print the current board with unicode pieces
void printBoard();

void parse_fen_string(const char*);

// array representing the piece placed on every square + useful functions
struct Board_pieces{
    int board_pieces[64];

    inline int get_piece(int square) {return board_pieces[square];}

    inline void set_piece(int square, int piece){
        board_pieces[square] = piece;
    }

    inline void remove_piece(int square){
        board_pieces[square] = Pieces::no_piece;
    }

    inline int remove_and_get_piece(int square){
        int removed_piece = board_pieces[square];
        board_pieces[square] = Pieces::no_piece;
        return removed_piece;
    }

    inline void move_piece(int source_square, int target_square, int piece){
        board_pieces[target_square] = piece;
        board_pieces[source_square] = Pieces::no_piece;
    }

    inline void clear_board(){
        for (int i = 0; i < 64; i ++)
            board_pieces[i] = Pieces::no_piece;
    }
}inline board;     