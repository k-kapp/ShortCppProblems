#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <utility>

/*
 * Maximum subarray sum, problem, Hackerrank
*/

using namespace std;

using ullong = unsigned long long;

vector<pair<ullong, ullong> > * make_prefix_sums(const vector<ullong> &data, ullong mod)
{
	vector<pair<ullong, ullong> > * prefix_sums = new vector<pair<ullong, ullong> >(data.size());

	auto list_iter = prefix_sums->begin();

	for (int idx = 0; idx < data.size(); idx++)
	{
		if (idx > 0)
			*list_iter = {idx, (next(list_iter, -1)->second + data[idx]) % mod};
		else
			*list_iter = {idx, data[idx] % mod};
		advance(list_iter, 1);
	}

	return prefix_sums;
}

bool sort_func(const pair<ullong, ullong> &fst, const pair<ullong, ullong> &snd)
{
	if (fst.second > snd.second)
	{
		return false;
	}
	else if (fst.second == snd.second)
	{
		return fst.first > snd.first;
	}
	else
		return true;
}

void eliminate_dupl(list<pair<ullong, ullong> > &sorted)
{
	auto before_end = next(sorted.end(), -1);
	for (auto list_iter = sorted.begin(); (list_iter != before_end) && (list_iter != sorted.end()); advance(list_iter, 1))
	{
		auto next_list_iter = next(list_iter, 1);
		if (next_list_iter->second == list_iter->second)
		{
			next_list_iter = sorted.erase(next(list_iter, 1));
		}
	}
}


ullong highest_val(const vector<pair<ullong, ullong> > &sorted, ullong mod)
{
	int max_sum, curr_sum;
	max_sum = -1;
	//cout << "getting highest val..." << endl;
	auto next_start = sorted.begin();
	auto iter = sorted.begin();
	auto next_comp = sorted.begin();

	int first_adv_c = 0;
	int second_adv_c = 0;
	int first_if_adv_c = 0;
	int second_if_adv_c = 0;

	while (iter != sorted.end())
	{
		iter = next_start;
		auto curr_iter = next(iter, 1);
		while ((curr_iter != sorted.end()) && (curr_iter->second == iter->second))
		{
			advance(curr_iter, 1);
			first_adv_c++;
		}
		next_start = curr_iter;
		if (distance(curr_iter, next_comp) > 0)
		{
			curr_iter = next_comp;
			cout << "assigning curr_iter to next_comp" << endl;
		}
		cout << "distance: " << distance(curr_iter, next_comp) << endl;
		while ((curr_iter != sorted.end()) && (curr_iter->first > iter->first))
		{
			advance(curr_iter, 1);
			second_adv_c++;
		}
		if (next_start->first < iter->first)
		{
			next_comp = curr_iter;
		}
		if (curr_iter != sorted.end())
		{
			curr_sum = mod + iter->second - curr_iter->second;
			if (curr_sum > max_sum)
				max_sum = curr_sum;
			first_if_adv_c++;
		}
		if (iter->second > max_sum)
		{
			max_sum = iter->second;
			second_if_adv_c++;
		}
	}
	//cout << "returning..." << endl;

	cout << "first adv: " << first_adv_c << ", second adv: " << second_adv_c << ", first if: " << first_if_adv_c << ", second if: " << second_if_adv_c << endl;

	return max_sum;
}

int main(int argc, char * argv [])
{
	long num_queries, arr_len, mod;

	cin >> num_queries;

	vector<ullong> data;

	ullong temp_in;

	list<ullong> answers;

	for (long i = 0; i < num_queries; i++)
	{
		cin >> arr_len;
		cin >> mod;

		data = vector<ullong>(arr_len);

		for (long j = 0; j < arr_len; j++)
		{
			cin >> data[j];
		}

		vector<pair<ullong, ullong> > * pref_ptr = make_prefix_sums(data, mod);
		vector<pair<ullong, ullong> > &pref_sums = *pref_ptr;

		sort(pref_sums.begin(), pref_sums.end(), sort_func);
		//pref_sums.sort(sort_func);

		answers.push_back(highest_val(pref_sums, mod));
		cout << "done with query " << i << endl;
		delete pref_ptr;
	}
	for (auto &ans : answers)
		cout << ans << endl;

	return 0;
}
