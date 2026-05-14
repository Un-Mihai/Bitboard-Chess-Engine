#include <chrono>
#include <assert.h>

#include "tests/MoveGenTests.hpp"
#include "core/Logger.hpp"
#include "movegen/MoveGen.hpp"
#include "movegen/MoveHandling.hpp"

long long count_moves(int depth) {
    if (depth == 0) return 1ULL;

    MoveList move_list;
    generate_moves(side, move_list);

    // La ultimul nivel doar numaram, nu mai facem recursivitate
    if (depth == 1) {
        long long count = 0;
        for (int i = 0; i < move_list.count; i++) {
            if (make_move(move_list.moves[i])) {
                count++;
                unmake_move(move_list.moves[i]);
            }
        }
        return count;
    }

    long long nodes = 0;

    for (int i = 0; i < move_list.count; i++) {
        if (make_move(move_list.moves[i])) {
            nodes += count_moves(depth - 1);
            unmake_move(move_list.moves[i]);
        }
    }

    return nodes;
}

void generate(const char* fen_position, int depth, long long& nodes, double& time){
    parse_fen_string(fen_position);
    auto start = std::chrono::high_resolution_clock::now();
    nodes = count_moves(depth);
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durata = stop - start;
    time = durata.count(); // Salveaza timpul ca numar (secunde)
}

void test_move_generation_fast() {
    long long nodes;
    double time;
    std::string msg;

    // starting position
    generate(fen_start_position_board, 6, nodes, time);
    if (nodes != 119060324) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    // kiwipete position
    generate(fen_kiwipete, 5, nodes, time);
    if (nodes != 193690690) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    // other positions
    generate(fen_test_1, 5, nodes, time);
    if (nodes != 15833292) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    generate(fen_test_2, 7, nodes, time);
    if (nodes != 178633661) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());
}

void test_move_generation_complex() {
    long long nodes;
    double time;
    std::string msg;

    // starting position
    generate(fen_start_position_board, 7, nodes, time);
    if (nodes != 3195901860) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    // kiwipete position
    generate(fen_kiwipete, 6, nodes, time);
    if (nodes != 8031647685) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    // other positions
    generate(fen_test_1, 6, nodes, time);
    if (nodes != 706045033) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());

    generate(fen_test_2, 8, nodes, time);
    if (nodes != 3009794393) {
        log(ERROR, "Move generation test failed.");
        return;
    }
    msg = std::to_string(nodes) + " nodes generated. Time: " +
                      std::to_string(time) + " seconds";
    log(INFO, msg.c_str());
}