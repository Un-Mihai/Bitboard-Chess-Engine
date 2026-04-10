#include <iostream>
#include "Attacks.hpp"
#include "Types.hpp"
#include "BitManipulation.hpp"
#include "Board.hpp"

U64 generate_knight_attacks (int square){
   U64 attacks = 0ULL;

   U64 knight_position_bitboard = 0ULL;
   setBit(knight_position_bitboard, square);

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

U64 generate_pawn_attacks(int square, int color){
   
   U64 attacks = 0ULL;

   U64 pawn_position_bitboard = 0ULL;
   setBit(pawn_position_bitboard, square);

   //if the color is white the pawn shifts right else it shifts left
   if (color == white){

      //prevent the pawn from capturing on the h file from the a file
      if(pawn_position_bitboard & empty_a_file) 
         attacks |= (pawn_position_bitboard >> 9);
      
      //prevent the pawn from capturing on the a file from the h file
      if(pawn_position_bitboard & empty_h_file)
         attacks |= (pawn_position_bitboard >> 7);
   }

   //conditions are inverted for the black pawns
   else {
      if(pawn_position_bitboard & empty_a_file) 
         attacks |= (pawn_position_bitboard << 7);
      if(pawn_position_bitboard & empty_h_file)
         attacks |= (pawn_position_bitboard << 9);
   }

   return attacks;
}

U64 generate_king_attacks(int square){

   U64 attacks = 0ULL;

   U64 king_position_bitboard = 0ULL;
   setBit(king_position_bitboard, square);

   //generate the front/back moves, if the king is on the first/last rank it will overflow so it doesn't need checking
   attacks |= king_position_bitboard >> 8;
   attacks |= king_position_bitboard << 8;

   //preventing the king from capturing over the board, same rules as the pawns for the diagonals, added horizontal movement
   if(king_position_bitboard & empty_a_file){
      attacks |= king_position_bitboard >> 9;
      attacks |= king_position_bitboard >> 1;
      attacks |= king_position_bitboard << 7;
   }

   if(king_position_bitboard & empty_h_file){
      attacks |= king_position_bitboard << 9;
      attacks |= king_position_bitboard << 1;
      attacks |= king_position_bitboard >> 7;
   }

   return attacks;
}

U64 generate_bishop_blocks(int square){
   U64 blocks = 0ULL;

   //calculate bishop coordinates
   int bishop_rank = square / 8;
   int bishop_file = square % 8;

   //skip the edges because a piece on the edge doesn't block the bishop

   //loop towards bottom right corner
   for ( int rank = bishop_rank + 1, file = bishop_file + 1; rank <= 6 && file <= 6; rank ++, file ++)
      setBit(blocks, rank * 8 + file);

   //loop towards upper right corner
   for ( int rank = bishop_rank - 1, file = bishop_file + 1; rank >= 1 && file <= 6; rank --, file ++)
      setBit(blocks, rank * 8 + file);

   //loop towards bottom left corner
   for ( int rank = bishop_rank + 1, file = bishop_file - 1; rank <= 6 && file >= 1; rank ++, file --)
      setBit(blocks, rank * 8 + file);

   //loop towards upper left corner
   for ( int rank = bishop_rank - 1, file = bishop_file - 1; rank >= 1 && file >= 1; rank --, file --)
      setBit(blocks, rank * 8 + file);

   return blocks;
}

U64 generate_bishop_attacks(int square, U64 blocks){\
   U64 attacks = 0ULL;

   //calculate bishop coordinates
   int bishop_rank = square / 8;
   int bishop_file = square % 8;

   //loop towards bottom right corner
   for ( int rank = bishop_rank + 1, file = bishop_file + 1; rank <= 7 && file <= 7; rank ++, file ++){
      int current_square = rank * 8 + file;
      setBit(attacks, current_square);
      if (getBit(blocks, current_square)) break;

   }
      
   //loop towards upper right corner
   for ( int rank = bishop_rank - 1, file = bishop_file + 1; rank >= 0 && file <= 7; rank --, file ++){
      int current_square = rank * 8 + file;
      setBit(attacks, current_square);
      if (getBit(blocks, current_square)) break;
   }

   //loop towards bottom left corner
   for ( int rank = bishop_rank + 1, file = bishop_file - 1; rank <= 7 && file >= 0; rank ++, file --){
      int current_square = rank * 8 + file;
      setBit(attacks, current_square);
      if (getBit(blocks, current_square)) break;
   }

   //loop towards upper left corner
   for ( int rank = bishop_rank - 1, file = bishop_file - 1; rank >= 0 && file >= 0; rank --, file --){
      int current_square = rank * 8 + file;
      setBit(attacks, current_square);
      if (getBit(blocks, current_square)) break;
   }

   return attacks;
}

U64 generate_one_occupancy(U64 blocks, int index) {
    U64 occupancy = 0ULL;

    //loop over all the possible occupancies
    while (blocks) {
        int square = first_lsb(blocks);
        clearBit(blocks, square);

        // if least significant bit of index is 1 than we keep the square occupied
        if (index & 1)
            setBit(occupancy, square);

        index >>= 1;  //move to the next bit
    }

    return occupancy;
}

U64 generate_rook_blocks(int square){

   U64 blocks = 0ULL;

   //get the rook's position
   int rook_rank = square / 8;
   int rook_file = square % 8;

   //loop vertically
   for(int rank = 1; rank < 7; rank ++){
      setBit(blocks, rank * 8 + rook_file);
   }

   //loop horizontally
   for(int file = 1; file < 7; file ++){
      setBit(blocks, rook_rank * 8 + file);
   }

   //clear the bit that represents the rook's position
   clearBit(blocks, square);

   return blocks;
}

U64 generate_rook_attacks(int square, U64 blocks){

   U64 attacks = 0ULL;

   //get the rook's position
   int rook_rank = square / 8;
   int rook_file = square % 8;

   //loop up
   for(int rank = rook_rank + 1; rank < 8; rank ++){
      int current_square = rank * 8 + rook_file;
      setBit(attacks, current_square);
      if(getBit(blocks, current_square)) break;
   }

   //loop down
   for(int rank = rook_rank - 1; rank > -1; rank --){
      int current_square = rank * 8 + rook_file;
      setBit(attacks, current_square);
      if(getBit(blocks, current_square)) break;
   }

   //loop right
   for(int file = rook_file + 1; file < 8; file ++){
      int current_square = rook_rank * 8 + file;
      setBit(attacks, current_square);
      if(getBit(blocks, current_square)) break;
   }

   //loop left
   for(int file = rook_file - 1; file > -1; file --){
      int current_square = rook_rank * 8 + file;
      setBit(attacks, current_square);
      if(getBit(blocks, current_square)) break;
   }

   return attacks;
   
}

//fisierul incepe sa fie mai voluminos decat credeam... imi zici daca crezi ca e nevoie de o restructurare la fisiere