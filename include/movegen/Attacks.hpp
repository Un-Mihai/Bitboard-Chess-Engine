#pragma once

#include <cstdint>

#include "core/Types.hpp"
#include "utils/BitManipulation.hpp"

/*---------PIECES ATTACS FOR ALL SQUARES---------*/

inline U64 knight_attacks[64];
inline U64 pawn_attacks[2][64];
inline U64 king_attacks[64];

//there are maximum 2^12 = 4096 different attacks for a rook 
inline U64 rook_attacks[64][4096];
//there are maximum 2^9 = 512 different attacks for a bishop
inline U64 bishop_attacks[64][512];


/*----------OCCUPANCIES MASKS-----------*/

//bit count arrays for occupancy masks
inline const int bishop_occupancy_mask_count[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

inline const int rook_occupancy_mask_count[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};


/*------ GENERATE LEAPER PIECES' ATTACKS--------*/

//mark the bits a knight can go from a given square (side doesn't matter)
U64 generate_knight_attacks (int square);

//mark the bits a pawn can attack (diagonals) (sides matter)
U64 generate_pawn_attacks(int square, int color);

//mark the bits a king can attack (check not accounted for) (side doesn't matter)
U64 generate_king_attacks(int square);


/*------GENERATE SLIDING PIECES' ATTACKS-------*/

//mark the positions that could block a bishop placed on a given square (side doesn't matter)
U64 generate_bishop_blocks(int square);

//mark the positions that could block a rook placed on a given square (side doesn't matter)
U64 generate_rook_blocks(int square);

//mark the bits a bishop can go based on a given bitboard with blocks
U64 generate_bishop_attacks(int square, U64 blocks);

//mark the bits a rook can go to based on a given bitboard with blocks
U64 generate_rook_attacks(int square, U64 blocks);

//generate the index-th bitboard among all the possible blocker configurations
U64 generate_one_occupancy(U64 blocks, int index);



