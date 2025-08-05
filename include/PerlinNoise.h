#pragma once

#include <glm/glm.hpp>

#include <vector>

class PerlinNoise {
public:
    static void Init(int seed);
    static double Noise(double x, double y);
    static double FractalBrownianMotion(double x, double y, int numOctaves, double amplitude = 1.0, double frequency = 0.005);

private:
    PerlinNoise() { }

    static double Fade(double t);
    static double Lerp(double t, double a, double b);
    static double Grad(int hash, double x, double y);
    static void Shuffle(std::vector<int>& arrayToShuffle);
    static glm::vec2 GetConstantVector(int value);

    static std::vector<int> permutation;
};