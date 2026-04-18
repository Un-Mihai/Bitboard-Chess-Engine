#include "core/Init.hpp"
#include "core/Board.hpp"
#include "movegen/Attacks.hpp"
#include "movegen/MagicNumbers.hpp"


void init_pawn_attacks(){
   for (int square = 0; square < 64; square ++){
      pawn_attacks[white][square] = generate_pawn_attacks(square, white);
      pawn_attacks[black][square] = generate_pawn_attacks(square, black);
   }
   
}

void init_knight_attacks(){
   for (int square = 0; square < 64; square ++){
        knight_attacks[square] = generate_knight_attacks(square);
   }
}

void init_king_attacks(){
   for (int square = 0; square < 64; square ++){
        king_attacks[square] = generate_king_attacks(square);
   }
}

void init_rook_attacks(){
   for (int square = 0; square < 64; square ++){
      // all the possible squares that could block the piece
      U64 blocks = generate_rook_blocks(square);
      int bits_in_mask = rook_occupancy_mask_count[square];

      int number_of_occupancies = (1ULL << bits_in_mask);  //2^n

      for (int count = 0; count < number_of_occupancies; count ++){
         U64 occupancy = generate_one_occupancy(blocks, count);
         
         int index = (occupancy * rook_magic_numbers[square]) >> (64 - bits_in_mask);
         //0 <= index <= 4095 because of the magic number we have found

         //generate and store the attack bitboard at index position
         U64 attacks = generate_rook_attacks(square, occupancy);
         rook_attacks[square][index] = attacks;
      }
   }
}

void init_bishop_attacks(){
   for (int square = 0; square < 64; square ++){
      // all the possible squares that could block the piece
      U64 blocks = generate_bishop_blocks(square);
      int bits_in_mask = bishop_occupancy_mask_count[square];

      int number_of_occupancies = (1ULL << bits_in_mask);  //2^n

      for (int count = 0; count < number_of_occupancies; count ++){
         U64 occupancy = generate_one_occupancy(blocks, count);
         
         int index = (occupancy * bishop_magic_numbers[square]) >> (64 - bits_in_mask);
         //0 <= index <= 511 because of the magic number we have found

         //generate and store the attack bitboard at index position
         U64 attacks = generate_bishop_attacks(square, occupancy);
         bishop_attacks[square][index] = attacks;
      }
   }
}

void init_magic_numbers() {
    for(int square = 0; square < 64; square++) {
        rook_magic_numbers[square] = find_magic_number(square, rook);
    }

    for(int square = 0; square < 64; square++) {
        bishop_magic_numbers[square] = find_magic_number(square, bishop);
    }
}