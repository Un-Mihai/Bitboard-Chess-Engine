#pragma once

#include <cstdint>

#include "Types.hpp"
#include "BitManipulation.hpp"

void functieExemplu();

//mask the bits a knight can go from a given square (side doesn't matter)
uint64_t generate_knight_attacks (int square);