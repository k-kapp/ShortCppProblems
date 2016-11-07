#include <vector>
#include <iostream>
#include <memory>

using namespace std;

struct node
{
	node(int val, shared_ptr<node> left, shared_ptr<node> right)
		: val(val), left(left), right(right)
	{}
	int val;
	shared_ptr<node> left;
	shared_ptr<node> right;
};

void insert(shared_ptr<node> root, int val)
{
	if (val <= root->val)
	{
		if (root->left)
		{
			insert(root->left, val);
		}
		else
		{
			root->left = make_shared<node>(val, nullptr, nullptr);
		}
	}
	else
	{
		if (root->right)
		{
			insert(root->right, val);
		}
		else
		{
			root->right = make_shared<node>(val, nullptr, nullptr);
		}
	}
}

shared_ptr<int> erase(shared_ptr<node> &, int);

void reassign_rightmost(shared_ptr<node> &root, int * assignee)
{
	if (root->right)
	{
		reassign_rightmost(root->right, assignee);
	}
	else
	{
		*assignee = root->val;
		erase(root, root->val);
	}
}

void reassign_leftmost(shared_ptr<node> &root, int * assignee)
{
	if (root->left)
	{
		reassign_leftmost(root->left, assignee);
	}
	else
	{
		*assignee = root->val;
		erase(root, root->val);
	}
}

shared_ptr<int> erase(shared_ptr<node> &root, int val)
{
	if (root->val == val)
	{
		auto return_val = make_shared<int>(val);
		if (root->left)
		{
			reassign_rightmost(root->left, &root->val);
		}
		else if (root->right)
		{
			reassign_leftmost(root->right, &root->val);
		}
		else
		{
			root.reset();
		}
		return return_val;
	}
	else
	{
		if (root->left && val <= root->val)
		{
			return erase(root->left, val);
		}
		else if (root->right && val > root->val)
		{
			return erase(root->right, val);
		}
		else
		{
			return nullptr;
		}
	}
}

shared_ptr<node> create_tree(vector<int> values)
{
	shared_ptr<node> root_ptr(nullptr);
	if (values.size() == 0)
		return root_ptr;
	
	root_ptr = make_shared<node>(values.front(), nullptr, nullptr);

	for (auto val_iter = next(values.begin(), 1); val_iter != values.end(); advance(val_iter, 1))
	{
		insert(root_ptr, *val_iter);
	}
	return root_ptr;
}

shared_ptr<int> erase_leftmost(shared_ptr<node> &root)
{
	if (root->left)
	{
		return erase_leftmost(root->left);
	}
	else
	{
		return erase(root, root->val);
	}
}


vector<int> build_ordered(vector<int> values)
{
	auto root = create_tree(values);

	vector<int> new_vec;

	while (root)
	{
		new_vec.push_back(*erase_leftmost(root));
	}
	return new_vec;
}

int main(int argc, char * argv [])
{
	vector<int> vec = {4, 8, 9, 3, 5, 9, 22, 15, 22, 1};
	auto root = create_tree(vec);

	auto ordered = build_ordered(vec);
	for (auto &val : ordered)
	{
		cout << val << " ";
	}
	cout << endl;


	return 0;
}
