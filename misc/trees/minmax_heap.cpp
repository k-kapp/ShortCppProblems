#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <cmath>

using namespace std;

struct node
{
	node (int val, shared_ptr<node> left, shared_ptr<node> right, shared_ptr<node> parent)
			: val(val), left(left), right(right), parent(parent)
	{}

	node (int val, shared_ptr<node> left, shared_ptr<node> right, weak_ptr<node> parent)
			: val(val), left(left), right(right), parent(parent)
	{}

	int val;
	shared_ptr<node> left;
	shared_ptr<node> right;
	weak_ptr<node> parent;
};

class heap
{
public:
	heap(vector<int> vals, bool max)
	{
		root = shared_ptr<node>(nullptr);
		row_count = 0;

		if (max)
			comp = greater<int>();
		else
			comp = less<int>();
	
		for (auto &val : vals)
		{
			insert(val);
		}
	}

	void insert(int val)
	{
		if (!root)
		{
			root = make_shared<node>(val, nullptr, nullptr, nullptr);
			history.push_back({weak_ptr<node>(root)});
			return;
		}

		int required_bottom = pow(2, row_count);
		
		if (history.back().size() == required_bottom)
		{
			history.push_back(vector<weak_ptr<node> >());
			row_count++;
		}

		shared_ptr<node> new_node;
		weak_ptr<node> parent_node;

		int idx = history.back().size() % 2 == 0 ? history.back().size()/2 : (history.back().size() - 1)/2;
		parent_node = weak_ptr<node>(history.at(history.size() - 2).at(idx));
		new_node = make_shared<node>(val, nullptr, nullptr, parent_node);

		if (!parent_node.lock()->left)
		{
			parent_node.lock()->left = new_node;
		}
		else
		{
			parent_node.lock()->right = new_node;
		}
		history.back().push_back(weak_ptr<node>(new_node));

		bubble_up(new_node);

	}

	void bubble_up(weak_ptr<node> addition)
	{
		if (addition.lock()->parent.lock() && 
				comp(addition.lock()->val, addition.lock()->parent.lock()->val))
		{
			swap(addition.lock()->val, addition.lock()->parent.lock()->val);
			bubble_up(addition.lock()->parent);
		}
	}

	shared_ptr<int> pop()
	{
		if (!root)
		{
			return nullptr;
		}

		auto return_val = make_shared<int>(root->val);
		if (!(root->left || root->right))
		{
			root.reset();
			return return_val;
		}
		root->val = history.back().back().lock()->val;
		if (history.back().size() % 2 == 1)
		{
			history.back().back().lock()->parent.lock()->left.reset();
		}
		else
		{
			history.back().back().lock()->parent.lock()->right.reset();
		}
		history.back().pop_back();
		if (history.back().size() == 0)
			history.pop_back();

		bubble_down(root);
		return return_val;
	}

	void bubble_down(shared_ptr<node> root)
	{
		shared_ptr<node> best_child;
		if (root->left && root->right)
		{
			best_child = comp(root->left->val, root->right->val) ? root->left : root->right;
		}
		else if (root->left)
		{
			best_child = root->left;
		}
		else if (root->right)
		{
			best_child = root->right;
		}
		else
			return;
		if (!comp(root->val, best_child->val))
		{
			swap(root->val, best_child->val);
			bubble_down(best_child);
		}
	}

	operator bool() const
	{
		return bool(root);
	}

private:
	vector< vector<weak_ptr<node> > > history;
	shared_ptr<node> root;
	function<bool(int, int)> comp;
	int row_count;
};

int main(int argc, char * argv [])
{
	heap heap_obj({4, 9, 5, 3, -5, 2, 1, 6, 2, -1, 5, 10, 2, 5, 1, 3, 2, -21, 1, 6,2 , -20, 10515, -515, 15, 2, 1, 3, 5, 1, -500, 51}, false);
	while(heap_obj)
	{
		auto lowest = heap_obj.pop();
		if (!lowest)
		{
			cout << "heap empty" << endl;
		}
		else
		{
			cout << "lowest: " << *lowest << endl;
		}
	}
	return 0;
}
