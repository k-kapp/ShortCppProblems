/*
Small program to convert decimal to hexadecimal
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

double log16(long num)
{
	return log2(num)/4;
}

void get_hex_idxes(long num, vector<pair<int, int> > &idxes)
{
	if (num == 0)
		return;

	int idx = log16(num);
	long filled = pow(16, idx);
	int num_bases = (int)(num / filled);
	long rem = num - filled * num_bases;

	idxes.push_back({idx, num_bases});

	get_hex_idxes(rem, idxes);
}

string get_string(vector<pair<int, int> > &idxes)
{
	string str_rep(idxes.front().first + 1, '0');

	for (auto &idx_pair : idxes)
	{
		char num_rep;
		if ((idx_pair.second >= 10) && (idx_pair.second <= 15))
			num_rep = (char)(idx_pair.second + 55);
		else if (idx_pair.second < 10)
			num_rep = (char)(idx_pair.second + '0');
		else
		{
			cout << "Fatal error" << endl;
			return "";
		}
		str_rep.at(idx_pair.first) = num_rep;
	}

	reverse(str_rep.begin(), str_rep.end());

	return str_rep;
}

string get_hex(long num)
{
	vector<pair<int, int> > idxes;

	get_hex_idxes(num, idxes);
	return get_string(idxes);
}

int main(int argc, char * argv [])
{
	if (argc != 2)
	{
		cout << "usage: ./get_hex <number>" << endl;
		return EXIT_FAILURE;
	}

	cout << get_hex(atol(argv[1])) << endl;

	return EXIT_SUCCESS;
}
