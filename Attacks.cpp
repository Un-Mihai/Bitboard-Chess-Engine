#include <iostream>
#include "Attacks.hpp"
#include "Types.hpp"
#include "BitManipulation.hpp"
#include "Board.hpp"

void functieExemplu() {
   setBit(main_bitboard, Squares::e4);
   printBitboard(main_bitboard);
   printBitboard(white_pawns);
   clearBit(main_bitboard, Squares::e4);
   printBitboard(main_bitboard);
}