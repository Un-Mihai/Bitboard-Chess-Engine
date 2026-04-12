#include <iostream>

#include "Board.hpp"
#include "BitManipulation.hpp"

void printBitboard(U64 bitboard) {
   for (int rank = 0; rank < 8; rank++) {
      std::cout << ' ' << 8 - rank << "  ";
      for (int file = 0; file < 8; file++) {
         std::cout << getBit(bitboard, rank * 8 + file) << ' ';
      }
      std::cout << '\n';
   }
   std::cout << '\n' << "    a b c d e f g h" << "\n\n";
   std::cout << "Bitboard: " << bitboard << "\n\n";
}

void printBoard(){
   for (int rank = 0; rank < 8; rank++){
      std::cout << ' ' << 8 - rank << "  ";
      for (int file = 0; file < 8; file++){
         
         int square = rank * 8 + file;
         bool piece_on_square = false;

         for (int piece = 0; piece < 12; piece ++){
            if ( getBit(pieces_bitboard[piece], square) ){
               std :: cout << unicode_pieces[piece] << ' ';
               piece_on_square = true;
               break;
            }
         }
         if (!piece_on_square)
            std :: cout << '.' << ' ';
      }
      std::cout << '\n';
   }
   std::cout << '\n' << "    a b c d e f g h" << "\n\n";
}

void init_pawn_attacks(){
   for (int square = 0; square < 64; square ++){
      pawn_attacks[white][square] = generate_pawn_attacks(square, white);
      pawn_attacks[black][square] = generate_pawn_attacks(square, black);
   }
   
}

void init_knight_attacks(){
   for (int square = 0; square < 64; square ++)
      knight_attacks[square] = generate_knight_attacks(square);
}

void init_king_attacks(){
   for (int square = 0; square < 64; square ++)
      king_attacks[square] = generate_king_attacks(square);
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