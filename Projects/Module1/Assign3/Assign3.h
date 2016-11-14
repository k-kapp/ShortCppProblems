#ifndef ASSIGN_H
#define ASSIGN_H

#include "Plot.h"
#include <utility>
#include <vector>
#include <iostream>
#include <random>
#include <deque>
#include <chrono>
#include <algorithm>
#include <cfloat>
#include <assert.h>
#include <string>
#include <climits>
using namespace std;

enum mutate_method
{
	RNORM,
	PPULL
};

enum cross_method
{
	STD,
	WEIGHT,
	NONE
};

struct cross_type
{
	cross_method cross_m;
	int num_choices;
};

struct individual
{
	vector<double> position;
	double value;

	individual operator - (individual &other)
	{
		int pos_idx;
		vector<double> new_pos = vector<double>(position.size());
		for (auto this_pos_iter = position.begin(), other_pos_iter = other.position.begin(); this_pos_iter != position.end();
			advance(this_pos_iter, 1), advance(other_pos_iter, 1))
		{
			pos_idx = this_pos_iter - position.begin();
			new_pos[pos_idx] = *this_pos_iter - *other_pos_iter;
			if (new_pos[pos_idx] < 0)
				new_pos[pos_idx] = 0;
		}
		return {new_pos, 0};
	}

	individual operator + (individual &other)
	{
		int pos_idx;
		vector<double> new_pos = vector<double>(position.size());
		for (auto this_pos_iter = position.begin(), other_pos_iter = other.position.begin(); this_pos_iter != position.end();
			advance(this_pos_iter, 1), advance(other_pos_iter, 1))
		{
			pos_idx = this_pos_iter - position.begin();
			new_pos[pos_idx] = *this_pos_iter + *other_pos_iter;
			if (new_pos[pos_idx] < 0)
				new_pos[pos_idx] = 0;
		}
		return {new_pos, 0};
	}

	individual operator * (double scalar)
	{
		int pos_idx;
		vector<double> new_pos = vector<double>(position.size());
		for (auto pos_iter = position.begin(); pos_iter != position.end();
			advance(pos_iter, 1))
		{
			pos_idx = pos_iter - position.begin();
			new_pos[pos_idx] = (*pos_iter)*scalar;
			if (new_pos[pos_idx] < 0)
				new_pos[pos_idx] = 0;
		}
		return {new_pos, 0};
	}

	bool operator > (const individual &rhs) const
	{
		return this->value > rhs.value;
	}

	bool operator < (const individual &rhs) const
	{
		return this->value < rhs.value;
	}

	bool operator >= (const individual &rhs) const
	{
		return *this > rhs || this->value == rhs.value;
	}

	bool operator <= (const individual &rhs) const
	{
		return *this < rhs || this->value == rhs.value;
	}

	bool operator == (const individual &rhs) const
	{
		return this->value == rhs.value;
	}

};
using prog_type = vector<vector<vector<double> > >;

class EvolAlgo
{
	public:
		EvolAlgo(int dim, int pop_size, function<double(const vector<double> &)> obj_func, bool max, mutate_method m_method, cross_type cross_t, vector<pair<double, double> > dim_limits);

		void init_pop();
		void weight_cross(individual & new_indiv);
		void crossover(individual & new_indiv);
		void no_cross(individual & new_indiv);
		void evaluate(individual &indiv);
		void evaluate(int indiv_idx);
		void run(int no_change_stop, int max_iter = INT_MAX);

		void show_best();
		void show_pop();
		vector<vector<double> > get_prog_vec(int type);
		int num_iter();
		int get_optim_count();
		double get_time();
		int get_dim();
		bool best_achieved;

	private:
		void mutate(individual &indiv);
		void new_generation();
		void insert_offspring(individual &indiv);
		void update_prog_vecs();
		void reverse_prog_vecs();
		double time_taken;
		int pop_size;
		int dim;
		int count;
		int iter_count;
		int optim_count;
		int num_choices;
		vector<pair<double, double> > dim_ranges;
		deque<individual> pop;
		default_random_engine generator;
		normal_distribution<double> norm_dist;
		uniform_real_distribution<double> unif_real;
		uniform_int_distribution<int> unif_int_dim;
		uniform_int_distribution<int> unif_int_pop;
		void show_indiv(deque<individual>::iterator indiv_iter);
		void mutate_rand_norm(individual &offspring);
		void (EvolAlgo::* mutate_f)(individual &);
		void (EvolAlgo::* cross_f)(individual &);
		function<double(const vector<double> &)> obj_func;
		unsigned int indiv_idx;
		bool max;
		function<bool(double, double)> comp_vals;
		function<bool(const individual &, const individual &)> comp_indivs;
		prog_type all_prog_vecs;
};

#endif
