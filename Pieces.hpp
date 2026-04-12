#pragma once

#include "Types.hpp"

namespace Pieces {
   enum {
      P, N, B, R, Q, K, p, n, b, r, q, k
   };
}

//unicode pieces
char *unicode_pieces[12] = {
    "♟", "♞", "♝", "♜", "♛", "♚",
    "♙", "♘", "♗", "♖", "♕", "♔"
};


inline U64 black_pawns = 65280ULL;
inline U64 white_pawns = 71776119061217280ULL;

inline U64 black_king = 16ULL;
inline U64 white_king = 1152921504606846976ULL;

inline U64 black_queen = 8ULL;
inline U64 white_queen = 576460752303423488ULL;

inline U64 black_bishops = 36ULL;
inline U64 white_bishops = 2594073385365405696ULL;

inline U64 black_knights = 66ULL;
inline U64 white_knights = 4755801206503243776ULL;

inline U64 black_rooks = 129ULL;
inline U64 white_rooks = 9295429630892703744ULL;