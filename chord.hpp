#ifndef CHORD_H
#define CHORD_H

#include <vector>
#include <utility>
#include <string>

#include "json.hpp"

using finger_coordinates = std::vector<std::pair<int, int>>;

enum class Fingers
{
    index = 1,
    middle,
    ring,
    pinky
};

struct Chord
{
    std::string chord_key;
    std::string chord_suffix;
    finger_coordinates points;
};

class ChordInterpreter
{
public:
    ChordInterpreter(nlohmann::json json);

    Chord interpret(std::string chord_string);

private:
    bool verify_string(std::string chord_string);
};

#endif