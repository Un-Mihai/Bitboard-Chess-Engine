#include<iostream>

#include "Types.hpp" //fisier in care definesc enum-urile si tipulde date U64 (e mai scurt) 
#include "BitManipulation.hpp" //fisier in care tinem functiile de manipulare a bitilor
#include "Attacks.hpp" //header in care tinem functiile de generare a atacurilor
#include "Board.hpp" //header in care tinem bitboardurile si functiile generale (de ex avem print ul acum)

/*
   In Types.hpp am incapsulat enum urile in namespaceuri pentru ca sa nu fie ceva probleme mai incolo
   Ce-i drept pozitiile a1, a2... nu ne incurca ca o nu folosim numele astea, dar cel putin pentru piese
   ma gandeam ca e bine sa le avem in namespace ca na sunt litere normale si sa se inteleaga in cod ce e cu litera aia cand o folosim.

   Poti sa modifici ce vrei daca ai ale idei :))
*/

/*
   In fisierele .hpp variablele le am declarat inline ca sa nu fie probleme de linking intre fisiere dupa
   Cam tot ce definim direct in .hpp trebuie sa fie inline
   Daca doar declaram, si apoi definim in .cpp, nu trebuie sa fie inline (la variabile trebuie specificat extern in .hpp, si apoi in .cpp sa fie definita fara extern)
   Penru Types si pentru BitManipulation nu am mai facut fisiere .cpp pentru ca nu au mult continut
   De asemenea "inline" la functii sugereaza compilatorului sa le insereze direct acolo unde sunt apelate,
   dar e doar o sugestie, oricum stie el cum e mai bine si face cum vrea
*/

int main() {
   
   init_pawn_attacks();
   init_knight_attacks();
   init_king_attacks();

   U64 block = empty_bitboard;
   setBit(block, Squares::e6);
   printBitboard(generate_rook_attacks(Squares::e4, block));

   // printBitboard(white_pawns);
   // setBit(main_bitboard, Squares::e4);
   // printBitboard(main_bitboard);
   // functieExemplu();


   //----- TEMPORARY CODE(might be reused) ----------

   // for (int i = 0; i < 64; i ++)
   //    printBitboard(knight_attacks[i]);

   // uint64_t b = empty_bitboard;
   // for(int rank = 0; rank < 8; rank ++){
   //    for (int file = 0; file < 6; file ++){
   //       setBit(b, rank * 8 + file);
   //    }
   // }
   // printBitboard(b);
   
   // std :: cout << first_lsb(b);

   // setBit(b, Squares::c3);
   // setBit(b, Squares::b6);
   // setBit(b, Squares::g7);

   // printBitboard(generate_bishop_attacks(Squares::d4, b));
   //printBitboard(generate_bishop_blocks(Squares::d1));

   // printBitboard( 
   //    generate_one_occupancy(
   //       generate_bishop_blocks(Squares::d4), 509
   //    )
   // );

   // for (int square = 0; square < 64; square ++){
   //    U64 occupancy = generate_rook_blocks(square);
   //    std :: cout << bitCount (occupancy ) << ", ";
   //    if (square % 8 == 7)
   //       std :: cout << '\n';
   // }
   
} 
