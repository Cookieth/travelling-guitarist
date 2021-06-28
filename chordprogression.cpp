#include <iostream>

#include "chordprogression.hpp"

ChordProgression::ChordProgression()
{
    num_positions = 0;
}

bool ChordProgression::add(Chord chord)
{
    chord_progression.push_back(chord);
    num_positions += chord.positions.size();
    return true;
}

bool ChordProgression::list()
{
    std::string top_line = "";
    std::string bottom_line = "";

    for(std::size_t index = 0; index < chord_progression.size(); ++index)
    {
        Chord chord = chord_progression[index];
        std::string top = std::to_string(index + 1);
        std::string bottom = chord.chord_key + " " + chord.chord_suffix + "  ";

        std::cout << "Num spaces: " << (chord.chord_key.size() + chord.chord_suffix.size()) - top.size() + 3 << std::endl;
        for(std::size_t spaces = 0, top_size = top.size(); spaces < (chord.chord_key.size() + chord.chord_suffix.size()) - top_size + 2; ++spaces)
        {
            top += " ";
        }

        top_line += top;
        bottom_line += bottom;
    }
    
    std::cout << (top_line + "\n" + bottom_line) << std::endl;;
    return true;
}

bool ChordProgression::remove(std::size_t index)
{
    if(index >= chord_progression.size() || index <= 0) return false;
    num_positions -= chord_progression[index - 1].positions.size();
    chord_progression.erase(chord_progression.begin() + index - 1);
    return true;
}

bool ChordProgression::process()
{
    if(!generate_adjacency_matrix()) return false;
}

// Private Helper Functions

bool ChordProgression::generate_adjacency_matrix()
{
    if(chord_progression.size() <= 1) return false;

    for(std::size_t chord_index = 0; chord_index < chord_progression.size(); ++chord_index)
    {
        
    }

    return true;
}