#include "import_data.h"
#include "Plot.h"
#include "PSO.h"
#include "GA.h"
#include "DiffEvo.h"

int main()
{
	nutr_data data;

	import("Prices.csv", data, true);
	import("Needs.csv", data, false);
	/*
	show_spec_data(data, true);

	cout << endl << endl;
	
	show_spec_data(data, false);
	*/
	PSO optim = PSO(data, 77, 600, 10);
	optim.run(100);

	cout << "best fitness: " << optim.get_best_fitness() << endl;
	cout << "best position: " << endl;
	optim.show_best_position();
/*
	GA optim = GA(data, 150, vector<pair<double, double> > (77, {0, 50}));

	optim.run(3000);

	optim.show_best();
*/
/*
	DiffEvo optim = DiffEvo(data, 150, vector<pair<double, double> > (77, {0, 50}), 15, {0.0, 1.0});
	optim.show_best();
	
	optim.run(10);
	//optim3.iterate();
	//optim3.iterate();


	optim.show_best();
*/
	Plot plt_hist = Plot(optim.get_best_pos());
	Plot plt_hist2 = Plot(optim.best_pos_weights());
	Plot plt_line = Plot(optim.fitness_historical);

	plt_hist.plot_basic_hist("Dollars per food type");
	plt_hist2.plot_basic_hist("Weight (grams) per food type");
	plt_line.plot_basic_line();

	return 0;
}
