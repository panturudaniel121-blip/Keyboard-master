#include "header/Joc.hpp"
#include <iostream>

int main() {
    const Scor scor_fantoma;
    const Cuvant cuvant_fantoma;//nu o sa afiseze niciun cuvant deoarece inca nu a fost ales cuvant
    //std::cout<<cuvant exista in fisierul in fisierul cuvant.cpp si este afisat in timpul rularii jocului
    std::cout << scor_fantoma;
    std::cout << cuvant_fantoma;
    // am supraincarcat si pentru clasa scor_board facuta pentru scrierea si citirea din fisier si nu am mai apelat-o in main (in fisierul scor.cpp)
    Joc joc;
    joc.ruleaza();
    return 0;
}
