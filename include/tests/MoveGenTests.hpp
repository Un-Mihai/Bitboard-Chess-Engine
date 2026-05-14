long long count_moves(int depth);

void generate(const char* fen_position, int depth, long long& nodes, double& time);

void test_move_generation_fast();

void test_move_generation_complex();