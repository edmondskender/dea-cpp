#include <iostream>
#include <string>
#include "dea.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cerr << "Usage: " << argv[0] << " <dea_file> <word>" << endl;
    }

    string filename(argv[1]);
    string word(argv[2]);

    dea automat;
    try {
        //1. Automaten aus Datei laden
        automat.load_from_file(filename);

        //2. Wort testen
        auto lambda = [](state* current_state) { cout << current_state->name << endl; };
        bool accepted = automat(word, lambda);

        if(accepted) {
            cout << "Das Wort \"" << word << "\" wird akzeptiert." << endl;
        } else {
            cout << "Das Wort \"" << word << "\" wird NICHT akzeptiert." << endl;
        }
    }
    catch (const invalid_description& e) {
        cerr << "Fehler bei der DEA-Beschreibung: " << e.what() << endl;
    }
    catch (const invalid_input& e) {
        cerr << "Fehler: Ungültiges Eingabezeichen: " << e.what() << endl;
    }

    return 0;
}
