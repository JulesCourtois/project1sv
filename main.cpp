// Include libraries for printing and string manipulation
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Include our complex numbers class
#include "Neuron.h"


int main(int argc, char* argv[])
{
	// Good practice to use with small scope
	using namespace std;
	
	static constexpr double TIME_STEP = 0.1; // ms

	// Initialize neurons
	vector<Neuron> n2conn = {};
	Neuron neuron2 = Neuron(n2conn, false);
	vector<Neuron> n1conn = {neuron2};
	Neuron neuron1 = Neuron(n1conn, true);

	// Initialize connections
	vector<Neuron> neurons = {neuron1, neuron2};

	// Options
	double ext_I = 5.0;
	int a = 0;
	int b = 100;

	// Equation variables
	double current_I = 0.0;

	// Set starting timestep to 0. Multiply by TIME_STEP to get actual time.
	int sim_time = 0;

	// First command-line argument is duration time in seconds
	int stop_time;
	sscanf(argv[1], "%d", &stop_time);

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

			// Update neuron, pass sim_time and current. Only neuron1 has current input
			if (!neuron->HasCurrent()) {
				current_I = 0.0;
			}
	
			bool spiked = neuron->Update(sim_time, current_I);
			cout << neuron->GetMembranePotential() << "\n";

			// If neuron spiked, send spike to all connected neurons
			if (spiked) {
					for (vector<Neuron>::iterator connected = neuron->GetConnections().begin() ; connected != neuron->GetConnections().end(); ++connected) {
						connected->ReceiveSpike();
					}
			}
		}

		sim_time++;
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
