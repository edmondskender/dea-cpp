//
// Created by edmond on 21.12.24.
//
#include "dea.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using iss = std::istringstream;

state* dea::find(const std::string &name) {
    for(int i = 0; i < n_states; ++i) {
        if(states[i].name == name)
            return states + i;
    }
    return nullptr;
}

void dea::load_alphabet(const std::string& line) {
    iss is(line);
    char c;
    while(is >> c)
        alpha += c;
}

void dea::load_states(const std::string &line) {
    iss is(line);
    string state;
    vector<string> state_vector;
    while(is >> state)
        state_vector.push_back(state);

    n_states = state_vector.size();

    states = new struct state[n_states];

    for(int i = 0; i < n_states; ++i)
        states[i].name = state_vector[i];

    //Oder so:
    /*
    iss is(line);
    int count_states = 0;
    std::string state;

    while(is >> state) {
        count_states++;
    }

    n_states = count_states;
    states = new struct state[n_states];

    is.clear();
    is.seekg(0,ios::beg);

    for(int i = 0; i < count_states; ++i) {
        is >> state;
        states[i].name = state;
    }
    */
}

void dea::assign_start_state(const std::string &line) {
    for(int i = 0; i < n_states; ++i) {
        if(states[i].name == line) {
            start = states + i;
            return;
        }
    }
    throw invalid_description("Startzustand '" + line + "' wurde nicht gefunden.");
}

void dea::tag_accepting_states(const std::string &line) {
    iss is(line);
    string accepting_state;
    bool found;

    while(is >> accepting_state) {
        found = false;
        for(int i = 0; i < n_states; ++i) {
            if(accepting_state == states[i].name) {
                states[i].accepting = true;
                found = true;
                break;
            }
        }
        if(!found) {
            throw invalid_description("Akzeptierender Zustand '" + accepting_state + "' ist nicht definiert");
        }
    }
}

void dea::single_transitions(const std::string &line) {
    iss is(line);
    string current_state, follow_state;
    char input_sign;

    if(!(is >> current_state >> input_sign >> follow_state)) {
        throw invalid_description("Ungültiger Übergange '" + line + "'. Erwartet <current_state> <input_sign> <follow_state> ");
    }

    state* current = find(current_state);
    state* follow = find(follow_state);

    if(!current) {
        throw invalid_description("Aktueller Zustand '" + current_state + "' nicht gefunden");
    }

    if(!follow) {
        throw invalid_description("Folgezustand '" + follow_state + "' nicht gefunden");
    }

    if(alpha.find(input_sign) == string::npos)
        throw invalid_input(input_sign);

    current->transitions[input_sign] = follow;
}

void read_line_and_check_error(std::ifstream &ifs, std::string &line) {
    if (!getline(ifs, line)) {
        throw invalid_description("Fehler beim Einlesen");
    }
}

bool dea::check_states() {
    for(int i = 0; i < n_states; ++i) {
        for(char c : alpha) {
            if(states[i].transitions.find(c) == states[i].transitions.end())
                return false;
        }
    }
    return true;
}

void dea::load_from_file(const std::string &path) {
    ifstream ifs(path);
    if(!ifs.is_open())
        throw invalid_description("Datei konnte nicht geöffnet werden");

    string line;

    //1. Alphabet einlesen
    read_line_and_check_error(ifs, line); load_alphabet(line);

    //2. Zustände einlesen
    read_line_and_check_error(ifs, line); load_states(line);

    //3. Start Zustand einlesen
    read_line_and_check_error(ifs, line); assign_start_state(line);

    //4. Akzeptierende Zustände einlesen
    read_line_and_check_error(ifs,line); tag_accepting_states(line);

    //5. Transitions
    while(getline(ifs, line)) {
        single_transitions(line);
    }

    if(!check_states())
        throw invalid_description("DEA ist unvollständig oder inkonsistent definiert worden");
}

bool dea::operator()(const std::string &wort, std::function<void(state*)> f) {
    state* current = start;

    for(char c : wort) {
        f(current);
        if(alpha.find(c) == string::npos)
            throw invalid_input(c);

        const auto it = current->transitions.find(c);
        current = it->second;
    }
    f(current);


    return current->accepting;
}

dea::dea() : alpha(""), states(nullptr), n_states(0), start(nullptr) {}

dea::~dea() {
    delete[] states;
}

dea::dea(dea &&other) : alpha(std::move(other.alpha)), n_states(other.n_states) {
    start = other.start;
    states = other.states;

    other.start = nullptr;
    other.states = nullptr;
}

dea::dea(const dea& other) : alpha(other.alpha), n_states(other.n_states) {
    states = new state[n_states];

    for(int i = 0; i < n_states; ++i) {
        states[i].name = other.states[i].name;
        states[i].accepting = other.states[i].accepting;
    }

    for(int i = 0; i < n_states; ++i) {
        for(const auto& [c, target_state] : other.states[i].transitions) {
            auto diff = target_state - other.states;
            states[i].transitions[c] = states + diff;
        }
    }


    /*
    for(int i = 0; i < n_states; ++i) {
        for(const auto& [c, target_state] : other.states[i].transitions) {
            string target_name = target_state->name;
            states[i].transitions[c] = find(target_name);
        }
    }
    */

    string start_name = other.start->name;
    start = find(start_name);
}



