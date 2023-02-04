#include "ContentRand.h"
#include <random>
#include <time.h>

class RandInitClass
{
public:
    RandInitClass()
    {
        srand(static_cast<int>(time(NULL)));
    }

    ~RandInitClass()
    {}
};

RandInitClass RandInit = RandInitClass();

int ContentRand::RandInt(int _Min, int _Max)
{
    return _Min + (rand() % (_Max - _Min));
}

float ContentRand::RandFloat(float _Min, float _Max)
{
    return  _Min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_Max - _Min)));
}

ContentRand::ContentRand()
{
}

ContentRand::~ContentRand()
{
}
