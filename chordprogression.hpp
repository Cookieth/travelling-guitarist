#ifndef CHORDPROGRESSION_H
#define CHORDPROGRESSION_H

#include <vector>
#include <utility>

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
    double get_total_distance(finger_coordinates source_position, finger_coordinates dest_position);

    std::vector<Chord> chord_progression;
    std::size_t num_positions;
    std::vector<std::vector<double>> adjacency_matrix;
    std::unordered_map<int, std::pair<std::size_t, std::size_t>> adjacency_reference;
    std::vector<std::pair<std::size_t, std::size_t>> shortest_path;
};

#endif