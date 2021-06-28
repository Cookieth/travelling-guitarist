#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
#include "chordprogression.hpp"
#include "chord.hpp"

int main()
{
    std::string line;
    bool status;

    std::ifstream input_file("chords.json");
    nlohmann::json json = nlohmann::json::parse(input_file);
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

            if(chord.is_valid == false)
            {
                std::cout << "Invalid input." << std::endl;
            }

            if(!progression.add(chord))
            {
                std::cout << "Could not add chord." << std::endl;
            }
        }

        if (cmd == "list")
        {
            progression.list();
        }

        if (cmd == "erase")
        {
            
            if(!progression.remove(std::atoi(obj.data())))
            {
                std::cout << "Index out of range." << std::endl;
            }
        }

        if (cmd == "process")
        {
            if(!progression.process())
            {
                std::cout << "Could not generate shortest path." << std::endl;
            }
        }
    }

    return 0;
}