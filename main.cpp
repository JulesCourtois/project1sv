// Include libraries for printing and string manipulation
#include <iostream>
#include <fstream>
#include <string>

// Include our complex numbers class
#include "Neuron.h"

double TIME_STEP = 0.1; // In ms

int main(int argc, char* argv[])
{
	// Good practice to use with small scope
	using namespace std;

	// Init neurons
	Neuron neuron1 = Neuron(TIME_STEP);

	// Options
	double ext_I = 10.0;
	double a = 0.0;
	double b = 10.0;

	// Equation variables
	double current_I = 0.0;

	// Set starting time to 0ms
	double sim_time = 0.0;

	// First command-line argument is duration time in seconds
	double stop_time;
	sscanf(argv[1], "%lf", &stop_time);

	// Loop while sim_time < stop_time
	while (sim_time < stop_time) {

		// Compute input current
		if (sim_time >= a && sim_time < b) {
			current_I = ext_I;
		}
		else {
			current_I = 0.0;
		}

		cout << neuron1.GetMembranePotential() << "\n";

		// Update neurons, pass current time, delta and current
		neuron1.Update(sim_time, TIME_STEP, current_I);
		
		sim_time += TIME_STEP;
	}

	// Once simulation is over, print to file
	ofstream output_file("out.txt");
	output_file << "Spikes occured at times:\n";
	for (vector<double>::const_iterator i = neuron1.GetSpikes().begin(); i != neuron1.GetSpikes().end(); ++i) {
		output_file << *i;
	}
	output_file.close();
	
    return 0;
}

/** Compréhension

N neurones dans le système,
Ne neurones excitatoires
Ni neurones inhibitoires

Chaque neurones a C connexions,
Ce connexions depuis des neurones Ne
Ci connexions depuis des neurones Ni

Chaque neurone a également Cext connexion extérieures de neurones excitatoires.
epsilon = Ce/Ne = Ci/Ni

timestep: 0.1 ms
refractory period: 2 ms
V_THRESHOLD = -55 mV
conductivity = 250.0
tau = 10
**/
