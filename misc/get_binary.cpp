/*
Small program that converts decimal numbers to binary
*/


#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void get_idx(long num, vector<int> &bit_idxes)
{
	if (num == 0)
	{
		return;
	}
	else
	{
		int bit_idx = (int)log2(num);
		bit_idxes.push_back(bit_idx);
		get_idx(num - pow(2, bit_idx), bit_idxes);
	}
}

string get_bits(vector<int> bit_idxes)
{
	string bits(bit_idxes.front() + 1, '0');

	for (auto &bit_idx : bit_idxes)
	{
		bits.at(bit_idx) = '1';
	}

	reverse(bits.begin(), bits.end());

	return bits;
}

int main(int argc, char * argv [])
{
	if (argc != 2)
	{
		cout << "usage: ./get_bin <integer>" << endl;
		return EXIT_FAILURE;
	}
	vector<int> bit_idxes;
	get_idx(atol(argv[1]), bit_idxes);
	cout << get_bits(bit_idxes) << endl;

	return EXIT_SUCCESS;
}

