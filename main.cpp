#include <iostream>
#include <cassert>
#include <unordered_map>
using namespace std;

const int N = 5;
const double INF = 1e9;
const bool BOT_TURN = 0;

// inline section
inline bool turn(int board) {
	return __builtin_popcount(board) & 1;
}

inline bool bit(int n, int i) {
	return (n >> i) & 1;
}

inline int get_ind(int i, int j) {
	return i * N + j;
}

inline bool get_val(int board, int i, int j) {
	return bit(board, get_ind(i, j));
}

inline double get_coef(int board) {
	return turn(board) == BOT_TURN ? 1 : -1;
}

inline double f(int board, int dep) {
	return get_coef(board) * (1 - 0.01 * dep);
}

inline void set_val(int &board, int i, int j) {
	board ^= (1 << get_ind(i, j));
}
// inline section

// endgame check
bool check_row(int board, int i) {
	for (int j = 0; j < N; ++j)
		if (!get_val(board, i, j)) return false;
	return true;
}

bool check_col(int board, int j) {
	for (int i = 0; i < N; ++i)
		if (!get_val(board, i, j)) return false;
	return true;
}

bool check_diag1(int board) {
	for (int i = 0; i < N; ++i)
		if (!get_val(board, i, i)) return false;
	return true;
}

bool check_diag2(int board) {
	for (int i = 0; i < N; ++i)
		if (!get_val(board, i, N - i - 1)) return false;
	return true;
}

bool is_end(int board) {
	for (int i = 0; i < N; ++i) {
		if (check_row(board, i)) return true;
		if (check_col(board, i)) return true;
	}
	if (check_diag1(board)) return true;
	if (check_diag2(board)) return true;
	return false;
}
// endgame check

// go section
double minimax(int board, int dep, int &nodes, double al, double be, unordered_map<int, double> &Cash, int &res_i, int &res_j) {
	auto it = Cash.find(board);
	if (it != Cash.end()) return (*it).second;
	++nodes;
	if (is_end(board)) return Cash[board] = f(board, dep);
	bool is_min = turn(board) != BOT_TURN;
	double cp = is_min ? INF : -INF;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j) {
			if (get_val(board, i, j)) continue;
			set_val(board, i, j);
			auto tmp = minimax(board, dep + 1, nodes, al, be, Cash, res_i, res_j);
			if (!dep && tmp > cp) res_i = i, res_j = j;
			if (is_min) {
				cp = min(cp, tmp);
				be = min(be, tmp); // tmp = cp
			}
			else {
				cp = max(cp, tmp);
				al = max(al, tmp); // tmp = cp
			}
			set_val(board, i, j);
			if (be < al) return Cash[board] = cp;
		}
	return Cash[board] = cp;
}

void go_bot(int board, int &i, int &j) {
	int nodes = 0;
	unordered_map<int, double> Cash;
	double al = -INF, be = INF;
	auto cp = minimax(board, 0, nodes, al, be, Cash, i, j);
	cout << "cp: " << cp << ", nodes: " << nodes << endl;
}

void go_human(int board, int &i, int &j) {
	cin >> i >> j, --i, --j;
}
// go section

// ui section
void print_board(int board) {
	cout << endl;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j)
			cout << get_val(board, i, j) << ' ';
		cout << endl;
	}
	cout << endl;
}

void print_winner(int board) {
	bool winner = turn(board);
	if (winner == BOT_TURN) cout << "AI is the winner" << endl;
	else cout << "You are the winner" << endl;
}

void terminal() {
	int board = 0;
	print_board(board);
	while (!is_end(board)) {
		int i, j;
		if (turn(board) == BOT_TURN) go_bot(board, i, j);
		else go_human(board, i, j);
		assert(!get_val(board, i, j));
		set_val(board, i, j);
		print_board(board);
	}
	print_winner(board);
}
// ui section

signed main() {
	terminal();
}