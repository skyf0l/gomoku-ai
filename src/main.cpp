#include "ppay/ppay_brain.hpp"
#include "random_brain.hpp"

int main(void)
{
    // gmk::BrainCore *brain = new gmk::randbrain::RandomBrain();
    gmk::BrainCore *brain = new gmk::ppay::PPayBrain();

    brain->run();
    delete brain;
    return 0;
}
