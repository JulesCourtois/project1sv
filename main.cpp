// Include libraries for printing and string manipulation
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Include our complex numbers class
#include "Neuron.h"

const double TIME_STEP = 0.1; // In ms
const double D = 1.0; // Delay #TODO: find real value
const double J = 1.0; // Spike force, #TODO: find real value

int main(int argc, char* argv[])
{
	// Good practice to use with small scope
	using namespace std;

	// Initialize neurons
	Neuron neuron1 = Neuron(TIME_STEP);
	Neuron neuron2 = Neuron(TIME_STEP);

	// Initialize connections
	map<Neuron, vector<Neuron>> connections = InitConnections(neurons);
	connections[neuron1] = {neuron2};
	connections[neuron2] = {neuron1};

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

		// Iterate over all neurons to update
		for (vector<Neuron>::iterator neuron = neurons.begin() ; neuron != neurons.end(); ++neuron) {
			// Update neuron, pass sim_time and current
			bool spiked = *neuron.Update(sim_time, current_I);
			cout << *neuron.GetMembranePotential() << "\n";

			if (spiked) {
					// TODO: translate to real code
					for (vector<Neuron>::iterator connected = connections[*neuron].begin() ; connected != connections[*neuron].end(); ++connected) {
						connected.Receive(sim_time + D, J);
					}
			}
		}



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
