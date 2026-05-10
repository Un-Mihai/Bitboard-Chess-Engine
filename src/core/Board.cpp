#include <iostream>
#include <string.h>

#include "core/Board.hpp"
#include "utils/BitManipulation.hpp"
#include "movegen/MagicNumbers.hpp"
#include "core/Rules.hpp"
#include "core/Types.hpp"
#include "core/Logger.hpp"

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
            if ( getBit(pieces_bitboards[piece], square) ){
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

void parse_fen_string(const char* fen){

   //reset main bitboards and occupancies
   memset(pieces_bitboards, 0ULL, sizeof(pieces_bitboards));
   memset(occupancies_bitboards, 0ULL, sizeof(occupancies_bitboards));
   board.clear_board();

   //reset game state variables
   side = 0;
   castle = 0;
   enpassant = Squares::no_square;

   //place pieces on the boards by looping the fen string and board squares simultaniously
   for(int rank = 0; rank < 8; rank ++){
      for(int file = 0; file < 8; file ++){

         int square = rank * 8 + file;

         //if the current character is a / separator we just ignore it
         if(*fen == '/'){
            *fen ++;
         }

         //check for the pieces
         if((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')){
            int piece = parse_piece_char_to_int(*fen);

            //if the current character does not represent a valid piece log a waring and leave the square empty
            if(piece == -1){
               warn("Invalid character in fen string");
               *fen ++;
               continue;
            }

            setBit(pieces_bitboards[piece], square);
            board.set_piece(square, piece);
            *fen ++;
            continue;
         }

         //check for empty spaces
         if(*fen >= '0' && *fen <= '9'){
            file += *fen - '0' - 1;
            *fen ++;
            continue;
         }
      }
   }

   //moving pointer to the side to move character
   *fen ++;
   (*fen == 'w') ? (side = white) : (side = black);

   //moving pointer to the castling rights and parsing them
   fen += 2;
   while(*fen != ' '){

      switch(*fen){
         case 'K' : castle |= wk; break;
         case 'Q' : castle |= wq; break;
         case 'k' : castle |= bk; break;
         case 'q' : castle |= bq; break;
         case '-' : break;
      }

      *fen ++;
   }

   //moving pointer to enpassant square and parsing it
   *fen ++;
   if(*fen != '-'){

      int file = fen[0] - 'a';
      int rank = 8 - (fen[1] - '0');

      enpassant = rank * 8 + file;
      
   } else{
      enpassant = Squares::no_square;
   }

   //init white occupancies
   for(int piece = Pieces::P; piece <= Pieces::K; piece ++){
      //populate white occupancy bitboard;
      occupancies_bitboards[white] |= pieces_bitboards[piece];
      occupancies_bitboards[both] |= pieces_bitboards[piece];
   }

   //init black occupancies
   for(int piece = Pieces::p; piece <= Pieces::k; piece ++){
      //populate black occupancy bitboard;
      occupancies_bitboards[black] |= pieces_bitboards[piece];
      occupancies_bitboards[both] |= pieces_bitboards[piece];
   }
}

