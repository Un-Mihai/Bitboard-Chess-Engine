#pragma once

#include <cstdint>

#include "Types.hpp"
#include "BitManipulation.hpp"

void functieExemplu();

//mark the bits a knight can go from a given square (side doesn't matter)
U64 generate_knight_attacks (int square);

//mark the bits a pawn can attack (diagonals) (sides matter)
U64 generate_pawn_attacks(int square, int color);

//mark the bits a king can attack (check not accounted for) (side doesn't matter)
U64 generate_king_attacks(int square);

//mark the positions that could block a bishop placed on a given square (side doesn't matter)
U64 generate_bishop_blocks(int square);

//mark the bits a bishop can go based on a given bitboard with blocks
U64 generate_bishop_attacks(int square, U64 blocks);