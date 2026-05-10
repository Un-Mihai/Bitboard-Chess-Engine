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
   bitboard &= ~(1ULL << square);
}

// works only if source bit = 1 and target bit = 0
inline void moveBit(U64& bitboard, int source_square, int target_square){
   bitboard ^= (1ULL << source_square) | (1ULL << target_square);
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

// returns the index of the first least significant bit equal to 1 and clears the bit at the same time 
//(faster than first_lsb + clearBit)
inline int pop_lsb(U64 &bitboard) {
    int index = __builtin_ctzll(bitboard);
    bitboard &= (bitboard - 1);
    return index;
}