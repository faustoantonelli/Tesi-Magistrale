#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct Result { string id, stato, dettagli; };

class LatexTester {
public:
    string file_tex;
    string file_bib;
    vector<Result> risultati;

    void detect_files() {
        system("grep -l '\\\\begin{document}' *.tex > main_found.txt");
        ifstream f1("main_found.txt");
        if (!(f1 >> file_tex)) file_tex = "tesi.tex";

        system("find . -name '*.bib' > bib_found.txt");
        ifstream f2("bib_found.txt");
        if (!(f2 >> file_bib)) file_bib = "bibliography/refs.bib";
    }

    void check_syntax() {
        // Analizziamo tutti i file .tex ricorsivamente per evitare gli "Unable to open"
        system("chktex -q -n1 -n3 *.tex chapters/*.tex > syntax.txt 2>/dev/null");
        risultati.push_back({"SINTASSI", "INFO", "Revisione completata. Controlla sopra per suggerimenti."});
    }

    void check_spelling() {
        // Controllo ortografico su tutti i file della tesi
        system("cat *.tex chapters/*.tex | aspell list -t -l it | sort -u > typos.txt");
        
        ifstream f("typos.txt");
        string word, all_typos;
        while(f >> word) {
            // Ignoriamo parole troppo corte o specifiche
            if(word.length() > 3) all_typos += word + " ";
        } // <--- Graffa aggiunta correttamente

        if (all_typos.empty()) {
            risultati.push_back({"ORTOGRAFIA", "PASS", "Nessun refuso trovato."});
        } else {
            risultati.push_back({"ORTOGRAFIA", "WARN", "Parole da controllare: " + all_typos});
        }
    }

    void check_citations() {
        // Regex semplificata per evitare errori di escape
        string cmd = "grep -rhE '\\\\cite\\{[^\\}]+\\}' . | sed 's/.*{//;s/}.*//' | sort -u > citations.txt";
        system(cmd.c_str());

        ifstream f("citations.txt");
        string key, missing;
        while(f >> key) {
            string check = "grep -q '" + key + "' " + file_bib;
            if (system(check.c_str()) != 0) missing += key + " ";
        }

        if (missing.empty()) {
            risultati.push_back({"CITAZIONI", "PASS", "Tutte le citazioni sono presenti nel file .bib"});
        } else {
            risultati.push_back({"CITAZIONI", "FAIL", "Chiavi non trovate nel .bib: " + missing});
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