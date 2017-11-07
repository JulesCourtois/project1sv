#include "Neuron.h"
#include "gtest/gtest.h"

namespace {

// Test whether single neuron ever spikes with I <= 1.0
TEST(one_neuron, current_ls_one) {
    Neuron* single = new Neuron(true, true);
    single->SetCurrent(0.9);
    EXPECT_EQ(false, single->Update(500));
}

// Test whether single neuron ever spikes with I = 1.1
TEST(one_neuron, current_gt_one) {
    Neuron* single = new Neuron(true, true);
    single->SetCurrent(1.1);
    EXPECT_EQ(true, single->Update(500));
}

// Test whether single neuron goes into refractory period for the right amount of time
TEST(one_neuron, refractory) {
    Neuron* single = new Neuron(true, true);
    single->SetCurrent(1000);
    EXPECT_EQ(true, single->Update(2));
    single->SetCurrent(0);
    EXPECT_EQ(false, single->Update(1));
    EXPECT_EQ(true, single->IsRefractory());
    single->Update(20);
    EXPECT_EQ(false, single->IsRefractory());
}

// Test whether two neurons send spikes to each other
TEST(two_neurons, current_gt_one) {
    Neuron* one = new Neuron(true, true);
    Neuron* two = new Neuron(false, true);
    one->AddConnexion(two);
    one->SetCurrent(1000);
    if (one->Update(2)) {
        one->GetConnexions()[0]->ReceiveSpike(one->GetClock(), 0.5);
    }
    two->Update(17);
    EXPECT_EQ(one->GetSpikeCount(), 1);
    EXPECT_GT(two->GetMembranePotential(), 0.0);
}

}
