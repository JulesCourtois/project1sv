#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <assert.h>
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
    assert(argc > 1);

	// Good practice to use with small scope
    using namespace std;

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
	for (size_t i = 0; i != neurons.size(); i++ ) {
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
		for (size_t i = 0; i < neurons.size(); i++) {

			Neuron* neuron = neurons[i];

			// Update neuron, pass sim_time and current. Only neuron1 has current input
			if (!neuron->HasCurrent()) {
				current_I = 0.0;
			}

			// Prepare neuron with current and external spikes
			neuron->SetCurrent(current_I);
			double exterior_strength = Je * (double) distribution(generator);
			neuron->SetExteriorStrength(exterior_strength);

			double curr_J = neuron->GetJ();
			
			// Main update
			bool spiked = neuron->Update(1);
			
			// If neuron spiked, send spike to all connected neurons
			if (spiked) {
					vector<Neuron*> targets = neuron->GetConnexions();
					for (size_t j = 0; j < targets.size(); j++) {
                        targets[j]->ReceiveSpike(sim_time, curr_J);
					}
			}
		}

		// Increment simulation time
		sim_time++;
	}

	// Once simulation is over, print to file
	ofstream output("out.csv");
    for (size_t neuron_id = 0; neuron_id < neurons.size(); neuron_id++) {
		Neuron* neuron = neurons.at(neuron_id);
		vector<double> spikes = neuron->GetSpikes();
		for (size_t j = 0; j < spikes.size(); j++) {
            output << neuron_id << "\t" << spikes[j] << "\n";
		}
	}
	
	output.close();

    return 0;
}
