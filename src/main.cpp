#include<iostream>
#include<bitset>

#include "core/Types.hpp" //fisier in care definesc enum-urile si tipulde date U64 (e mai scurt) 
#include "utils/BitManipulation.hpp" //fisier in care tinem functiile de manipulare a bitilor
#include "movegen/Attacks.hpp" //header in care tinem functiile de generare a atacurilor si variabilele aferente
#include "core/Board.hpp" //header in care tinem bitboardurile principale si functiile generale (de ex avem print ul acum)
#include "movegen/MagicNumbers.hpp" //header in care generam magic numbers
#include "core/Pieces.hpp" //in asta tinem bitboardurile cu pisese, si sirul cu unicodes pentru afisare
#include "core/Init.hpp" //fisier care initializeaza ce avem nevoie
#include "movegen/MoveGen.hpp"

//--------Windows only-----------
#ifdef _WIN32
#include <windows.h>
#endif

//make sure windows console can output fancy pieces
void setup_console() {
#ifdef _WIN32
    // Set Windows console to use UTF-8
    SetConsoleOutputCP(65001);
#endif
}
//---------------------------------

int main() {
   setup_console();

   init_all();

   //printBitboard(attacked_squares(white));
   //printBitboard(occupancies_bitboards[black]);

   // printBitboard(white_pawns);
   // setBit(main_bitboard, Squares::e4);
   // printBitboard(main_bitboard);
   // functieExemplu();


   //----- TEMPORARY CODE(might be reused) ----------

   // for (int i = 0; i < 64; i ++)
   //    printBitboard(knight_attacks[i]);

    uint64_t b = empty_bitboard;
   // for(int rank = 0; rank < 8; rank ++){
   //    for (int file = 0; file < 6; file ++){
   //       setBit(b, rank * 8 + file);
   //    }
   // }
   // printBitboard(b);
   
   // std :: cout << first_lsb(b);

   //  setBit(b, Squares::e5);
   //  setBit(b, Squares::b6);
   //  setBit(b, Squares::d6);

   // printBitboard(generate_bishop_attacks(Squares::d4, b));
   //printBitboard(generate_bishop_blocks(Squares::d1));

   /*printBitboard( 
      generate_one_occupancy(
          generate_bishop_blocks(Squares::d4), 100
       )
    );
   */
   // for (int square = 0; square < 64; square ++){
   //    U64 occupancy = generate_rook_blocks(square);
   //    std :: cout << bitCount (occupancy ) << ", ";
   //    if (square % 8 == 7)
   //       std :: cout << '\n';
   // }
   
   // for (int i = 0; i < 12; i ++){
   //    std :: cout << unicode_pieces[i] << ' ';
   // }

   //printBitboard(pieces_bitboard[Pieces :: P]);
   //printBoard();

   // U64 rook_attacks = get_rook_attacks(Squares::d4, b);
   // printBitboard(rook_attacks);


   U32 move = encode_move(Squares::h1, Squares::h1, 0b0100, 0b1000000000000001);
   std::cout << std::bitset<32>(move) << '\n' << is_move_capture(move) << ' ' << is_move_promotion(move) << '\n';

   parse_fen_string(fen_move_generation_testing);
   printBoard();

   MoveList move_list;
   generate_moves(white, move_list);

   for (int i = 0; i < move_list.count; i ++){
      U32 move = move_list.moves[i];
      int flags = get_move_flags(move);
      
      std::cout << "Mutarea " << i+1 << ": " 
                << square_to_coordinates[get_move_source_square(move)] << "->"
                << square_to_coordinates[get_move_target_square(move)] 
                << " | Captura: " << (is_move_capture(move) ? "Da" : "Nu")
                << " | Promovare: " << (is_move_promotion(move) ? "Da" : "Nu")
                << " | EP: " << (is_move_enpassant(move) ? "Da" : "Nu")
                << " | DoublePush: " << (is_move_double_pawn_push(move) ? "Da" : "Nu")
                << " | Rocada(K/Q): " << is_move_king_castle(move) << "/" << is_move_queen_castle(move)
                << " | Flags: " << flags << '\n';
   }
} 
