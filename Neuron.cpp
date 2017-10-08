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

	c1 = std::exp(-timestep / TAU);
	c2 = R * (1 - c1);
}

void Neuron::AddSpike(int sim_time) {
	is_refractory = true;
	refractory_time = 0.0;
	++spike_count;
	spikes.push_back(sim_time);
}

void Neuron::HandleRefractoryPeriod(double delta) {
	refractory_time += delta;
	if (refractory_time > 2.0) {
		is_refractory = false;
	}
	membrane_potential = 0.0;
}

void Neuron::Update(double sim_time, double delta, double input_current) {
	// Manage refractory period (2 ms)
	if (is_refractory) {
		HandleRefractoryPeriod(delta);
	}
	
	// Manage if spike detected
	else if (membrane_potential > V_THRESHOLD) {
		AddSpike(sim_time);
	}

	// Solve equation
	membrane_potential = c1 * membrane_potential + input_current * c2;
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
