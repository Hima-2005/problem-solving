#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

void printBoard(const vector<vector<int>>& b) {
	for (int r = 0; r < 9; ++r) {
		for (int c = 0; c < 9; ++c) {
			int v = b[r][c];
			if (v == 0) cout << '.';
			else cout << v;
			if ((c + 1) % 3 == 0 && c != 8) cout << "  "; else cout << ' ';
		}
		cout << '\n';
		if ((r + 1) % 3 == 0 && r != 8) cout << '\n';
	}
}

vector<int> candidates(const vector<vector<int>>& b, int r, int c) {
	bool used[10] = {};
	for (int i = 0; i < 9; ++i) {
		int v = b[r][i]; if (v) used[v] = true;
		v = b[i][c]; if (v) used[v] = true;
	}
	int br = (r / 3) * 3, bc = (c / 3) * 3;
	for (int i = br; i < br + 3; ++i)
		for (int j = bc; j < bc + 3; ++j) {
			int v = b[i][j]; if (v) used[v] = true;
		}
	vector<int> opts;
	for (int n = 1; n <= 9; ++n) if (!used[n]) opts.push_back(n);
	return opts;
}

struct CellChoice { int r = -1, c = -1; vector<int> opts; };
enum class FindResult { Solved, DeadEnd, Found };

pair<FindResult, CellChoice> findBestCell(const vector<vector<int>>& b) {
	CellChoice best;
	bool found = false;
	int bestSize = 10;
	for (int r = 0; r < 9; ++r) {
		for (int c = 0; c < 9; ++c) {
			if (b[r][c] != 0) continue;
			auto opts = candidates(b, r, c);
			if (opts.empty()) return {FindResult::DeadEnd, {}};
			if (!found || (int)opts.size() < bestSize) {
				found = true;
				best.r = r; best.c = c; best.opts = move(opts);
				bestSize = (int)best.opts.size();
				if (bestSize == 1) return {FindResult::Found, best};
			}
		}
	}
	if (!found) return {FindResult::Solved, {}};
	return {FindResult::Found, best};
}

bool solve(vector<vector<int>>& b) {
	auto pr = findBestCell(b);
	if (pr.first == FindResult::Solved) return true;
	if (pr.first == FindResult::DeadEnd) return false;
	CellChoice choice = move(pr.second);
	for (int v : choice.opts) {
		b[choice.r][choice.c] = v;
		if (solve(b)) return true;
		b[choice.r][choice.c] = 0;
	}
	return false;
}

int main() {
	// 0 means empty
	vector<vector<int>> puzzle = {
		{5,3,0, 0,7,0, 0,0,0},
		{6,0,0, 1,9,5, 0,0,0},
		{0,9,8, 0,0,0, 0,6,0},

		{8,0,0, 0,6,0, 0,0,3},
		{4,0,0, 8,0,3, 0,0,1},
		{7,0,0, 0,2,0, 0,0,6},

		{0,6,0, 0,0,0, 2,8,0},
		{0,0,0, 4,1,9, 0,0,5},
		{0,0,0, 0,8,0, 0,7,9},
	};

	cout << "Puzzle:\n";
	printBoard(puzzle);
	if (solve(puzzle)) {
		cout << "\nSolution:\n";
		printBoard(puzzle);
	} else {
		cout << "\nNo solution found.\n";
	}
	return 0;
}
