#include <iostream>
#include "Attacks.hpp"
#include "Types.hpp"
#include "BitManipulation.hpp"
#include "Board.hpp"

void functieExemplu() {
   setBit(main_bitboard, Squares::e5);
   printBitboard(main_bitboard);
   printBitboard(white_pawns);
   clearBit(main_bitboard, Squares::e4);
   printBitboard(main_bitboard);
}

uint64_t generate_knight_attacks (int square){
   uint64_t attacks = 0ULL;

   uint64_t knight_position_bitboard = 0ULL;
   setBit(knight_position_bitboard, square);

   //15,17,6,10

   //---------- LEFT SIDE ATTACKS -----------
   
   //if the knight is not placed on files a or b can go anywhere left
   if (empty_ab_file & knight_position_bitboard){
      attacks |= (knight_position_bitboard >> 10);
      attacks |= (knight_position_bitboard >> 17);
      attacks |= (knight_position_bitboard << 6);
      attacks |= (knight_position_bitboard << 15);
      
   }
   
   //prevent the knight from going to h file across the board
   else if(empty_a_file & knight_position_bitboard){
      attacks |= (knight_position_bitboard >> 17);
      attacks |= (knight_position_bitboard << 15);
      
   }

   //else the knight can't move to the left


   //---------- RIGHT SIDE ATTACKS -----------

   //if the knight is not placed on files g or h can go anywhere right
   if (empty_gh_file & knight_position_bitboard){
      attacks |= (knight_position_bitboard << 10);
      attacks |= (knight_position_bitboard << 17);
      attacks |= (knight_position_bitboard >> 6);
      attacks |= (knight_position_bitboard >> 15);
      
   }
   
   //prevent the knight from going to a file across the board
   else if(empty_h_file & knight_position_bitboard){
      attacks |= (knight_position_bitboard << 17);
      attacks |= (knight_position_bitboard >> 15);
      
   }

   //else the knight can't move to the right

   //OBS: Checking the ranks is not necessary because the bits shifted above or below the bitboard will overflow the number and will be ignored
   
   return attacks;
}