#ifndef PSO_H
#define PSO_H
#include <list>
#include <deque>
#include <utility>
#include <random>
#include <chrono>
#include <cfloat>
#include "misc.h"
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <thread>
using namespace std;

class PSO
{


	static default_random_engine generator;
	static normal_distribution<double> norm_dist;
	static uniform_real_distribution<double> unif_r_dist;
	static uniform_int_distribution<int> unif_i_dist;

	class individual
	{
	public:
		individual();
		individual(int dim, nutr_data * data_ptr);
		individual(vector< pair<double, double> > ranges, nutr_data * data_ptr);
		void update_velocity(const vector<double> &social_target);
		void update_position();
		void update_fitness();
		void update_cognitive();
		double get_fitness();
		void show_attributes();

		void show_position();

		vector<double> position;
		double fitness;

	private:
		void init_velocity();
		void init_params();
		double shortf_penalty();

		vector<double> velocity;

		vector<double> nutrients;
		bool needs_met;
		
		double scale_cog, scale_social, damp;
		
		vector<double> best_position;
		double best_fitness;

		nutr_data * data_ptr;
		vector<double> sphere_surface(const int &dim);
		void init_pos(const int &dim);
	};

public:
	PSO(nutr_data data, int dim, int pop_size, int local_size);
	void run(int no_change);
	double get_best_fitness();
	vector<double> get_best_pos();
	void show_best_position();
	void show_pop();
	void show_best();
	vector<double> best_pos_weights();

	vector<double> fitness_historical;
private:
	void init_pop();
	void update_best_indiv();
	void update_fitness();

	void update_indiv_fitness(PSO::individual &indiv);
	void do_updates(int idx, const vector<double> &best_pos);

	void iteration();

	nutr_data data;
	int dim;
	int pop_size;
	deque<individual> pop;

	int local_size;
	deque<individual> local_bests;

	int no_change;

	individual best_indiv;


	individual * get_area_best(pair<int, int> &range);
	void get_indiv_loc_best(int indiv_idx);
	void get_local_bests();


};

#endif
