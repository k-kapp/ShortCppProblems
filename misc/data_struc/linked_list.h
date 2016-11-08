#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

template<typename T>
struct node
{
	node(shared_ptr<node<T> > prev, shared_ptr<node<T> > next, T val) : prev(prev), next(next), val(val)
	{}
	node(T val) : prev(nullptr), next(nullptr), val(val)
	{}
	shared_ptr<node<T> > next;
	shared_ptr<node<T> > prev;
	T val;
};


template<typename T>
class linked_list
{
public:
	linked_list(vector<T> vals) : root(nullptr), back(nullptr)
	{
		create_list(vals);
	}

	linked_list() : root(nullptr), back(nullptr)
	{
	}

	void reverse()
	{
		shared_ptr<node<T> > temp;
		shared_ptr<node<T> > prev = nullptr;
		shared_ptr<node<T> > curr_node = root;

		while (curr_node)
		{
			swap(curr_node->next, curr_node->prev);
			curr_node = curr_node->prev;
		}

		swap(root, back);
	}

	void print()
	{
		if (!root)
			cout << "Empty list" << endl;
		shared_ptr<node<T> > curr_node = root;
		while (curr_node)
		{
			cout << curr_node->val;
			if (curr_node->next)
			{
				cout << " => ";
			}
			else
			{
				cout << endl;
			}
			curr_node = curr_node->next;
		}
	}

	void insert(int idx, T val)
	{
		auto new_node = make_shared<node<T> >(val);
		if (idx == 0)
		{
			new_node->next = root;
			new_node->prev = nullptr;
			root = new_node;
			if (!new_node->next)
				back = new_node;
			return;
		}

		auto prev_node = root;
		while (prev_node)
		{
			if (!prev_node->next || idx == 0)
			{
			
				new_node->next = prev_node->next;
				if (new_node->next)
				{
					new_node->next->prev = new_node;
				}
				else
				{
					back = new_node;
				}
				prev_node->next = new_node;
				new_node->prev = prev_node;
			}
			prev_node = prev_node->next;
		}
	}

	void append(T val)
	{
		if (!back)
		{
			back = make_shared<node<T> >(nullptr, nullptr, val);
			root = back;
		}
		else
		{
			back->next = make_shared<node<T> >(back, nullptr, val);
			back = back->next;
		}
	}

	void erase(int idx)
	{
		if (!root)
			return;
		if (idx == 0)
		{
			root = root->next;
			if (root)
			{
				root->prev = nullptr;
				if (!root->next)
				{
					back = root;
				}
			}
			return;
		}
		else if (!root->next)
		{
			return;
		}

		auto curr_node = root;
		for (int i = 1; i < idx; i++)
		{
			curr_node = curr_node->next;
			if (!curr_node->next)
			{
				return;
			}
		}

		curr_node->next = curr_node->next->next;
		if (!curr_node->next)
		{
			back = curr_node;
		}
		else
		{
			curr_node->next->prev = curr_node;
		}
	}

	void pop_back()
	{
		if (!back)
			return;
		else if (back->prev)
		{

			back->prev->next = nullptr;
		}
		else
		{
			back = root = nullptr;
		}
	}

	void pop_front()
	{
		if (!root)
			return;
		else if (root->next)
		{
			root->next->prev = nullptr;
			root = root->next;
		}
		else
		{
			root = back = nullptr;
		}
	}

	template<typename CompT = T>
	shared_ptr<T> find(CompT el, function<bool(CompT, T)> comp = function<bool(CompT, T)>())
	{
		auto curr_node = root;
		bool condition;
		if (comp)
		{
			condition = !comp(el, curr_node->val);
		}
		else
			condition = !(el == curr_node->val);
		while (condition)
		{
			curr_node = curr_node->next;
			if (!curr_node)
				return nullptr;
			if (comp)
			{
				condition = !comp(el, curr_node->val);
			}
			else
				condition = !(el == curr_node->val);
		}
		return make_shared<T>(curr_node->val);
	}

	operator bool() const
	{
		return bool(root);
	}

private:
	void create_list(vector<int> inputs)
	{
		for (auto &val : inputs)
		{
			append(val);
		}
	}

	shared_ptr<node<T> > back;
	shared_ptr<node<T> > root;
};



#endif
