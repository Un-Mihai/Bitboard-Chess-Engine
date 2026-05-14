#pragma once

#include <cstdint>

#include "core/Types.hpp"
#include "utils/BitManipulation.hpp"
#include "MagicNumbers.hpp"
#include "core/Board.hpp"

/*---------PIECES ATTACKS FOR ALL SQUARES---------*/

inline U64 knight_attacks[64];
inline U64 pawn_attacks[2][64];
inline U64 king_attacks[64];

//there are maximum 2^12 = 4096 different attacks for a rook 
inline U64 rook_attacks[64][4096];
//there are maximum 2^9 = 512 different attacks for a bishop
inline U64 bishop_attacks[64][512];

/*-------- BLOCKS FOR ROOK AND BISHOP -------------*/

inline U64 rook_blocks[64];
inline U64 bishop_blocks[64];

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

//mark the bits a queen can go to based on a given bitboard with blocks
inline U64 generate_queen_attacks(int square, U64 blocks){
    //queen = rook + bishop
    return generate_bishop_attacks(square, blocks) | generate_rook_attacks(square, blocks);
};

//generate the index-th bitboard among all the possible blocker configurations
U64 generate_one_occupancy(U64 blocks, int index);

/*--------- GET ATTACKS FOR ROOK AND BISHOP -----------*/

// returns attacks bitboard from the precalculated rook_attacks array
inline U64 get_rook_attacks(int square, U64 blocks){
    int index = ((blocks & rook_blocks[square]) * rook_magic_numbers[square]) >> (64 - rook_occupancy_mask_count[square]);
    return rook_attacks[square][index];
};

// returns attacks bitboard from the precalculated bishop_attacks array
inline U64 get_bishop_attacks(int square, U64 blocks){
    int index = ((blocks & bishop_blocks[square]) * bishop_magic_numbers[square]) >> (64 - bishop_occupancy_mask_count[square]);
    return bishop_attacks[square][index];
};

/*------------------------------------------------*/

//checks whether a square is attacked by the side to move
inline bool is_square_attacked(int square, int side_to_move){
   int offset = side_to_move*6; // used to select the right color for pieces (values are: 0 for white, 6 for black)

   // if a pawn of opposite color, hypothetically placed on the square, would attack an existing pawn 
   // then the square is attacked by that existing pawn
   if (pawn_attacks[1^side_to_move][square] & pieces_bitboards[Pieces::P + offset])
      return 1;

   //same for the other pieces

   if (knight_attacks[square] & pieces_bitboards[Pieces::N + offset])
      return 1;

   if (king_attacks[square] & pieces_bitboards[Pieces::K + offset])
      return 1;

   // for the sliding pieces we must find the attacks for the specific current position
   U64 occupancies = occupancies_bitboards[both];

   // check horizontal queen attack or rook attack
   if (get_rook_attacks(square, occupancies) & (pieces_bitboards[Pieces::R + offset] |
                                                pieces_bitboards[Pieces::Q + offset] ))
      return 1;

   // check diagonal queen attack or bishop attack
   if (get_bishop_attacks(square, occupancies) & (pieces_bitboards[Pieces::B + offset] |
                                                pieces_bitboards[Pieces::Q + offset] ))
      return 1;

   // else, the square is not attacked
   return 0;
}

//creates a bitboard marking the attacked squares
U64 attacked_squares(int side_to_move);