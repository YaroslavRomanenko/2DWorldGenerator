#include "PerlinNoise.h"

#include <cstdlib>
#include <cmath>

void PerlinNoise::Init(int seed) {
    srand(seed);


    permutation.reserve(512);

    for (int i = 0; i < 256; i++) {
        permutation.emplace_back(i);
    }

    Shuffle(permutation);

    for (int i = 0; i < 256; i++) {
        permutation.emplace_back(permutation[i]);
    }
}

double PerlinNoise::Noise(double x, double y)
{
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    glm::vec2 topRight(x - 1.0f, y - 1.0f);
    glm::vec2 topLeft(x, y - 1.0f);
    glm::vec2 bottomRight(x - 1.0f, y);
    glm::vec2 bottomLeft(x, y);

    int valueTopRight = permutation[permutation[xi + 1] + yi + 1];
    int valueTopLeft = permutation[permutation[xi] + yi + 1];
    int valueBottomRight = permutation[permutation[xi + 1] + yi];
    int valueBottomLeft = permutation[permutation[xi] + yi];

    double dotTopRight = glm::dot(topRight, GetConstantVector(valueTopRight));
    double dotTopLeft = glm::dot(topLeft, GetConstantVector(valueTopLeft));
    double dotBottomRight = glm::dot(bottomRight, GetConstantVector(valueBottomRight));
    double dotBottomLeft = glm::dot(bottomLeft, GetConstantVector(valueBottomLeft));

    double u = Fade(x);
    double v = Fade(y);

    double lerpLeft = Lerp(v, dotBottomLeft, dotTopLeft);
    double lerpRight = Lerp(v, dotBottomRight, dotTopRight);

    return Lerp(u, lerpLeft, lerpRight);
}

double PerlinNoise::FractalBrownianMotion(double x, double y, int numOctaves)
{
    double result = 0.0;
    double amplitude = 1.0;
    double frequency = 0.005;

    if (numOctaves > 8) {
        numOctaves = 1;
    }

    for (int octave = 0; octave < numOctaves; octave++) {
        double n = amplitude * Noise(x * frequency, y * frequency);
        result += n;

        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return result;
}

double PerlinNoise::Fade(double t)
{
    return ((6*t - 15)*t + 10) * t * t * t;
}

double PerlinNoise::Lerp(double t, double a, double b)
{
    return a + t * (b - a);
}

double PerlinNoise::Grad(int hash, double x, double y)
{
    int h = hash & 3;

    switch (h)
    {
    case 0: return  x + y;
    case 1: return -x + y;
    case 2: return  x - y;
    case 3: return -x - y;
    default: return 0;
    }
}

void PerlinNoise::Shuffle(std::vector<int>& arrayToShuffle)
{
    for (int i = arrayToShuffle.size() - 1; i > 0; i--) {
        int index = rand() % (i + 1);

        int temp = arrayToShuffle[i];
        arrayToShuffle[i] = arrayToShuffle[index];
        arrayToShuffle[index] = temp;
    }
}

glm::vec2 PerlinNoise::GetConstantVector(int value)
{
    const int hash = value & 3;

    if (hash == 0)
        return glm::vec2(1.0f, 1.0f);
    else if (hash == 1)
        return glm::vec2(-1.0f, 1.0f);
    else if (hash == 2)
        return glm::vec2(-1.0f, -1.0f);
    else
        return glm::vec2(1.0f, -1.0f);
}

std::vector<int> PerlinNoise::permutation;

