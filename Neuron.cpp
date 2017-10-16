// Include header file for Neuron class
#include "Neuron.h"
#include <cmath>
#include <iostream>

// Default constructor sets to 0
Neuron::Neuron(std::vector<Neuron> connected_to, bool curr) {
	is_refractory = false;
	has_current = curr;
	spikes = {};
	spike_count = 0;
	refractory_time = 0.0;
	membrane_potential = V_RESET;
	connections = connected_to;

	c1 = std::exp(-TIME_STEP / TAU);
	c2 = R * (1 - c1);
}

std::vector<Neuron> Neuron::GetConnections() {
	return connections;
}

bool Neuron::HasCurrent() {
	return has_current;
}

void Neuron::AddSpike(int sim_time) {
	is_refractory = true;
	refractory_time = 0.0;
	++spike_count;
	spikes.push_back((double) sim_time*TIME_STEP);
}

void Neuron::HandleRefractoryPeriod() {
	refractory_time++;
	if (refractory_time > REFRACTORY_PERIOD) {
		is_refractory = false;
	}
	membrane_potential = 0.0;
}

bool Neuron::Update(int sim_time, double input_current) {
	bool ret = false;
	// Manage refractory period
	if (is_refractory) {
		HandleRefractoryPeriod();
	}
	else {
		// Manage if spike detected
		if (membrane_potential > V_THRESHOLD) {
			AddSpike(sim_time);
			ret = true;
		}

		// Solve equation
		membrane_potential = c1 * membrane_potential + input_current * c2 + J * ring_buffer[ring_ind];

		// Update ring buffer
		ring_buffer[ring_ind] = 0;
		ring_ind = (ring_ind + 1) % RING_BUFFER_SIZE;
	}

	// Return true iff spike has occurred
	return ret;
}

void Neuron::ReceiveSpike() {
	int last_ind = (ring_ind - 1) % RING_BUFFER_SIZE;
	ring_buffer[last_ind] += 1;
	std::cout << "ring buffer last ind:" << ring_buffer[last_ind];
}

double Neuron::GetMembranePotential() {
	return membrane_potential;
}

bool Neuron::IsRefractory() {
	return is_refractory;
}

int Neuron::GetSpikeCount() {
	return spike_count;
}

std::vector<double> Neuron::GetSpikes() {
	return spikes;
}
