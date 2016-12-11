#include <vector>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

/*
 * data structure that represents a node in the trie.
 * char c is the content of the node, 
 * int count is the amount of stored words that match 
 * 		the prefix represented by the node
 * int value is the arbitrary integer assigned to the node
 * 		if it represents a whole word (not just a prefix)
 * 		TODO: have int value represent the number of stored 
 * 				words that match the word represented by node?
 */
struct node
{
	node(char c) : c(c), count(0), value(0)
	{}

	vector<shared_ptr<node> > children;
	char c;
	int count;
	int value;
};


/*
 * class that implements a trie data structure, with supporting
 * functions for getting the amount of words stored that have a certain
 * prefix, and adding new words.
 */
class trie
{
public:
	trie() : root(0)
	{
		next_val = 1;
	}

	void add(const char * word)
	{
		curr = &root;

		add_help(word);
	}

	int get_size()
	{
		return root.count;
	}

	/*
	 * function that finds the amount of words that match the prefix given by
	 * char * prefix
	 */
	int num_prefix(const char * prefix)
	{
		int num = 0;
		int curr_idx = 0;
		curr = &root;

		do
		{
			curr = find_match(prefix[0]).get();
			prefix++;
		} while (curr && prefix[0] != '\0');

		if (curr && prefix[0] == '\0')
		{
			return curr->count;
		}
		else
		{
			return 0;
		}
	}

private:
	node root;
	node * curr;
	int next_val;

	shared_ptr<node> find_match(char c)
	{
		if (curr->children.size() == 0)
			return nullptr;
		else
		{
			for (auto &child_ptr : curr->children)
			{
				if (c == child_ptr->c)
				{
					return child_ptr;
				}
			}
		}
		return nullptr;
	}

	/*
	 * recursive helper function for adding a new word into the trie. 
	 * A character array, instead of a string, is used so that
	 * no copying has to be done when finding matches recursively.
	 * Instead, the pointer to the start of the array is incremented, so
	 * that smaller substrings may be matched against the characters in the trie.
	 * The character array contents is managed by the caller.
	 */
	void add_help(const char * word)
	{
		if (word[0] == '\0')
		{
			curr->value = next_val;
			curr->count++;
			next_val++;
			return;
		}
		auto match_ptr = find_match(word[0]);
		if (!match_ptr)
		{
			curr->children.push_back(make_shared<node>(word[0]));
			curr->count++;

			curr = curr->children.back().get();
			add_help(word + 1);

		}
		else
		{
			curr->count++;
			curr = match_ptr.get();
			add_help(word + 1);
		}
	}
};

static trie data;
static vector<int> found_nums;

void parse_command(string command, string content)
{
	if (command == "add")
	{
		data.add(content.c_str());
	}
	else if (command == "find")
	{
		found_nums.push_back(data.num_prefix(content.c_str()));
	}
}

int main(int argc, char * argv [])
{
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		string command, content;

		cin >> command;
		cin >> content;

		parse_command(command, content);
	}

	for (auto &val : found_nums)
	{
		cout << val << endl;
	}

	return 0;

}

