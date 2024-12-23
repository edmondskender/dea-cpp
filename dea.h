//
// Created by edmond on 21.12.24.
//
#pragma once

#include <string>
#include <map>
#include <exception>
#include <functional>
using namespace std;

struct state {
    std::string name;
    std::map<char, state*> transitions;
    bool accepting = false;
};

class dea {
    std::string alpha;      // enthält die Zeichen des Eingabealphabets (z.B. "ab" oder "abc")
    state* states;          // Array aus Zuständen
    int n_states;           // Anzahl der Zustände im Array
    state* start;           // Zeiger auf Startzustand

public:
    dea();
    dea(const dea &other);
    dea(dea &&other);
    ~dea();

    // Lädt den DEA aus einer Datei im angegebenen Format
    void load_from_file(const std::string &path);

    // Sucht einen Zustand im states-Array anhand seines Namens und gibt einen Zeiger zurück
    // oder nullptr, falls der Zustand nicht gefunden wird
    state* find(const std::string &name);

    // Überprüft, ob jeder Zustand für jedes Zeichen aus alpha einen definierten Übergang besitzt
    bool check_states();

    // Operator, der ein Wort auf den DEA anwendet
    // true, wenn Wort akzeptiert wird; sonst false
    bool operator()(const std::string &wort, std::function<void(state*)> f);

    void load_alphabet(const std::string& line);
    void load_states(const std::string &line);
    void assign_start_state(const std::string &line);
    void tag_accepting_states(const std::string &line);
    void single_transitions(const std::string &line);

};

class invalid_description : public std::exception {
    string description;
public:
    explicit invalid_description(const std::string& str) : description(str) {}
};

class invalid_input : public std::exception {
    char c_;
public:
    explicit invalid_input(char c) : c_(c) { }
    // ...
};

