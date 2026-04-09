#pragma once

#include <cstdint>

#include "Types.hpp"

inline const bool getBit(U64 bitboard, int square){
   return (bitboard & (1ULL << square));
}

inline void setBit(U64 &bitboard, int square) {
   bitboard |= (1ULL << square);
}

inline void clearBit(U64 &bitboard, int square) {
   getBit(bitboard, square) ? bitboard ^= (1ULL << square) : 0;
}