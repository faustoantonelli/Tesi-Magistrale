#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct Result { string id, stato, dettagli; };

class LatexTester {
public:
    string file_tex = "tesi.tex";
    string file_bib = "refs.bib";
    vector<Result> risultati;

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
        // Estrae le chiavi dai \cite{...} e verifica se sono nel file .bib
        string cmd = "grep -oP '\\\\cite\\{\\K[^\\}]+' " + file_tex + " | sort -u > citations.txt";
        system(cmd.c_str());

        ifstream f("citations.txt");
        string key, missing;
        while(f >> key) {
            string check = "grep -q '" + key + "' " + file_bib;
            if (system(check.c_str()) != 0) missing += key + " ";
        }
        risultati.push_back({"CITAZIONI", missing.empty() ? "PASS" : "FAIL", 
                           missing.empty() ? "Tutte le citazioni presenti" : "Mancano nel .bib: " + missing});
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