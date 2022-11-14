#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>

class FiniteAutomata{

private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> graph;
    std::vector<std::string> states;
    std::vector<std::string> final_states;
    std::string init_state;

    FiniteAutomata(
        std::vector<std::string> _states,
        std::vector<std::string> _final_states,
        std::string _init_state,
        std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> g
        ):  graph(g),
            states(_states),
            final_states(_final_states),
            init_state(_init_state)
        {}

    static std::vector<std::string> tokenize(std::string const &str, const char delim){
        size_t start;
        size_t end = 0;
        std::vector<std::string> out;
        while ((start = str.find_first_not_of(delim, end)) != std::string::npos){
            end = str.find(delim, start);
            out.push_back(str.substr(start, end - start));
        }
        return out;
    }

public:

    bool matches(std::string token){
        std::string current_state = this->init_state;
        for(auto chr : token){
            auto possible_transitions = graph.at(current_state);
            bool found = false;
            for(auto transition: possible_transitions){
                if(transition.second[0] == chr){
                    found = true;
                    current_state = transition.first;
                    break;
                }
            }
            if(!found){
                return false;
            }
        }
        return std::find(final_states.begin(), final_states.end(), current_state) != final_states.end();
    }

    static FiniteAutomata buildFiniteAutomata(std::string input_file_path){
        std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> fa_graph;
        // Read file to build FA graph
        std::ifstream file(input_file_path);
        std::string line;

        // Read all states from first line
        std::getline(file, line);
        std::vector<std::string> states = tokenize(line, ' ');
        for(auto state : states){
            fa_graph.insert({state, std::vector<std::pair<std::string, std::string>>()});
        }

        // Read initial state
        std::getline(file, line);
        std::string init_state = line;
        
        // Read final states
        std::getline(file, line);
        std::vector<std::string> final_states = tokenize(line, ' ');
        
        // Read transitions
        while(std::getline(file, line)){
            auto transition = tokenize(line, ',');
            auto start = transition[0];
            auto end = transition[1];
            auto value = transition[2];
            fa_graph.at(start).push_back({end, value});
        }
        return FiniteAutomata(states, final_states, init_state, fa_graph);
    }
};