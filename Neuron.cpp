// Include header file for Neuron class
#include "Neuron.h"
#include <cmath>
#include <iostream>

// Default constructor sets to 0
Neuron::Neuron(double timestep) {
	is_refractory = false;
	spikes = {};
	spike_count = 0;
	refractory_time = 0.0;
	membrane_potential = V_RESET;
	h = timestep;

	c1 = std::exp(-h / TAU);
	c2 = R * (1 - c1);
}

void Neuron::AddSpike(double sim_time) {
	is_refractory = true;
	refractory_time = 0.0;
	++spike_count;
	spikes.push_back(sim_time);
}

void Neuron::HandleRefractoryPeriod() {
	refractory_time += h;
	if (refractory_time > 2.0) {
		is_refractory = false;
	}
	membrane_potential = 0.0;
}

bool Neuron::Update(double sim_time, double input_current) {
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
		membrane_potential = c1 * membrane_potential + input_current * c2; // TODO: Add "+J" part according to ring_buffer
	}

	// Return true iff spike has occurred
	return ret;
}

void Neuron::Receive(double time, double strength) {
	// TODO: Implement ring buffer and receive method
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
