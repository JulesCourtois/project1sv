// Include header file for Neuron class
#include "Neuron.h"
#include <cmath>
#include <iostream>

// Default constructor sets to 0
Neuron::Neuron(bool curr, bool exc) {
	is_refractory = false;
	has_current = curr;
	spikes = {};
	spike_count = 0;
	refractory_time = 0.0;
	ring_ind = 0;
	membrane_potential = V_RESET;

	is_excitatory = exc;
	if (is_excitatory) {
		J = 0.1;
	}
	else {
		J = 0.5;
	}

	connexions = {};

	ring_buffer.assign(RING_BUFFER_SIZE, 0);
	c1 = std::exp(-TIME_STEP / TAU);
	c2 = R * (1 - c1);
}

std::vector<Neuron*> Neuron::GetConnexions() {
	return connexions;
}

bool Neuron::HasCurrent() {
	return has_current;
}

void Neuron::AddSpike() {
	is_refractory = true;
	refractory_time = 0.0;
	++spike_count;
	spikes.push_back((double) local_clock*TIME_STEP);
}

void Neuron::HandleRefractoryPeriod() {
	refractory_time++;
	if (refractory_time > REFRACTORY_PERIOD) {
		is_refractory = false;
	}
	membrane_potential = 0.0;
}

bool Neuron::Update(int timesteps) {
	bool ret = false;

	for (int i = 0; i < timesteps; i++) {
		// Manage refractory period
		if (is_refractory) {
			HandleRefractoryPeriod();
		}
		else {
			// Manage if spike detected
			if (membrane_potential > V_THRESHOLD) {
				AddSpike();
				// Update ring buffer
				ring_buffer.at(ring_ind) = 0;
				ring_ind = (ring_ind + 1) % RING_BUFFER_SIZE;
				membrane_potential = 0.0;
				ret = true;
			}
			else {
				// Solve equation
				membrane_potential = c1 * membrane_potential + input_current * c2 + ring_buffer.at(ring_ind) + exterior_strength;

				// Update ring buffer
				ring_buffer.at(ring_ind) = 0;
				ring_ind = (ring_ind + 1) % RING_BUFFER_SIZE;
			}
		}
		local_clock++;
	}

	// Return true iff spike has occurred
	return false;
}

void Neuron::AddConnexion(Neuron* neuron) {
	connexions.push_back(neuron);
}

void Neuron::ReceiveSpike(int clock, double strength) {
	int offset = 0;
	if (local_clock == clock) {
		offset = -1;
	}
	int last_ind = (ring_ind + DELAY + offset) % RING_BUFFER_SIZE;
	ring_buffer.at(last_ind) += strength;
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

void Neuron::SetCurrent(double i) {
	input_current = i;
}

double Neuron::GetJ() {
	return J;
}

int Neuron::GetClock() {
	return local_clock;
}

void Neuron::SetExteriorStrength(double strength) {
	exterior_strength = strength;
}
