#pragma once

#include "Attacks.hpp"
#include "Types.hpp"

inline U64 main_bitboard = 0ULL;

inline U64 black_pawns = 65280ULL;
inline U64 white_pawns = 71776119061217280ULL;

inline U64 black_king = 16ULL;
inline U64 white_king = 1152921504606846976ULL;

inline U64 black_queen = 8ULL;
inline U64 white_queen = 576460752303423488ULL;

inline U64 black_bishops = 36ULL;
inline U64 white_bishops = 2594073385365405696ULL;

inline U64 black_knights = 66ULL;
inline U64 white_knights = 4755801206503243776ULL;

inline U64 black_rooks = 129ULL;
inline U64 white_rooks = 9295429630892703744ULL;

//--------------------------------------------//

//Bitboards having 0 on specific columns and 1 in rest
inline U64 empty_a_file = 18374403900871474942ULL;
inline U64 empty_ab_file = 18229723555195321596ULL;
inline U64 empty_h_file = 9187201950435737471ULL;
inline U64 empty_gh_file = 4557430888798830399ULL;

//--------------------------------------------//

//Pieces attacks for all squares
inline U64 knight_attacks[64];

//--------------------------------------------//

//Print bitboard in visual and numeric format
void printBitboard(U64 bitboard);

//initialize the knight attacks array
void init_knight_attacks();
