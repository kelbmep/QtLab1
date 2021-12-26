#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>

class Generator
{
public:
    Generator() { }

    static void generate(std::vector<double>& points, double maxX, double maxY);
    static void generate(std::vector<double>& points);
};

#endif // GENERATOR_H
