#include <vector>

// Class representing a neuron
class Neuron {

private:
	bool is_refractory;
	double membrane_potential, refractory_time;
	std::vector<double> spikes;
	int spike_count;
	double c1, c2, h;

public:
	const double V_THRESHOLD = 20.0; // mV
	const double TAU = 20.0; // ms
	const double REFRACTORY_PERIOD = 2.0; // ms
	const double V_RESET = 0.0; // mV or 10.0
	const double C = 1.0; // Capacity
	const double R = C * TAU; // Resistance = 20.0

	Neuron(double timestep);

	double GetMembranePotential();

	bool IsRefractory();
	void HandleRefractoryPeriod();

	void Receive(double time, double strength);

	void AddSpike(int sim_time);

	int GetSpikeCount();

	std::vector<double> GetSpikes();

	bool Update(double time, double input_current);
};
