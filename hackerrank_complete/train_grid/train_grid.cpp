#include <map>
#include <list>
#include <iostream>
#include <utility>

using namespace std;

void insert_ordered(list<pair<long, long> > &lst, pair<long, long> new_pair)
{
	auto pair_iter = lst.begin();

	while (pair_iter != lst.end())
	{
		if (new_pair.first >= pair_iter->first)
		{
			lst.insert(next(pair_iter, 1), new_pair);
			return;
		}
	}
	lst.push_back(new_pair);
}

void modify_list(list<pair<long, long> > &lst, long start_col, long end_col)
{
	bool no_change = false;
	bool no_overlap = true;
	auto pair_iter = lst.begin();
	for (; pair_iter != lst.end(); advance(pair_iter, 1))
	{
		if ((pair_iter->first <= start_col) && (pair_iter->second >= start_col))
		{
			no_overlap = false;
			if (end_col > pair_iter->second)
			{
				start_col = pair_iter->first;
				//pair_iter->second = end_col;
			}
			else
				no_change = true;
			break;
		}
		else if((pair_iter->first <= end_col) && (pair_iter->second >= end_col))
		{
			no_overlap = false;
			if (start_col < pair_iter->first)
			{
				end_col = pair_iter->second;
				//pair_iter->first = start_col;
			}
			else
				no_change = true;
			break;
		}
		if (start_col > pair_iter->second)
		{
			break;
		}
	}
	if (no_overlap)
	{
		insert_ordered(lst, {start_col, end_col});
	}
	else if (!no_change)
	{
		lst.erase(pair_iter);
		modify_list(lst, start_col, end_col);
	}
}

long count_filled_row(const list<pair<long, long> > &lst)
{
	long count = 0;
	for (const auto &pair_vals : lst)
	{
		count += (pair_vals.second - pair_vals.first + 1);
	}
	return count;
}

long count_filled_grid(const map<long, list<pair<long, long> > > &grid_map)
{
	long count = 0;
	for (const auto &lst_pair : grid_map)
	{
		count += count_filled_row(lst_pair.second);
	}
	return count;
}

void print_list(const list<pair<long, long> > &lst)
{
	for (const auto &pair : lst)
	{
		cout << "(" << pair.first << ", " << pair.second << ")" << " ";
	}
	cout << endl;
}

int main(int argc, char * argv [])
{
	map<long, list<pair<long, long> > > grid_map;
	
	long rows, cols, tracks;
	cin >> rows;
	cin >> cols;
	cin >> tracks;

	
	for (int i = 0; i < tracks; i++)
	{
		long row, start_col, end_col;
		cin >> row;
		cin >> start_col;
		cin >> end_col;
		auto iter = grid_map.find(row);
		if (iter != grid_map.end())
		{
			modify_list(iter->second, start_col, end_col);
		}
		else
		{
			grid_map[row] = list<pair<long, long> >({{start_col, end_col}});
		}
	}


	cout << rows*cols - count_filled_grid(grid_map) << endl;

	return 0;
}

