#include<iostream>

#include "movegen/MagicNumbers.hpp"
#include "movegen/Attacks.hpp"

U32 get_random_U32_number() {
    random_state ^= random_state << 13;
    random_state ^= random_state >> 17;
    random_state ^= random_state << 5;

    return random_state;
}

U64 get_random_U64_number() {
    U64 n1, n2, n3, n4; 

    //extract only the last 16 bits from each random number
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    //shift and combine the numbers to form the 64-bit number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 get_magic_number_candidate() {
    //biwise & 3 numbers to get a number with very few bits set to 1 (sparse number).
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

U64 find_magic_number(int square, int piece) {
    
    //the number of relevant bits for the current square and piece
    int bit_count = piece ? bishop_occupancy_mask_count[square] : rook_occupancy_mask_count[square];

    //allocate the maximum needed 2^12 = 4096 combinations for a rook on a corner
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];

    //the mask with all squares that could block the piece moves
    U64 blocks = piece ? generate_bishop_blocks(square) : generate_rook_blocks(square);

    //the total number of possible blocking combinations
    int occupancy_indices = 1 << bit_count;

    //pre-calculate all blocking combinations and their actual attacks
    for(int index = 0; index < occupancy_indices; index++) {
        occupancies[index] = generate_one_occupancy(blocks, index);
        attacks[index] = piece ? generate_bishop_attacks(square, occupancies[index]) : generate_rook_attacks(square, occupancies[index]);
    }

    //try to find the magic number (limit of 10 million attempts)
    for(int random_count = 0; random_count < 10000000; random_count++) {
        
        U64 magic_number = get_magic_number_candidate();

        //filter out the numbers that don't have enough bits in the upper part (may lower the threshold if it fails for bishops)
        if(bitCount((magic_number * blocks) & 0xFF00000000000000) < 6) continue;

        //clear the used attacks array for the magic number new candidate
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        int fail = 0;
        
        //test if the candidate generates collisions
        for(int index = 0; index < occupancy_indices; index++) {
            
            //the magic formula: multiply the occupancy by the magic number and shift right
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - bit_count));

            //check if the index is free (if it is save the attack there)
            if(used_attacks[magic_index] == 0ULL) {
                used_attacks[magic_index] = attacks[index];
            }
            //if the index is occupied but gives the same attack, it is ok
            //if it gives a different attack, the magic number is invalid ((collision)
            else if(used_attacks[magic_index] != attacks[index]) {
                fail = 1;
                break;
            }
        }

        //if there was no destructive collision the magic number is valid
        if(!fail) {
            return magic_number;
        }
    }
    
    std::cout << "fail\n";
    return 0;
}