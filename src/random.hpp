#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <ctime>

static boost::random::mt19937 gen(time(0));

static int randrange(int a, int b) {
    boost::random::uniform_int_distribution<> dist(a, b);
    return dist(gen);
}
