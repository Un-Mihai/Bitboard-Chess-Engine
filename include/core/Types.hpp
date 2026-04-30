#pragma once

#include <cstdint>

typedef uint64_t U64;
typedef uint32_t U32;

namespace Squares{
   enum {
      a8, b8, c8, d8, e8, f8, g8, h8,
      a7, b7, c7, d7, e7, f7, g7, h7,
      a6, b6, c6, d6, e6, f6, g6, h6,
      a5, b5, c5, d5, e5, f5, g5, h5,
      a4, b4, c4, d4, e4, f4, g4, h4,
      a3, b3, c3, d3, e3, f3, g3, h3,
      a2, b2, c2, d2, e2, f2, g2 ,h2,
      a1 ,b1 ,c1 ,d1 ,e1 ,f1 ,g1 ,h1,
      no_square
   };
}

namespace Pieces {
   enum {
      P, N, B, R, Q, K, p, n, b, r, q, k
   };
}

enum{
   rook, bishop
};

enum {
   white, black, both
};

struct MoveList{
   U32 moves[256];      //there are maximum 218 possible moves but we choose 256 (2^8) to allow better low level optimisations
   int count = 0;

   //helper function to add a move to the list
   inline void add_move(U32 move){
      moves[count++] = move;
   }
};