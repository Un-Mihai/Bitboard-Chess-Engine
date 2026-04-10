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