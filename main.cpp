// Include libraries for printing and string manipulation
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Include our complex numbers class
#include "Neuron.h"

const double TIME_STEP = 0.1; // In ms

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

			// If neuron spiked, send spike to all connected neurons
			if (spiked) {
					for (vector<Neuron>::iterator connected = connections[*neuron].begin() ; connected != connections[*neuron].end(); ++connected) {
						connected.ReceiveSpike();
					}
			}
		}

		sim_time += TIME_STEP;
	}

	// Once simulation is over, print to file
	ofstream output1("out1.txt");
	output1 << "Spikes in Neuron1 occured at times:\n";
	for (vector<double>::const_iterator i = neuron1.GetSpikes().begin(); i != neuron1.GetSpikes().end(); ++i) {
		output1 << *i;
	}
	output1 << "\nSpikes in Neuron2 occured at times:\n";
	for (vector<double>::const_iterator i = neuron2.GetSpikes().begin(); i != neuron2.GetSpikes().end(); ++i) {
		output1 << *i;
	}
	output1.close();

    return 0;
}
