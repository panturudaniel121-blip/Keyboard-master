#pragma once
#include <exception>
#include <string>

class EroareJoc : public std::exception {
protected:
    std::string mesaj;
public:
    explicit EroareJoc(const std::string& msg) : mesaj("Eroare Joc: " + msg) {}

    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class EroareResursaCritica : public EroareJoc {
public:
    explicit EroareResursaCritica(const std::string& numeFisier)
        : EroareJoc("Fisier critic lipsa sau inaccesibil: " + numeFisier) {}
};

class EroareDateInvalide : public EroareJoc {
public:
    explicit EroareDateInvalide(const std::string& detaliu)
        : EroareJoc("Date invalide detectate: " + detaliu) {}
};

class EroareInitializare : public EroareJoc {
public:
    explicit EroareInitializare(const std::string& componenta)
        : EroareJoc("Nu s-a putut initializa componenta grafica: " + componenta) {}
};