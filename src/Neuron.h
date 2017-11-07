#include <vector>
#include <cmath>

/**
* A class representing a single neuron in a network.
*/
class Neuron {

private:

	// declare all neuron constants
	static constexpr double V_THRESHOLD = 20.0; /** Spike threshold in milliVolts. */
	static constexpr double TAU = 20.0; /** Tau */
	static constexpr int REFRACTORY_PERIOD = 20; /** Number of timesteps for refactory period.*/
	static constexpr double V_RESET = 0.0; /** Reset value in milliVolts. */
	static constexpr double C = 1.0; /** Capacity */
	static constexpr double R = 20.0; /** Resistance */
	static constexpr int DELAY = 15; /** Delay for a spike to be received by another neuron. */
	static constexpr int RING_BUFFER_SIZE = 16; /** Delay + 1 */
	static constexpr double TIME_STEP = 0.1; /** Value of a time step in milliseconds. */

	// declare neuron-specific constants
	bool is_excitatory; /** Neuron type, true for excitatory and false for inhibitory. */
	double J; /** In milliVolts, 0.1 for excitatory, 0.5 for inhibitory. */
	double c1 = std::exp(-TIME_STEP/TAU), c2 = R * (1 - c1); /** Constants for decay modellization. */

	// declare unused current variables
	bool has_current; /** Does this neuron receive exterior current ? */
	double input_current = 0.0; /** Store how much current is input in the neuron artificially. */

	// declare refractoring variables
	bool is_refractory = false; /** Used to determine behaviour of neuron. */
	double refractory_time = 0.0; /** Counts how long neuron has been refractoring. */

	// declare spiking variables
	double membrane_potential = 0.0; /** Store the current membrane potential. */
	int spike_count = 0; /** How many spikes have occurred. */

	// declare network variables
	double exterior_strength = 0.0; /** Store the extrapolated potential coming from non-simulated parts of the brain. */
	std::vector<double> ring_buffer; /** Used to store received spikes from other simulated neurons. */
	int ring_ind = 0; /** Index for the ring_buffer. */
	std::vector<int> spikes; /** Times of each spike the neuron has experienced. */
	std::vector<Neuron*> connexions; /** Each neuron that should be sent a signal when this one spikes. */

	int local_clock = 0; /** Store the time the neuron has been simulated up to. */

	void HandleRefractoryPeriod(); /** Private helper function to manage refractory period. */
	void AddSpike(); /** Private helper function to add a spike. */

public:
	/**
	* A constructor for the Neuron.
	* @param current a boolean to determine if neuron receives artificial current.
	* @param excitatory a boolean representing the type of the neuron.
	* @see is_excitatory
	*/
	Neuron(bool current, bool excitatory);

	/**
	* The most important function, update the neuron.
	* @param timesteps the number of timesteps to update for.
	*/
	bool Update(int timesteps);

	/**
	Simple setter for the exterior current.
	@param i exterior current.
	*/
	void SetCurrent(double i);

	/**
	* Simple setter for the extrapolated potential from the rest of the brain.
	* @param strength extrapolated exterior potential.
	*/
	void SetExteriorStrength(double strength);

	/**
	* Add a connexion to an existing neuron going from this to param.
	* @param neuron pointer to the receiver neuron.
	*/
	void AddConnexion(Neuron* neuron);

	/**
	* Used to receive a spike.
	* @param clock the time the spike was sent.
	* @param strength the potential sent by the spike.
	*/
	void ReceiveSpike(int clock, double strength);

	/** Simple getter. */
	std::vector<Neuron*> GetConnexions(); /** Simple getter. */
	int GetClock(); /** Simple getter. */
	double GetMembranePotential(); /** Simple getter. */
	bool IsRefractory(); /** Simple getter. */
	bool HasCurrent(); /** Simple getter. */
	int GetSpikeCount(); /** Simple getter. */
	double GetJ(); /** Simple getter. */
	std::vector<int> GetSpikes();

};
