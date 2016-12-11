#include <iostream>
#include <vector>
#include <climits>
#include <stdexcept>

using namespace std;

int do_checking(vector<int> &count_vec, const vector<int> &orig, int succ_count)
{
	int curr_pos = 0;

	int highest_count = succ_count;
	int best_pos = 0;

	int curr_count;

	for (int i = 0; i < orig.size(); i++)
	{
		succ_count -= count_vec[i];
		if (orig[i] == orig.size())
		{
			continue;
		}
		
		int margin = orig.size() - 1 - orig[i];

		/*
		 * try block is for some debugging I did earlier
		 */
		try
		{
			count_vec.at(i + margin + 1) += 1;
		}
		catch (out_of_range &e)
		{
			cout << "margin: " << margin << endl;
			throw;
		}

		succ_count += 1;

		if (succ_count > highest_count)
		{
			succ_count = highest_count;
			best_pos = i + 1;
		}

	}

	return best_pos;
}


pair<vector<int>, int> get_info(const vector<int> &orig)
{

	vector<int> count_vec(2*orig.size(), 0);

	int succ_count = 0;

	for (int i = 0; i < orig.size(); i++)
	{
		int result = i - orig[i];
		if (result >= 0)
		{
			succ_count++;
			count_vec[result] += 1;
		}
	}

	return {count_vec, succ_count};
}


int main(int argc, char * argv [])
{
	int n;

	cin >> n;

	vector<int> data(n);

	for (int i = 0; i < n; i++)
	{
		cin >> data[i];
	}

	auto info = get_info(data);

	cout << do_checking(info.first, data, info.second) + 1 << endl;

	return 0;
}
