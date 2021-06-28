#ifndef CHORDPROGRESSION_H
#define CHORDPROGRESSION_H

#include <vector>

#include "chord.hpp"

class ChordProgression
{
public:
    ChordProgression();

    bool add(Chord chord);
    bool list();
    bool remove(std::size_t index);

    bool process();

private:
    bool generate_adjacency_matrix();

    std::vector<Chord> chord_progression;
    std::size_t num_positions;
    std::vector<std::vector<double>> adjacency_matrix;
};

#endif