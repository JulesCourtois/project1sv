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
	static constexpr int DELAY = 15;
	static constexpr int RING_BUFFER_SIZE = 16;
	static constexpr double TIME_STEP = 0.1; // ms

	double J; // mv (0.1 for excitatory, 0.5 for inhibitory)

	bool is_refractory, is_excitatory, has_current;
	double membrane_potential, refractory_time, input_current, exterior_strength;
	std::vector<double> spikes;
	int spike_count, local_clock = 0;
	double c1, c2;
	std::vector<double> ring_buffer; // 16 ints with value 0
	int ring_ind;
	std::vector<Neuron*> connexions;

public:
	Neuron(bool curr, bool exc);

	double GetMembranePotential();

	std::vector<Neuron*> GetConnexions();

	std::vector<Neuron*> GetConnexions();

	bool IsRefractory();
	bool HasCurrent();
	void HandleRefractoryPeriod();
	void SetCurrent(double i);

	void AddConnexion(Neuron* neuron);

	void ReceiveSpike();

	void AddSpike(int sim_time);

	int GetSpikeCount();

	double GetJ();

	std::vector<double> GetSpikes();

	bool Update(int timesteps);

	int GetClock();

	void SetExteriorStrength(double strength);
};
