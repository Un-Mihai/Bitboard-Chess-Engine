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