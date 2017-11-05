#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "Neuron.h"

static constexpr int Ne = 10000; /** Number of excitatory neurons in simulated brain. */
static constexpr int Ni = 2500; /** Number of inhibitory neurons in simulated brain. */
static constexpr int Ce = 1000; /** Ne / 10 */
static constexpr int Ci = 250; /** Ni / 10 */
static constexpr int Cext = 1000;  /** Number of excitatory neurons in simulated brain. */
static constexpr double Je = 0.5; /** Potential of spikes from exterior neurons. */

/**
* Main function
* Behaviour analog to neural network
* @param argv[1] simulation length (in timesteps)
*/
int main(int argc, char* argv[])
{
	// Good practice to use with small scope
	using namespace std;

	static constexpr double TIME_STEP = 0.1; // ms

	// Initialize network
	vector<Neuron*> excitatory;
	vector<Neuron*> inhibitory;
	vector<Neuron*> neurons;

	// Initialize neurons
	for (int i = 0; i < Ne + Ni; ++i) {
		if (i < Ne) {
			Neuron* neuron = new Neuron(false, true);
			excitatory.push_back(neuron);
		}
		else {
			Neuron* neuron = new Neuron(false, false);
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
				neuron->AddConnexion(excitatory.at(target));
			}
			else {
				int target = rand() % 2500;
				neuron->AddConnexion(inhibitory.at(target));
			}
		}
	}

	// Initialize poisson distribution
	default_random_engine generator;
	poisson_distribution<int> distribution(2);

	// External current (unused)
	double ext_I = 6.0;
	int a = 0;
	int b = 100;
	double current_I = 0.0;

	// Set starting timestep to 0. Multiply by TIME_STEP to get actual time.
	int sim_time = 0;

	// First command-line argument is simulation length in timesteps
	int stop_time;
	sscanf(argv[1], "%d", &stop_time);

	// Loop while sim_time < stop_time
	while (sim_time < stop_time) {

		// Compute input current (unused)
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
	
			int curr_clock = neuron->GetClock();
			double curr_J = neuron->GetJ();
			
			// Main update
			bool spiked = neuron->Update(1);
			
			// If neuron spiked, send spike to all connected neurons
			if (spiked) {
					for (vector<Neuron*>::size_type j = 0; j != neuron->GetConnexions().size(); j++) {
						Neuron* connected = neuron->GetConnexions().at(j);
						connected->ReceiveSpike(curr_clock, curr_J);
					}
			}
		}

		// Increment simulation time
		sim_time++;
	}

	// Once simulation is over, print to file
	ofstream output("out.txt");
	for (vector<Neuron*>::size_type neuron_id = 0; neuron_id != neurons.size(); neuron_id++ )
		Neuron* neuron = neurons.at(neuron_id);
		for (vector<double>::const_iterator j = neuron->GetSpikes().begin(); j != neuron->GetSpikes().end(); ++j) {
			output << neuron_id << "\t" << *j;
		}
	}
	
	output.close();

    return 0;
}
