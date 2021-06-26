#ifndef CHORDPROGRESSION_H
#define CHORDPROGRESSION_H

#include <vector>

#include "chord.hpp"

class ChordProgression
{
public:
    ChordProgression();

    bool add(std::string chord);

    bool process();

private:
    std::vector<Chord> chord_progression;
};

#endif