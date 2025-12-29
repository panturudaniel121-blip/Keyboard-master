#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Joc;

enum class DificultateJoc {
    Usor,
    Mediu,
    Greu
};

// --- Clasa de Baza
class Buton {
protected:
    sf::RectangleShape shape;
    sf::Text textButton;
    sf::Color culoareNormala;
    sf::Color culoareHover;

public:
    Buton(const sf::Vector2f& pozitie, const sf::Vector2f& dimensiune,
          const std::string& text, const sf::Font& font,
          sf::Color culoare);

    virtual ~Buton() = default;

    // 1. Constructor Virtual (Clone)
    virtual Buton* clone() const = 0;

    // 2. Functie virtuala pura
    virtual void executaActiune(Joc& joc) = 0;

    // 3. Interfata Non-Virtuala (NVI)
    void afiseaza(sf::RenderWindow& window) const;

    bool esteMouseDeasupra(const sf::Vector2f& mousePos) const;
    void updateCuloare(const sf::Vector2f& mousePos);

    friend std::ostream& operator<<(std::ostream& out, const Buton& b);

protected:
    virtual void doAfiseaza(sf::RenderWindow& window) const;
    virtual void doPrint(std::ostream& out) const;
};

// --- Clase Derivate ---

class ButonIesire : public Buton {
public:
    ButonIesire(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonReset : public Buton {
public:
    ButonReset(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonRestart : public Buton {
public:
    ButonRestart(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonDificultate : public Buton {
    DificultateJoc dificultateAsociata;

public:
    ButonDificultate(const sf::Vector2f& pos, const sf::Font& font, DificultateJoc dif, const std::string& text);

    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonMeniu : public Buton {
public:
    ButonMeniu(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonMute : public Buton {
    bool* refMuted;
public:
    ButonMute(const sf::Vector2f& pos, const sf::Font& font, bool* stareMute);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;

    void doAfiseaza(sf::RenderWindow& window) const override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonStatistici : public Buton {
public:
    ButonStatistici(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonClipboard : public Buton {
public:
    ButonClipboard(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

class ButonInapoi : public Buton {
public:
    ButonInapoi(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;

};

class ButonInfo : public Buton {
public:
    ButonInfo(const sf::Vector2f& pos, const sf::Font& font);
    Buton* clone() const override;
    void executaActiune(Joc& joc) override;
protected:
    void doPrint(std::ostream& out) const override;
};

//*************************************************** Meniu
class Meniu {
    std::vector<Buton*> butoane;

public:
    Meniu() = default;
    ~Meniu();
    Meniu(const Meniu& other);
    Meniu& operator=(Meniu other);

    friend void swap(Meniu& first, Meniu& second) noexcept;

    void adaugaButon(Buton* btn);
    void gestioneazaClick(const sf::Vector2f& mousePos, Joc& joc) const;
    void actualizeazaHover(const sf::Vector2f& mousePos) const;
    void deseneaza(sf::RenderWindow& window) const;
};