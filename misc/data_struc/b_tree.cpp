#include <iostream>
#include <vector>
#include <memory>
#include <utility>

using namespace std;

struct node;

using key_cont = vector<int>;
using child_cont = vector<shared_ptr<node> >;

const int max_size = 4;

struct node
{
	node(key_cont keys, child_cont children)
		: keys(keys), children(children)
	{}
	key_cont keys;
	child_cont children;

	bool is_leaf()
	{
		if (children.front())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	int size()
	{
		return keys.size();
	}
};

void print_vec(vector<int> vec)
{
	cout << "[";

	for (int i = 0; i < vec.size(); i++)
	{
		if (i < vec.size() - 1)
		{
			cout << vec.at(i) << ", ";
		}
		else
		{
			cout << vec.at(i) << "]";
		}
	}
}

/*
	this function only prints the first three generations of the tree
*/
void print_tree(shared_ptr<node> root)
{
	if (!root)
	{
		cout << "Tree empty" << endl;
		return;
	}
	print_vec(root->keys);

	cout << endl;
	
	for (auto &child : root->children)
	{
		if (child)
		{
			print_vec(child->keys);
			cout << " ";
		}
	}

	cout << endl;

	for (auto &child : root->children)
	{
		if (child)
			for (auto &grand_child : child->children)
			{
				if (grand_child)
				{
					print_vec(grand_child->keys);
					cout << " ";
				}
			}
			cout << " | ";
	}

	cout << endl;

}

/*
	inserts val in the appropriate position
*/
int insert_inorder(key_cont &keys, int val)
{
	for (auto key_iter = keys.begin(); key_iter != keys.end(); advance(key_iter, 1))
	{
		if (val <= *key_iter)
		{
			key_iter = keys.insert(key_iter, val);
			return distance(keys.begin(), key_iter);
		}
	}
	keys.push_back(val);
	return keys.size() - 1;
}

node &get_next_node(node &root, int val)
{
	key_cont::iterator key_iter;
	child_cont::iterator child_iter;
	for (key_iter = root.keys.begin(), child_iter = root.children.begin();
			key_iter != root.keys.end(); advance(key_iter, 1), advance(child_iter, 1))
	{
		if (val <= *key_iter)
		{
			return *(*child_iter);
		}
	}
	return *root.children.back();
}

/*
	inserts val into root. Also takes care of cases where the root contains too many keys. In these cases, 
		the root is broken in two, with the median key of root getting sent to root's parent.

		new_child is not a null pointer in cases where this root node is finding a new position for a child node
			previously broken up due to an insertion. In these cases, val would be the value of the median key 
			from the direct descendant of this root node, and new child is the left half of the child node which was broken up
		
		this function returns the value of the median key, as well as the left of the broken up node if it
			was broken up due to it containing too many nodes. Otherwise, if insertion is successful without the need
			to break it up, then it returns -1 and a null pointer
*/
pair<int, shared_ptr<node> > insert_here(node &root, int val, shared_ptr<node> new_child = shared_ptr<node>(nullptr))
{

	int new_idx = insert_inorder(root.keys, val);
	root.children.insert(next(root.children.begin(), new_idx), new_child);
	if (root.size() > max_size)
	{
		auto left_keys = key_cont(root.keys.begin(), next(root.keys.begin(), (root.size() + 1)/2));
		auto left_children = child_cont(root.children.begin(), next(root.children.begin(), (root.size() + 1)/2));
		int median_val = left_keys.back();
		left_keys.pop_back();

		root.children.erase(root.children.begin(), next(root.children.begin(), (root.size() + 1)/2));
		root.keys.erase(root.keys.begin(), next(root.keys.begin(), (root.size() + 1)/2));

		shared_ptr<node> new_node = make_shared<node>(left_keys, left_children);

		return {median_val, new_node};
	}
	else
	{
		return {-1, nullptr};
	}

}

/*
 * insert function: if splitting, send left node up to parent node
 */
pair<int, shared_ptr<node> > insert_help(node &root, int val)
{
	if (root.is_leaf())
	{
		return insert_here(root, val);
	}
	else
	{
		node & next_node = get_next_node(root, val);

		auto return_val = insert_help(next_node, val);

		if (return_val.second)
		{
			return_val = insert_here(root, return_val.first, return_val.second);
			return return_val;
		}
		else
		{
			return {-1, nullptr};
		}
	}
}

void insert(shared_ptr<node> &root, int val)
{
	auto return_val = insert_help(*root, val);

	if (return_val.second)
	{
		auto old_root = root;
		root = make_shared<node>(key_cont(1, return_val.first), child_cont{return_val.second, old_root});
	}
}


shared_ptr<node> create_tree(vector<int> keys)
{
	shared_ptr<node> root = make_shared<node>(key_cont(1, keys.front()), child_cont(2, shared_ptr<node>(nullptr)));

	for (auto key_iter = next(keys.begin(), 1); key_iter != keys.end(); advance(key_iter, 1))
	{
		insert(root, *key_iter);
	}

	return root;
}

void rotate_from_right(node &left, node &right, node &parent, int idx)
{
	left.keys.push_back(parent.keys.at(idx));
	left.children.push_back(right.children.front());
	right.children.erase(right.children.begin());
	parent.keys.at(idx) = right.keys.front();
	right.keys.erase(right.keys.begin());
}
void rotate_from_left(node &left, node &right, node &parent, int idx)
{
	right.keys.insert(right.keys.begin(), parent.keys.at(idx));
	right.children.insert(right.children.begin(), left.children.back());
	left.children.pop_back();
	parent.keys.at(idx) = left.keys.back();
	left.keys.pop_back();
}

/*
	merges two nodes that did not have enough keys for sharing
*/
void merge(node &left, child_cont::iterator right_iter, node &parent)
{
	int idx = distance(parent.children.begin(), right_iter) - 1;

	left.keys.push_back(parent.keys.at(idx));
	left.keys.insert(left.keys.end(), (*right_iter)->keys.begin(), (*right_iter)->keys.end());
	left.children.insert(left.children.end(), (*right_iter)->children.begin(), (*right_iter)->children.end());

	parent.keys.erase(next(parent.keys.begin(), distance(parent.children.begin(), right_iter) - 1));
	parent.children.erase(right_iter);
}


pair<int, bool> get_rightmost(node &root)
{
	if (root.is_leaf())
	{
		int ret_val = root.keys.back();
		root.keys.pop_back();
		root.children.pop_back();
		bool go_back = root.size() == 0;
		return {ret_val, go_back};
	}
	else
	{
		return get_rightmost(*root.children.back());
	}
}

pair<bool, bool> find_neighbour(node &parent, int idx)
{
	if (idx == 0)
	{
		if (parent.children.at(1)->size() > 1)
		{
			return {true, true};
		}
		else
		{
			return {true, false};
		}
	}
	else if (idx == parent.children.size() - 1)
	{
		if (parent.children.back()->size() > 1)
		{
			return {false, true};
		}
		else
		{
			return {false, false};
		}
	}
	else
	{
		if (parent.children.at(idx + 1)->size() > 1)
		{
			return {true, true};
		}
		else if (parent.children.at(idx - 1)->size() > 1)
		{
			return {false, true};
		}
		else
		{
			return {true, false};
		}
	}
}
pair<int *, child_cont::iterator> remove_here(node &root, int val)
{
	child_cont::iterator child_iter;
	key_cont::iterator key_iter;
	for (key_iter = root.keys.begin(), child_iter = root.children.begin();
			key_iter != root.keys.end();
			advance(key_iter, 1), advance(child_iter, 1))
	{
		if (val == *key_iter)
		{
			if (root.is_leaf())
			{
				root.keys.erase(key_iter);
				root.children.pop_back();
				return {nullptr, root.children.end()};
			}
			else
			{
				return {&(*key_iter), child_iter};
			}
		}
		else if (val < *key_iter)
		{
			return {nullptr, child_iter};
		}
	}

	return {nullptr, next(root.children.end(), -1)};
}

/*
	the helper function was written so that the "real" function may be called more elegantly,
		with less parameters. The parameters in the remove_help function is to assist with 
		recursion
*/
void remove_help(shared_ptr<node> &root, int root_idx, shared_ptr<node> parent, int val,
					int rightmost, int * insertee_replace)
{
	
	pair<int *, child_cont::iterator> search_result;
	if (rightmost == 2)
		search_result = remove_here(*root, val);

	bool fix_this = false;


	if (rightmost == 2)
	{
		if (search_result.first)
		{
			remove_help(*search_result.second, distance(root->children.begin(), search_result.second), 
						root, -1, 1, search_result.first);
		}
		else if (!root->is_leaf())
		{
			remove_help(*search_result.second, distance(root->children.begin(), search_result.second), 
						root, val, rightmost, insertee_replace);
		}
	}
	else if (rightmost == 1)
	{
		if (root->is_leaf())
		{
			*insertee_replace = root->keys.back();
			root->keys.pop_back();
			root->children.pop_back();
		}
		else
		{
			remove_help(root->children.back(), root->children.size() - 1,
						root, -1, rightmost, insertee_replace);
		}

	}


	if (root->size() == 0)
		fix_this = true;

	if (!fix_this)
		return;

	if (!parent)
	{
		root = root->children.front();
		return;
	}

	auto neighbour_info = find_neighbour(*parent, root_idx);

	if (neighbour_info.second)
	{
		if (neighbour_info.first)
		{
			rotate_from_right(*root, *parent->children.at(root_idx + 1), *parent, root_idx);
		}
		else
		{
			rotate_from_left(*parent->children.at(root_idx - 1), *root, *parent, root_idx - 1);
		}
	}
	else
	{
		if (neighbour_info.first)
		{
			merge(*root, next(parent->children.begin(), root_idx + 1), *parent);
		}
		else
		{
			merge(*parent->children.at(root_idx - 1), next(parent->children.begin(), root_idx), *parent);
		}
	}
}

void remove(shared_ptr<node> &root, int val)
{
	remove_help(root, 0, nullptr, val, 2, nullptr);
}


int main(int argc, char * argv [])
{
	vector<int> my_vec = {4, 5, 3, 2, 10, 5, 1, 9, 5, 8, 11, 16, 18, 7, 7, 2, 6, 8, 9, 20, 40, 35, 22, 25};

	auto root_ptr = create_tree(my_vec);

	print_tree(root_ptr);
	for (auto vec_iter = my_vec.begin(); vec_iter != next(my_vec.end(), 0); advance(vec_iter, 1))
	{
		remove(root_ptr, *vec_iter);
		print_tree(root_ptr);
	}

	return 0;
}
