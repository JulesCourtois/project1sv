// Include libraries for printing and string manipulation
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

// Include our complex numbers class
#include "Neuron.h"

static constexpr int Ne = 10000; // Excitatory neurons
static constexpr int Ni = 2500;  // Inhibitory neurons
static constexpr int Ce = 1000;
static constexpr int Ci = 250;
static constexpr int Cext = 1000; // Exterior (excitatory) neurons
static constexpr double Je = 0.5; // Exterior (excitatory) neurons

int main(int argc, char* argv[])
{
	// Good practice to use with small scope
	using namespace std;

	static constexpr double TIME_STEP = 0.1; // ms

	static constexpr double TIME_STEP = 0.1; // ms

	// Initialize network
	vector<Neuron*> excitatory;
	vector<Neuron*> inhibitory;
	vector<Neuron*> neurons;

	// Initialize neurons
	for (int i = 0; i < Ne + Ni; ++i) {
		Neuron neuron;
		if (i < Ne) {
			neuron = Neuron(false, true);
			excitatory.push_back(neuron);
		}
		else {
			neuron = Neuron(false, false);
			inhibitory.push_back(neuron);
		}
	}

	neurons = excitatory;
	neurons.insert(neurons.end(), inhibitory.begin(), inhibitory.end());

	// Initialize connexions
	for (vector<Neuron*>::size_type i = 0; i != neurons.size(); i++ ) {
		Neuron* neuron = neurons.at(i);
		for (int j = 0; j < Ce + Ci; j++) {
			if (j < Ce) {
				int target = rand() % 10000;
				neuron->AddConnexion(&(excitatory.at(target)));
			}
			else {
				int target = rand() % 2500;
				neuron->AddConnexion(&(inhibitory.at(target)));
			}
		}
	}

	// Initialize poisson distribution
	default_random_engine generator;
  poisson_distribution<int> distribution(10.0);

	// Initialize neurons
	// Neuron neuron2 = Neuron(false, true);
	// Neuron neuron1 = Neuron(true, true);

	// Initialize connections
	// neuron1.AddConnexion(&neuron2);
	// vector<Neuron*> neurons = {&neuron1, &neuron2};

	// Options
	double ext_I = 6.0;
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
		for (vector<Neuron*>::size_type i = 0; i != neurons.size(); i++ ) {

			Neuron* neuron = neurons.at(i);

			// Update neuron, pass sim_time and current. Only neuron1 has current input
			if (!neuron->HasCurrent()) {
				current_I = 0.0;
			}

			// Prepare neuron with current and external spikes
			neuron->SetCurrent(current_I);
		  double exterior_strength = Je * (double) distribution(generator);
			neuron->SetExteriorStrength(exterior_strength);

			bool spiked = neuron->Update(1);

			// If neuron spiked, send spike to all connected neurons
			if (spiked) {
					for (vector<Neuron*>::size_type j = 0; j != neuron->GetConnexions().size(); j++) {
						neuron->GetConnexions().at(j)->ReceiveSpike(neuron->GetClock(), neuron->GetJ());
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
