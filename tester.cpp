#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct Result { string id, stato, dettagli; };

class LatexTester {
public:
    // Rimuoviamo i nomi fissi e cerchiamo i file dinamicamente
    string file_tex;
    string file_bib;
    vector<Result> risultati;

    void detect_files() {
    system("grep -l '\\\\begin{document}' *.tex > main_found.txt");
    ifstream f1("main_found.txt");
    if (!(f1 >> file_tex)) file_tex = "tesi.tex"; // Forza il nome se grep fallisce

    system("find . -name '*.bib' > bib_found.txt"); // Cerca anche in sottocartelle
    ifstream f2("bib_found.txt");
    if (!(f2 >> file_bib)) file_bib = "bibliography/refs.bib";
}

    void check_syntax() {
        system(("chktex -q -n1 -n3 '" + file_tex + "' > syntax.txt").c_str());
        risultati.push_back({"SINTASSI", "INFO", "Controlla il log per suggerimenti tipografici."});
    }

    void check_spelling() {
        string cmd = "cat " + file_tex + " | aspell list -t -l it | sort -u > typos.txt";
        system(cmd.c_str());
        
        ifstream f("typos.txt");
        string word, all_typos;
        while(f >> word) {
            all_typos += word + " ";
        } 

        if (all_typos.empty()) {
            risultati.push_back({"ORTOGRAFIA", "PASS", "Nessun refuso trovato."});
        } else {
            risultati.push_back({"ORTOGRAFIA", "WARN", "Possibili refusi: " + all_typos});
        }
    }

    void check_citations() {
    // 1. Verifica se il file .bib esiste
    ifstream check_file(file_bib);
    if(!check_file) {
        risultati.push_back({"CITAZIONI", "WARN", "File " + file_bib + " non trovato."});
        return;
    }

    // 2. Estrazione chiavi: nota le 4 barre per gestire l'escape di C++ e Grep
    // Questo comando cerca \cite{chiave} e scrive 'chiave' in citations.txt
    string cmd = "grep -oP '\\\\\\\\cite\\\\{\\\\K[^\\\\}]+' " + file_tex + " | sort -u > citations.txt";
    system(cmd.c_str());

    ifstream f("citations.txt");
    string key, missing;
    
    // 3. Verifica ogni chiave dentro il file .bib
    while(f >> key) {
        // Cerchiamo la chiave nel file .bib
        string check = "grep -q '" + key + "' " + file_bib;
        if (system(check.c_str()) != 0) {
            missing += key + " ";
        }
    }

    // 4. Risultato finale
    if (missing.empty()) {
        // Se citations.txt è vuoto, f >> key non parte e missing resta vuoto (PASS)
        risultati.push_back({"CITAZIONI", "PASS", "Tutte le citazioni sono presenti nel file .bib"});
    } else {
        risultati.push_back({"CITAZIONI", "FAIL", "Chiavi mancanti nel .bib: " + missing});
    }
}

    void genera_report() {
    cout << "\n" << string(60, '=') << endl;
    cout << "           RISULTATI REVISIONE TESI" << endl;
    cout << string(60, '=') << endl;
    for(auto &res : risultati) {
        string icona = (res.stato == "PASS") ? "✅" : (res.stato == "FAIL") ? "❌" : "⚠️";
        cout << icona << " [" << res.id << "] " << res.stato << endl;
        cout << "   Dettagli: " << res.dettagli << endl;
        cout << string(60, '-') << endl;
    }
}
};

int main() {
    LatexTester t;
    t.detect_files();
    t.check_syntax();
    t.check_spelling();
    t.check_citations();
    t.genera_report();
    return 0;
}