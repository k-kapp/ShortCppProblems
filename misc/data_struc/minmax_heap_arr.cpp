#include <iostream>
#include <vector>
#include <functional>
#include <utility>
#include <memory>

using namespace std;

class heap
{
public:
	heap(vector<int> values, bool max)
	{
		if (max)
		{
			comp = greater<int>();
		}
		else
		{
			comp = less<int>();
		}
		for (auto &val : values)
		{
			insert(val);
		}
	}

	void insert(int val)
	{
		heap_arr.push_back(val);
		int curr_idx = heap_arr.size() - 1;
		int parent_idx = calc_parent_idx(curr_idx);

		while ((curr_idx > 0) && !comp(heap_arr.at(parent_idx), heap_arr.at(curr_idx)))
		{
			swap(heap_arr.at(parent_idx), heap_arr.at(curr_idx));
			curr_idx = parent_idx;
			parent_idx = calc_parent_idx(curr_idx);
		}
	}

	shared_ptr<int> pop()
	{
		if (!(*this))
		{
			return nullptr;
		}

		auto return_val = make_shared<int>(heap_arr.front());
		heap_arr.front() = heap_arr.back();
		heap_arr.pop_back();
		
		int curr_idx = 0;
		auto children_idxes = calc_children_idx(curr_idx);

		while (true)
		{
			int best_child_idx = -1;
			if (children_idxes.first < heap_arr.size())
			{
				best_child_idx = children_idxes.first;
			}
			if (children_idxes.second < heap_arr.size())
			{
				if (comp(heap_arr.at(children_idxes.second), heap_arr.at(children_idxes.first)))
				{
					best_child_idx = children_idxes.second;
				}
			}
			if (best_child_idx == -1)
				break;
			else
			{
				if (!comp(heap_arr.at(curr_idx), heap_arr.at(best_child_idx)))
				{
					swap(heap_arr.at(curr_idx), heap_arr.at(best_child_idx));
					curr_idx = best_child_idx;
				}
				else
				{
					break;
				}
			}
			children_idxes = calc_children_idx(curr_idx);
		}
		return return_val;
	}

	operator bool() const
	{
		return heap_arr.size() > 0;
	}

private:
	int calc_parent_idx(int curr_idx)
	{
		if (curr_idx % 2 == 1)
		{
			return (curr_idx + 1)/2 - 1;
		}
		else
		{
			return curr_idx/2 - 1;
		}
	}

	pair<int, int> calc_children_idx(int curr_idx)
	{
		return {2*(curr_idx + 1) - 1, 2*(curr_idx + 1)};
	}

	vector<int> heap_arr;
	function<bool(int, int)> comp;
};

int main(int argc, char * argv [])
{
	heap heap_obj({4, 8, 9, 2, 1, 6, -5, 1, 6, 2, 0, -10, 1, 2, 2, -1}, false);

	while (heap_obj)
	{
		auto lowest = heap_obj.pop();
		cout << "lowest: " << *lowest << endl;
	}

	return 0;
}
