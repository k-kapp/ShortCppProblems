#include "linked_list.h"
#include <vector>
#include <iostream>

using namespace std;

struct entry
{
	entry(string key, int val) : key(key), val(val)
	{}

	entry() : key(string()), val(0)
	{}

	string key;
	unsigned int val;
};

bool operator == (string other_key, entry obj)
{
	return other_key == obj.key;
}
bool operator == (entry obj, string other_key)
{
	return other_key == obj.key;
}

unsigned long djb2_hash(string str)
{
	unsigned long hash = 5381;

	for (int ch : str)
	{
		hash = ((hash << 5) + hash) + ch;
	}

	return hash;
}

template<typename T>
class hash_table
{
public:
	hash_table(int init_size, function<unsigned long(decltype(T().key))> hash_func, double max_load) 
		: table(init_size, linked_list<T>()), hash_func(hash_func), max_load(max_load),
			filled(0), load_factor(0)
	{
	}

	hash_table(int init_size, function<unsigned long(decltype(T().key))> hash_func, vector<T> init_data, double max_load) 
		: hash_func(hash_func), max_load(max_load), table(init_size, linked_list<T>()),
			filled(0), load_factor(0)
	{
		for (auto &val : init_data)
		{
			insert(val);
		}
	}

	void insert(T entry)
	{
		int insert_idx = hash_func(entry.key) % table.size();

		if (!table.at(insert_idx))
		{
			filled++;
			load_factor = (double)filled/table.size();
		}
		table.at(insert_idx).append(entry);
	}

	shared_ptr<entry> retrieve(decltype(T().key) search_term)
	{
		int idx = hash_func(search_term) % table.size();

		return table.at(idx).find(search_term);
	}

private:
	vector<linked_list<T> > table;
	int filled;
	double load_factor;
	double max_load;
	function<unsigned long(decltype(T().key))> hash_func;
	
};

int main(int argc, char * argv [])
{
	hash_table<entry> table(10, djb2_hash, {{"Name1", 456}, {"Name2", 265}, {"Name3", 515}}, 0.8);
	auto found = table.retrieve("Name2");
	if (found)
		cout << found->val << endl;
	found = table.retrieve("Name3");
	if (found)
		cout << found->val << endl;
	return 0;
}
