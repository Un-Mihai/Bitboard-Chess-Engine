#include<iostream>
#include<cstdint>

uint64_t main_bitboard = 0ULL;
uint64_t black_pawns = 65280ULL;
uint64_t white_pawns = 71776119061217280ULL;

uint64_t black_king = 16ULL;
uint64_t white_king = 1152921504606846976ULL;

uint64_t black_queen = 8ULL;
uint64_t white_queen = 576460752303423488ULL;

uint64_t black_bishops = 36ULL;
uint64_t white_bishops = 2594073385365405696ULL;

uint64_t black_knights = 66ULL;
uint64_t white_knights = 4755801206503243776ULL;

uint64_t black_rooks = 129ULL;
uint64_t white_rooks = 9295429630892703744ULL;

enum squares {
   a8, b8, c8, d8, e8, f8, g8, h8,
   a7, b7, c7, d7, e7, f7, g7, h7,
   a6, b6, c6, d6, e6, f6, g6, h6,
   a5, b5, c5, d5, e5, f5, g5, h5,
   a4, b4, c4, d4, e4, f4, g4, h4,
   a3, b3, c3, d3, e3, f3, g3, h3,
   a2, b2, c2, d2, e2, f2, g2, h2,
   a1, b1, c1, d1, e1 ,f1 ,g1 ,h1
};

inline const bool getBit(uint64_t bitboard, int square){
   return (bitboard & (1ULL << square));
}

inline void setBit(uint64_t &bitboard, int square) {
   bitboard |= (1ULL << square);
}

inline void clearBit(uint64_t &bitboard, int square) {
   getBit(bitboard, square) ? bitboard ^= (1ULL << square) : 0;
}

void printBitboard(uint64_t bitboard) {
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

int main() {
    std::cout << "Hello World!" << std::endl;
    return 0;
} 
