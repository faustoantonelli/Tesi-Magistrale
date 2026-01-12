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
        // Trova il primo file .tex che contiene \begin{document}
        system("grep -l '\\\\begin{document}' *.tex > main_found.txt");
        ifstream f1("main_found.txt");
        f1 >> file_tex;

        // Trova il primo file .bib disponibile
        system("ls *.bib > bib_found.txt");
        ifstream f2("bib_found.txt");
        f2 >> file_bib;
        
        if(file_tex.empty()) file_tex = "main.tex"; // fallback
        if(file_bib.empty()) file_bib = "refs.bib"; // fallback
    }

    void check_syntax() {
        system(("chktex -q -n1 -n3 '" + file_tex + "' > syntax.txt").c_str());
        risultati.push_back({"SINTASSI", "INFO", "Controlla il log per suggerimenti tipografici."});
    }

    void check_spelling() {
        // Aspell estrae i refusi ignorando i comandi LaTeX
        string cmd = "cat " + file_tex + " | aspell list -t -l it | sort -u > typos.txt";
        system(cmd.c_str());
        
        ifstream f("typos.txt");
        string word, all;
        while(f >> word) all += word + " ";
        risultati.push_back({"ORTOGRAFIA", all.empty() ? "PASS" : "WARN", 
                           all.empty() ? "Nessun errore" : "Refusi trovati: " + all});
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
        ofstream r("index.html");
        r << "<html><body style='font-family:sans-serif; background:#f4f4f4; padding:20px;'>";
        r << "<h1>📝 Report Revisione Tesi</h1><table border='1' style='width:100%; background:white;'>";
        for(auto &res : risultati) {
            string color = (res.stato == "PASS") ? "green" : (res.stato == "FAIL") ? "red" : "orange";
            r << "<tr><td><b>" << res.id << "</b></td><td style='color:" << color << "'>" << res.stato 
              << "</td><td>" << res.dettagli << "</td></tr>";
        }
        r << "</table></body></html>";
    }
};

int main() {
    LatexTester t;
    t.check_syntax();
    t.check_spelling();
    t.check_citations();
    t.genera_report();
    return 0;
}