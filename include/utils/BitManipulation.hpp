#pragma once

#include <cstdint>

#include "core/Types.hpp"

inline const bool getBit(U64 bitboard, int square){
   return (bitboard & (1ULL << square));
}

inline void setBit(U64 &bitboard, int square) {
   bitboard |= (1ULL << square);
}

inline void clearBit(U64 &bitboard, int square) {
   getBit(bitboard, square) ? bitboard ^= (1ULL << square) : 0;
}

/*
Description: Counts the bits in a bitboard
Obs:  __builtin_popcountll forces the compiler to use POPCNT CPU instruction (fast method)
*/
inline int bitCount(U64 bitboard){
   return __builtin_popcountll(bitboard);
}

/*
Description: Returns the index of the first least significant bit equal to 1
Important! : Unknown behavior for bitboard = 0
*/
inline int first_lsb(U64 bitboard){
   return __builtin_ctzll(bitboard);
}