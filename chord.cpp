#include <iostream>
#include <utility>

#include "chord.hpp"

ChordInterpreter::ChordInterpreter(nlohmann::json json)
{
    chords_json = json;
}

Chord ChordInterpreter::interpret(std::string chord_string)
{
    // Syntax "Chord:Suffix"
    if(!verify_string(chord_string)) return Chord{false};

    int pos = chord_string.find(":");
    std::string chord = chord_string.substr(0, pos);
    std::string suffix = chord_string.substr(pos + 1);

    nlohmann::json chord_info = chords_json["chords"][chord];
    nlohmann::json chord_positions;
    
    // Look for the particular suffix
    for(std::size_t index = 0; index < chord_info.size(); ++index)
    {
        if(chord_info[index]["suffix"] == suffix)
        {
            chord_positions = chord_info[index]["positions"];
        }
    }

    Chord translated_chord{true, chord, suffix};

    // Get all the positions, and convert into x/y coordinate system
    for(std::size_t index = 0; index < chord_positions.size(); ++index)
    {
        nlohmann::json position = chord_positions[index];
        if(position.contains("capo")) continue;

        translated_chord.positions.push_back(std::vector<std::pair<int, int>>(5));

        nlohmann::json fingers = chord_positions[index]["fingers"];
        nlohmann::json frets = chord_positions[index]["frets"];
        int finger_strings[5] = {0, 0, 0, 0, 0};
        int finger_frets[5] = {-1, -1, -1, -1, -1};

        // A lot of the following seems a bit complicated, and this is for two reasons:
        //  The strings are 1-based, and in reverse order (I want to adhere to guitar music notation)
        //  The frets are 1-based
        std::cout << "Strings" << std::endl;
        // To take barre chords into consideration, we iterate from the end of the array, and take the "highest" string as the location
        for(std::size_t string_index = fingers.size() - 1; string_index < fingers.size(); --string_index)
        {
            // Skip all 0's (counted as invalid)
            if(fingers[string_index] == 0) continue;
            // Explanation:
            //  6 - string_index represents the string
            //  fingers[string_index] represents the finger being used at that string
            //  finger_strings thus stores a map of [finger] -> [string]
            std::cout << fingers[string_index] << " -> " << 6 - string_index << std::endl;
            finger_strings[fingers[string_index]] = 6 - string_index;
        }

        std::cout << "Frets" << std::endl; 
        // Now we find the frets for respective strings
        for(std::size_t finger_index = 0; finger_index < 5; ++finger_index)
        {
            if(finger_strings[finger_index] == 0) continue;
            int string = finger_strings[finger_index];
            // Explanation:
            //  finger_strings[finger_index] gets the string for the respective finger(- 1 for 0-based)
            //  frets[...] gets the fret at that specific string
            //  finger_frets keeps track of these for each finger (instead of per string)
            //  Thus, this stores the map [finger] -> [fret]
            std::cout << finger_index << " -> " << frets[5 - (string - 1)] << std::endl;
            finger_frets[finger_index] = frets[6 - string];
        }

        // Now enter this into the struct we created
        for(std::size_t finger_index = 0; finger_index < 5; ++finger_index)
        {
            if(finger_strings[finger_index] == 0) continue;
            if(finger_frets[finger_index] == -1) continue;
            std::cout << "Pair: " << finger_frets[finger_index] << ", " << finger_strings[finger_index] << std::endl;
            translated_chord.positions[translated_chord.positions.size() - 1][finger_index] = std::make_pair(finger_frets[finger_index], finger_strings[finger_index]);
        }
    }

    std::cout << "Total positions: " << translated_chord.positions.size() << std::endl;

    return translated_chord;
}

bool ChordInterpreter::verify_string(std::string chord_string)
{
    if(chord_string.find(":") == std::string::npos) return false;
    int pos = chord_string.find(":");
    
    // Case-sensitive for easy indexing
    //  A bit unsure how to go from case insensitive -> case sensitive keys
    std::string chord = chord_string.substr(0, pos);
    std::string suffix = chord_string.substr(pos + 1);

    std::vector<std::string> chords = chords_json["keys"];
    if(std::find(chords.begin(), chords.end(), chord) == chords.end())
    {
        std::cout << "Invalid chord. Valid chords are: " << chords_json["keys"] << std::endl;
        return false;
    }

    std::vector<std::string> suffixes = chords_json["suffixes"];
    if(std::find(suffixes.begin(), suffixes.end(), suffix) == suffixes.end())
    {
        std::cout << "Invalid suffix. Valid suffixes are: " << chords_json["suffixes"] << std::endl;
        return false;
    }

    return true;
}