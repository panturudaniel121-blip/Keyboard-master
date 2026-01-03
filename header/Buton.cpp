    #include "Buton.hpp"
    #include "Joc.hpp"
    #include <algorithm>
    #include <iostream>

    Buton::Buton(const sf::Vector2f& pozitie, const sf::Vector2f& dimensiune,
                 const std::string& text, const sf::Font& font, sf::Color culoare)
    : shape(dimensiune),textButton(font, text, 20),culoareNormala(culoare)

    {
        shape.setPosition(pozitie);
        shape.setFillColor(culoare);

        culoareHover = culoare;
        culoareHover.r = std::min(255, culoare.r + 40);
        culoareHover.g = std::min(255, culoare.g + 40);
        culoareHover.b = std::min(255, culoare.b + 40);

        textButton.setFillColor(sf::Color::White);

        sf::FloatRect bounds = textButton.getLocalBounds();
        textButton.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f,
            bounds.position.y + bounds.size.y / 2.0f
        });
        textButton.setPosition({
            pozitie.x + dimensiune.x / 2.0f,
            pozitie.y + dimensiune.y / 2.0f
        });
    }

    void Buton::afiseaza(sf::RenderWindow& window) const {
        doAfiseaza(window);
    }

    void Buton::doAfiseaza(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(textButton);
    }

    bool Buton::esteMouseDeasupra(const sf::Vector2f& mousePos) const {
        return shape.getGlobalBounds().contains(mousePos);
    }

    void Buton::updateCuloare(const sf::Vector2f& mousePos) {
        if (esteMouseDeasupra(mousePos)) {
            shape.setFillColor(culoareHover);
        } else {
            shape.setFillColor(culoareNormala);
        }
    }

    std::ostream& operator<<(std::ostream& out, const Buton& b) {
        b.doPrint(out);
        return out;
    }

    void Buton::doPrint(std::ostream& out) const {
        out << "Buton Generic";
    }

    ButonIesire::ButonIesire(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {120.f, 50.f}, "Iesire", font, sf::Color(200, 50, 50)) {}

    Buton* ButonIesire::clone() const { return new ButonIesire(*this); }

    void ButonIesire::executaActiune(Joc& joc) {
        joc.getWindow().close();
    }

    void ButonIesire::doPrint(std::ostream& out) const { out << "[Buton Iesire]"; }

    ButonReset::ButonReset(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {140.f, 50.f}, "Reset Top", font, sf::Color(50, 50, 200)) {}

    Buton* ButonReset::clone() const { return new ButonReset(*this); }

    void ButonReset::executaActiune(Joc& joc) {
        joc.reseteazaClasament();
    }

    void ButonReset::doPrint(std::ostream& out) const { out << "[Buton Reset]"; }

    ButonRestart::ButonRestart(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {140.f, 50.f}, "Joaca Iar", font, sf::Color(50, 200, 50)) {}

    Buton* ButonRestart::clone() const { return new ButonRestart(*this); }

    void ButonRestart::executaActiune(Joc& joc) {
        joc.restartJoc();
    }

    void ButonRestart::doPrint(std::ostream& out) const { out << "[Buton Restart]"; }

    ButonDificultate::ButonDificultate(const sf::Vector2f& pos, const sf::Font& font,
                                       DificultateJoc dif, const std::string& text)
        : Buton(pos, {200.f, 60.f}, text, font, sf::Color(100, 100, 100)),
          dificultateAsociata(dif)
    {
        if (dif == DificultateJoc::Usor) culoareNormala = sf::Color(50, 200, 50);
        else if (dif == DificultateJoc::Endless) culoareNormala = sf::Color(33, 33, 33);
        else if (dif == DificultateJoc::Mediu) culoareNormala = sf::Color(200, 200, 50);
        else culoareNormala = sf::Color(200, 50, 50);

        shape.setFillColor(culoareNormala);

        culoareHover = culoareNormala;
        culoareHover.r = std::min(255, culoareNormala.r + 40);
        culoareHover.g = std::min(255, culoareNormala.g + 40);
        culoareHover.b = std::min(255, culoareNormala.b + 40);
    }

    Buton* ButonDificultate::clone() const { return new ButonDificultate(*this); }

    void ButonDificultate::executaActiune(Joc& joc) {
        joc.setDificultate(dificultateAsociata);
        joc.incepeJoc();
    }

    void ButonDificultate::doPrint(std::ostream& out) const {
        out << "[Dificultate: " << static_cast<int>(dificultateAsociata) << "]";
    }

    ButonMeniu::ButonMeniu(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {160.f, 50.f}, "Meniu", font, sf::Color(100, 100, 200))
    {}

    Buton* ButonMeniu::clone() const { return new ButonMeniu(*this); }

    void ButonMeniu::executaActiune(Joc& joc) {
        joc.mergiLaMeniu();
    }

    void ButonMeniu::doPrint(std::ostream& out) const { out << "[Buton Meniu]"; }
    ButonMute::ButonMute(const sf::Vector2f& pos, const sf::Font& font, bool* stareMute)
        : Buton(pos, {100.f, 40.f}, "Sound: ON", font, sf::Color(50, 50, 50)),
          refMuted(stareMute)
    {
        textButton.setCharacterSize(16);

        sf::FloatRect bounds = textButton.getLocalBounds();
        textButton.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f,
            bounds.position.y + bounds.size.y / 2.0f
        });
        textButton.setPosition({
            pos.x + 100.f / 2.0f,
            pos.y + 40.f / 2.0f
        });
    }
    Buton* ButonMute::clone() const {
        return new ButonMute(*this);
    }

    void ButonMute::executaActiune(Joc& joc) {
        joc.toggleMute();
    }

    void ButonMute::doAfiseaza(sf::RenderWindow& window) const {
        auto& textRef = const_cast<sf::Text&>(textButton);
        auto& shapeRef = const_cast<sf::RectangleShape&>(shape);

        if (*refMuted) {
            textRef.setString("Sound: OFF");
            shapeRef.setFillColor(sf::Color(150, 50, 50));
        } else {
            textRef.setString("Sound: ON");
            shapeRef.setFillColor(sf::Color(50, 150, 50));
        }

        sf::FloatRect bounds = textRef.getLocalBounds();
        textRef.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f,
            bounds.position.y + bounds.size.y / 2.0f
        });

        window.draw(shape);
        window.draw(textButton);
    }

    void ButonMute::doPrint(std::ostream& out) const { out << "[Buton Mute]"; }

    ButonStatistici::ButonStatistici(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {180.f, 50.f}, "Statistici", font, sf::Color(100, 200, 200))
    {}
    Buton* ButonStatistici::clone() const {
        return new ButonStatistici(*this);
    }
    void ButonStatistici::executaActiune(Joc& joc) {
        joc.tranzitieLaStatistici();
    }
    void ButonStatistici::doPrint(std::ostream& out) const {
        out << "[Buton Statistici]";
    }

    ButonClipboard::ButonClipboard(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {160.f, 50.f}, "Copiaza", font, sf::Color(200, 200, 50))
    {}
    Buton* ButonClipboard::clone() const {
        return new ButonClipboard(*this);
    }
    void ButonClipboard::executaActiune(Joc& joc) {
        joc.copiazaStatisticiInClipboard();
    }
    void ButonClipboard::doPrint(std::ostream& out) const {
        out << "[Buton Clipboard]";
    }

    ButonInapoi::ButonInapoi(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {160.f, 50.f}, "Inapoi", font, sf::Color(150, 150, 150))
    {}
    Buton* ButonInapoi::clone() const { return new ButonInapoi(*this); }

    void ButonInapoi::executaActiune(Joc& joc) {
        joc.tranzitieLaGameOver();
    }

    ButonInfo::ButonInfo(const sf::Vector2f& pos, const sf::Font& font)
        : Buton(pos, {80.f, 40.f}, "Info", font, sf::Color(100, 100, 100)) {
        textButton.setCharacterSize(16);
    }
    Buton* ButonInfo::clone() const { return new ButonInfo(*this); }
    void ButonInfo::executaActiune(Joc& joc) {
        joc.tranzitieLaInfo();
    }
    void ButonInfo::doPrint(std::ostream& out) const { out << "[Buton Info]"; }


