#include <vector>

// Class representing a neuron
class Neuron {

private:
	static constexpr double V_THRESHOLD = 20.0; // mV
	static constexpr double TAU = 20.0; // ms
	static constexpr int REFRACTORY_PERIOD = 20; // timesteps = 2.0ms
	static constexpr double V_RESET = 0.0; // mV or 10.0
	static constexpr double C = 1.0; // Capacity
	static constexpr double R = 20.0; // Gohm
	static constexpr double J = 0.1; // mV
	static constexpr int RING_BUFFER_SIZE = 16; // (Time_step / delay) + 1
	static constexpr double TIME_STEP = 0.1; // ms
	
	bool is_refractory, has_current;
	double membrane_potential, refractory_time;
	std::vector<double> spikes;
	int spike_count;
	double c1, c2;
	double ring_buffer [RING_BUFFER_SIZE] = {}; // Init to 0
	int ring_ind = 0;
	std::vector<Neuron> connections;

public:	
	Neuron(std::vector<Neuron> connections, bool curr);

	double GetMembranePotential();
	
	std::vector<Neuron> GetConnections();

	bool IsRefractory();
	bool HasCurrent();
	void HandleRefractoryPeriod();

	void ReceiveSpike();

	void AddSpike(int sim_time);

	int GetSpikeCount();

	std::vector<double> GetSpikes();

	bool Update(int time, double input_current);
};
