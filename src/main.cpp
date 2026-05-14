#include<iostream>
#include<bitset>
#include <cstdlib>
#include <chrono>

#include "core/Types.hpp" //fisier in care definesc enum-urile si tipulde date U64 (e mai scurt) 
#include "utils/BitManipulation.hpp" //fisier in care tinem functiile de manipulare a bitilor
#include "movegen/Attacks.hpp" //header in care tinem functiile de generare a atacurilor si variabilele aferente
#include "core/Board.hpp" //header in care tinem bitboardurile principale si functiile generale (de ex avem print ul acum)
#include "movegen/MagicNumbers.hpp" //header in care generam magic numbers
#include "core/Pieces.hpp" //in asta tinem bitboardurile cu pisese, si sirul cu unicodes pentru afisare
#include "core/Init.hpp" //fisier care initializeaza ce avem nevoie
#include "movegen/MoveGen.hpp"
#include "movegen/MoveHandling.hpp"
#include "core/Rules.hpp"

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

long long count_moves(int depth) {
    if (depth == 0) return 1ULL;

    MoveList move_list;
    generate_moves(side, move_list);

    // La ultimul nivel doar numaram, nu mai facem recursivitate
    if (depth == 1) {
        long long count = 0;
        for (int i = 0; i < move_list.count; i++) {
            if (make_move(move_list.moves[i])) {
                count++;
                unmake_move(move_list.moves[i]);
            }
        }
        return count;
    }

    long long nodes = 0;

    for (int i = 0; i < move_list.count; i++) {
        if (make_move(move_list.moves[i])) {
            nodes += count_moves(depth - 1);
            unmake_move(move_list.moves[i]);
        }
    }

    return nodes;
}

void play_random_game(){
   parse_fen_string(fen_start_position_board);
   printBoard();

   MoveList move_list;
   srand(time(NULL));

   for(int k = 1; k <= 100; k ++){
      move_list.count = 0;
      generate_moves(side, move_list);
      while (true){
         //std::getchar();
         int random_index = rand() % move_list.count;
         U32 move = move_list.moves[random_index];
         if ( make_move(move) == false ){
            std::cout << square_to_coordinates[get_move_source_square(move)] << " -> " 
                      << square_to_coordinates[get_move_target_square(move)] << ": illegal move" << '\n';
            printBoard();
            std::cout << "Unmaking move..." << '\n';
            std::cout << "State restored." << '\n';
         }
         else break;
      }
      printBoard();
   }
}

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

   

   
   //U32 move = encode_move(Squares::h1, Squares::h1, 0b0100, 0b1000000000000001);
   //std::cout << std::bitset<32>(move) << '\n' << is_move_capture(move) << ' ' << is_move_promotion(move) << '\n';

   //play_random_game();

   parse_fen_string(fen_start_position_board);

   //parse_fen_string(fen_kiwipete);

   auto start = std::chrono::high_resolution_clock::now();
   std::cout << count_moves(7) << '\n';
   auto stop = std::chrono::high_resolution_clock::now();

   std::chrono::duration<double> durata = stop - start;
   std::cout << "A durat " << durata.count() << " secunde\n";
   

   // for (int i = 0; i < move_list.count; i ++){
   //    U32 move = move_list.moves[i];
   //    int flags = get_move_flags(move);
      
   //    std::cout << "Mutarea " << i+1 << ": " 
   //              << square_to_coordinates[get_move_source_square(move)] << "->"
   //              << square_to_coordinates[get_move_target_square(move)] 
   //              << " | Captura: " << (is_move_capture(move) ? "Da" : "Nu")
   //              << " | Promovare: " << (is_move_promotion(move) ? "Da" : "Nu")
   //              << " | EP: " << (is_move_enpassant(move) ? "Da" : "Nu")
   //              << " | DoublePush: " << (is_move_double_pawn_push(move) ? "Da" : "Nu")
   //              << " | Rocada(K/Q): " << is_move_king_castle(move) << "/" << is_move_queen_castle(move)
   //              << " | Flags: " << flags << '\n';
   // }
} 
