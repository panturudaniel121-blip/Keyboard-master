#include "header/Joc.hpp"
#include "header/Exceptii.hpp"
#include <iostream>

int main() {
    try {
        Joc joc;
        std::cout << joc;
        joc.ruleaza();
    }
    catch (const EroareResursaCritica& e) {
        std::cerr << "[CRITIC] " << e.what() << "\nVerificati folderul 'Date' sau 'fonts'.\n";
        return 1;
    }
    catch (const EroareDateInvalide& e) {
        std::cerr << "[DATE] " << e.what() << "\nFisierele de cuvinte sunt goale?\n";
        return 2;
    }
    catch (const EroareInitializare& e) {
        std::cerr << "[SFML] " << e.what() << "\n";
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Eroare neasteptata: " << e.what() << "\n";
        return -1;
    }
    return 0;
}