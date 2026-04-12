#pragma once

#include "Attacks.hpp"
#include "Types.hpp"
#include "Pieces.hpp"

inline U64 empty_bitboard = 0ULL;

//Bitboards having 0 on specific columns and 1 in rest
inline const U64 empty_a_file = 18374403900871474942ULL;
inline const U64 empty_ab_file = 18229723555195321596ULL;
inline const U64 empty_h_file = 9187201950435737471ULL;
inline const U64 empty_gh_file = 4557430888798830399ULL;

//pieces bitboards
inline U64 pieces_bitboard[12] = {
    white_pawns, white_knights, white_bishops, white_rooks, white_queen, white_king,
    black_pawns, black_knights, black_bishops, black_rooks, black_queen, black_king
};

//---------Pieces attacks for all squares----------//

inline U64 knight_attacks[64];
inline U64 pawn_attacks[2][64];
inline U64 king_attacks[64];

//there are maximum 2^12 = 4096 different attacks for a rook 
inline U64 rook_attacks[64][4096];
//there are maximum 2^9 = 512 different attacks for a bishop
inline U64 bishop_attacks[64][512];

//--------------------------------------------//

//Print bitboard in visual and numeric format
void printBitboard(U64 bitboard);

//Print the current board with unicode pieces
void printBoard();

//initialize the pawn attacks (diagonals) array for both sides
void init_pawn_attacks();

//initialize the knight attacks array
void init_knight_attacks();

//initialize the king attacks array
void init_king_attacks();

//initialize the rook attacks array
void init_rook_attacks();

//initialize the bishop attacks array
void init_bishop_attacks();