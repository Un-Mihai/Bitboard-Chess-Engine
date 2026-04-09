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

int main() {
   printBitboard(white_pawns);
   setBit(main_bitboard, Squares::e4);
   printBitboard(main_bitboard);
   functieExemplu();
} 
