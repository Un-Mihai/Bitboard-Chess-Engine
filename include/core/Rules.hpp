#pragma once

#include "Types.hpp"

//side to move
int side;

// the one and only square that can capture enpassant in current position
int enpassant = Squares :: no_square;

/*----------- castling rights ----------------

0001 - white king can castle to the king side
0010 - white king can castle to the queen side
0100 - black king can castle to the king side
1000 - black king can castle to the queen side

ex: 1010 means both kings can castle queen side only

--------------------------------------------*/
int castle;

enum{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8
};

