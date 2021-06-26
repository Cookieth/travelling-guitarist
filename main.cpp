#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
#include "chordprogression.hpp"
#include "chord.hpp"

int main(){
    std::ifstream input_file("chords.json");
    nlohmann::json json = nlohmann::json::parse(input_file);
    
    std::string line;
    bool status;

    ChordProgression progression;
    ChordInterpreter interpreter(json);

    while (true)
    {
        if (std::cin.eof())
        {
            break;
        }

        std::getline(std::cin, line);

        if (line == "exit")
        {
            break;
        }

        int pos = line.find(" ");
        std::string cmd = line.substr(0, pos);
        std::string obj = line.substr(pos + 1);

        if (cmd == "add")
        {
            Chord chord = interpreter.interpret(obj);
        }
    }


    return 0;
}