ButonAchievements::ButonAchievements(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {150.f, 50.f}, "Trofee", font, sf::Color(255, 215, 0))
{}
Buton* ButonAchievements::clone() const {
        return new ButonAchievements(*this);}
void ButonAchievements::executaActiune(Joc& joc) {
        joc.tranzitieLaAchievements();}
void ButonAchievements::doPrint(std::ostream& out) const {
        out << "[Buton Achievements]";}


ButonResetAchievements::ButonResetAchievements(const sf::Vector2f& pos, const sf::Font& font)
    : Buton(pos, {140.f, 40.f}, "Reset", font, sf::Color(200, 50, 50))
{}
Buton* ButonResetAchievements::clone() const {
        return new ButonResetAchievements(*this);}
void ButonResetAchievements::executaActiune(Joc& joc) {
        joc.ResetAchievements();}
void ButonResetAchievements::doPrint(std::ostream& out) const {
        out << "[Buton Reset Achievements]";}

    //*************************************************** Meniu
    Meniu::~Meniu() {
        for (const auto btn : butoane) delete btn;
        butoane.clear();
    }

    Meniu::Meniu(const Meniu& other) {
        for (const auto* btn : other.butoane) {
            butoane.push_back(btn->clone());
        }
    }

    Meniu& Meniu::operator=(Meniu other) {
        swap(*this, other);
        return *this;
    }

    void swap(Meniu& first, Meniu& second) noexcept {
        using std::swap;
        swap(first.butoane, second.butoane);
    }

    void Meniu::adaugaButon(Buton* btn) {
        butoane.push_back(btn);
    }
    //dynamic_cast cu altceva decat cout
    void Meniu::gestioneazaClick(const sf::Vector2f& mousePos, Joc& joc) const {
        for (const auto btn : butoane) {
            if (btn->esteMouseDeasupra(mousePos)) {

                if (dynamic_cast<ButonIesire*>(btn)) {
                    Joc::scrieInLog("Utilizatorul a apasat Butonul Iesire. Se inchide sesiunea...");
                }
                btn->executaActiune(joc);
                return;
            }
        }
    }

    void Meniu::actualizeazaHover(const sf::Vector2f& mousePos) const {
        for (const auto btn : butoane) {
            btn->updateCuloare(mousePos);
        }
    }

    void Meniu::deseneaza(sf::RenderWindow& window) const {
        for (const auto btn : butoane) {
            btn->afiseaza(window);
        }
    }
