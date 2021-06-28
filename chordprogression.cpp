#include <iostream>
#include <math.h>
#include <limits>

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

        for(std::size_t spaces = 0, top_size = top.size(); spaces < (chord.chord_key.size() + chord.chord_suffix.size()) - top_size + 3; ++spaces)
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
    if(index > chord_progression.size() || index <= 0) return false;
    num_positions -= chord_progression[index - 1].positions.size();
    chord_progression.erase(chord_progression.begin() + index - 1);
    return true;
}

bool ChordProgression::process()
{
    if(!generate_adjacency_matrix()) return false;

    // DEBUG: Print adjacency matrix
    for(std::size_t i = 0; i < adjacency_matrix.size(); ++i)
    {
        for(std::size_t j = 0; j < adjacency_matrix[i].size(); ++j)
        {
            std::cout << adjacency_matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    // Keep track of the distance from the source node to all other nodes
    std::vector<double> total_distance(num_positions, std::numeric_limits<double>::max());

    // Keep track of the shortest path to any node.
    std::vector<std::size_t> parent_index(num_positions, -1);

    // Keep track of the total weight of the path from the source node to all other nodes
    std::vector<bool> positions_visited(num_positions, false);

    total_distance[0] = 0;

    for(std::size_t position_index = 0; position_index < num_positions - 1; ++position_index)
    {
        // Find the position with the shortest distance
        int min_index = 0;
        int min_distance = std::numeric_limits<double>::max();
        for (int index = 0; index < total_distance.size(); index++)
        {
            if(total_distance[index] <= min_distance && positions_visited[index] == false)
            {
                min_distance = total_distance[index];
                min_index = index;
            }
        }

        // We choose to "visit" this minimum position
        positions_visited[min_index] = true;

        // Update all adjacent vertices
        for(std::size_t adjacent_index = 0; adjacent_index < num_positions; ++adjacent_index)
        {
            if(positions_visited[adjacent_index] == false && // Only update for unvisited nodes
                adjacency_matrix[min_index][adjacent_index] != 0 && // Only update for valid nodes (path exists)
                total_distance[min_index] != std::numeric_limits<double>::max() && // Only update if the source node is valid
                total_distance[min_distance] + adjacency_matrix[min_index][adjacent_index] < total_distance[adjacent_index]) // Update the minimum
            {
                parent_index[adjacent_index] = min_index;
                total_distance[adjacent_index] = total_distance[min_distance] + adjacency_matrix[min_index][adjacent_index];
            }
        }
    }


    // Interpret the shortest path
    
    // Find out how many of the last indices belong to the last chord
    std::size_t num_final_positions = chord_progression.back().positions.size();

    std::size_t shortest_path_index = 0;
    double shortest_path_weight = std::numeric_limits<double>::max();

    // Get the shortest index
    for(std::size_t index = 0; index < num_final_positions; ++index)
    {
        if(total_distance[num_positions - 1 - index] <= shortest_path_weight)
        {
            shortest_path_weight = total_distance[num_positions - 1 - index];
            shortest_path_index = index;
        }
    }

    // Draw out the shortest path
    std::vector<finger_coordinates*> shortest_path;
    std::size_t cursor_index = shortest_path_index;
    while(parent_index[shortest_path_index] != -1)
    {
        shortest_path.emplace_back(adjacency_reference[cursor_index]);
        cursor_index = parent_index[cursor_index];
    }

    // Print the shortest path (improve on this in helper function)
    for(std::size_t index = 0; index < shortest_path.size(); ++index)
    {
        finger_coordinates* position = shortest_path[index];

        std::cout << index << ": ";
        for(std::size_t finger_index = 1; finger_index < 5; ++finger_index)
        {
            std::cout << "(" << position->at(finger_index).first << ", " << position->at(finger_index).second << ") --> ";
        }
    }
    std::cout << std::endl;

    return true;
}

// Private Helper Functions

bool ChordProgression::generate_adjacency_matrix()
{
    if(chord_progression.size() <= 1) return false;

    adjacency_matrix.clear();
    adjacency_reference.clear();

    adjacency_matrix = std::vector<std::vector<double>>(num_positions, std::vector<double>(num_positions, 0.0));

    std::size_t source_node_index = 0;
    std::size_t dest_base_index = 0;
    
    // Iterate through all chords, and populate the adjacency matrix
    for(std::size_t chord_index = 0; chord_index < chord_progression.size() - 1; ++chord_index)
    {
        Chord source_chord = chord_progression[chord_index];
        Chord dest_chord = chord_progression[chord_index + 1];

        dest_base_index += source_chord.positions.size();

        // Iterate through all the positions at the current position, calculate distance to next position
        for(std::size_t source_position_index = 0; source_position_index < source_chord.positions.size(); ++source_position_index)
        {
            for(std::size_t dest_position_index = 0; dest_position_index < dest_chord.positions.size(); ++dest_position_index)
            {
                double distance = get_total_distance(source_chord.positions[source_position_index], dest_chord.positions[dest_position_index]);
                adjacency_matrix[source_node_index][dest_base_index + dest_position_index] = distance;
                adjacency_reference[source_node_index] = &(source_chord.positions[source_position_index]);
            }
            ++source_node_index;
        }
    }

    return true;
}

double ChordProgression::get_total_distance(finger_coordinates source_position, finger_coordinates dest_position)
{
    double total_distance = 0.0;

    std::cout << "total distance" << std::endl;

    for(int finger_index = 1; finger_index < 5; finger_index++)
    {
        if(dest_position[finger_index].first == 0 || source_position[finger_index].first == 0
            || dest_position[finger_index].second == 0 || source_position[finger_index].second == 0) continue;

        int x_distance = dest_position[finger_index].first - source_position[finger_index].first;
        int y_distance = dest_position[finger_index].second - source_position[finger_index].second;
        total_distance += sqrt((x_distance * x_distance) + (y_distance * y_distance));
    }

    return total_distance;
}