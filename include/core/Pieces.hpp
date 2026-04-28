#pragma once

#include "Types.hpp"

//unicode pieces
inline const char* unicode_pieces[12] = {
    "♟", "♞", "♝", "♜", "♛", "♚",
    "♙", "♘", "♗", "♖", "♕", "♔"
};

//ascii_pieces
inline const char ascii_pieces[13] = "PNBRQKpnbrqk";

//convering ascii_pieces to unicode pieces
inline int parse_piece_char_to_int(char c) {
    switch (c) {
        case 'P': return Pieces::P;
        case 'N': return Pieces::N;
        case 'B': return Pieces::B;
        case 'R': return Pieces::R;
        case 'Q': return Pieces::Q;
        case 'K': return Pieces::K;
        case 'p': return Pieces::p;
        case 'n': return Pieces::n;
        case 'b': return Pieces::b;
        case 'r': return Pieces::r;
        case 'q': return Pieces::q;
        case 'k': return Pieces::k;
        default:  return -1;
    }
}

/*-----------BITBOARDS FOR ALL KINDS OF PIECES----------*/

//starting positions
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