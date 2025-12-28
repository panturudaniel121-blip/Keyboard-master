#include "Joc.hpp"
#include "Buton.hpp"
#include "Static.hpp"
#include "Exceptii.hpp"
#include "CuvantSpecial.hpp"
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <SFML/Window/Clipboard.hpp>

Joc::Joc()
    : window(sf::VideoMode({static_cast<unsigned int>(Config::LATIME_FEREASTRA),
                            static_cast<unsigned int>(Config::INALTIME_FEREASTRA)}),
             Config::TITLU_FEREASTRA),
      fundal(52, 235, 137),
      fontPrincipal(),
      cuvant(),
      scor(),
      scorBoard(Config::CALE_SCORURI),
      ceasJoc(),
      timpLimita(sf::seconds(30.f)),
      textTimer(fontPrincipal, ""),
      hpCurent(100), hpMaxim(100), textHP(fontPrincipal, ""),
      stareCurenta(StareJoc::SelectieDificultate),
      nivelDificultate(DificultateJoc::Mediu),
      meniuStatistici(),
      textGameOver(fontPrincipal, ""),
      textScorFinal(fontPrincipal, ""),
      textIntroduNume(fontPrincipal, ""),
      textNumeJucator(fontPrincipal, ""),
      scorSalvat(false),
      meniuGameOver(),
      meniuStart(),
      textTitluStart(fontPrincipal, ""),
      meniuPierdut(),
      textMesajPierdut(fontPrincipal, ""),
      sunetOprit(false),
      textStatisticiTitlu(fontPrincipal, ""),
      textStat1(fontPrincipal, ""),
      textStat2(fontPrincipal, ""),
      textStat3(fontPrincipal, ""),
      textVersiune(fontPrincipal, "")
{
    window.setFramerateLimit(60);

    if (!fontPrincipal.openFromFile(Config::CALE_FONT)) {
        window.close();
        throw EroareInitializare("Font principal (" + Config::CALE_FONT + ")");
    }

    scor.initializareFont(fontPrincipal);
    cuvant.initializeaza(fontPrincipal);

    textHP.setFont(fontPrincipal);
    textHP.setCharacterSize(24);
    textHP.setFillColor(sf::Color::Red);
    textHP.setPosition({20.f, 60.f});
    textHP.setStyle(sf::Text::Bold);

    scorBoard.SBincarcare();
    incarcaMuzica();

    initializeazaUIGameOver();
    initializeazaUIStart();
    initializeazaUIPierdut();
    initializeazaUIStatistici();

    muzicaMeniu.setLooping(true);
    muzicaMeniu.play();


}


std::string Joc::obtineTimestamp()
{
    std::time_t t = std::time(nullptr);
    const std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" << now->tm_mday;
    return ss.str();
}

void Joc::scrieInLog(const std::string& mesaj)
{
    std::cout << "[LOG] " << mesaj << std::endl;
}



void Joc::initializeazaUIStart()
{
    textTitluStart.setString("Alege Dificultatea");
    textTitluStart.setCharacterSize(50);
    textTitluStart.setFillColor(sf::Color::Black);
    textTitluStart.setStyle(sf::Text::Bold);

    const sf::FloatRect bounds = textTitluStart.getLocalBounds();
    textTitluStart.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });
    textTitluStart.setPosition({350.f, 200.f});

    meniuStart.adaugaButon(new ButonDificultate({250.f, 300.f}, fontPrincipal, DificultateJoc::Usor, "Usor"));
    meniuStart.adaugaButon(new ButonDificultate({250.f, 400.f}, fontPrincipal, DificultateJoc::Mediu, "Mediu"));
    meniuStart.adaugaButon(new ButonDificultate({250.f, 500.f}, fontPrincipal, DificultateJoc::Greu, "Greu"));
    meniuStart.adaugaButon(new ButonMute({590.f, 10.f}, fontPrincipal, &sunetOprit));
}

void Joc::initializeazaUIGameOver()
{
    textGameOver.setString("Game Over!");
    textGameOver.setCharacterSize(50);
    textGameOver.setFillColor(sf::Color::Black);
    textGameOver.setStyle(sf::Text::Bold);

    textScorFinal.setCharacterSize(40);
    textScorFinal.setFillColor(sf::Color::Black);

    textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");
    textIntroduNume.setCharacterSize(24);
    textIntroduNume.setFillColor(sf::Color::Black);
    textIntroduNume.setPosition({150.f, 700.f});

    textNumeJucator.setCharacterSize(30);
    textNumeJucator.setFillColor(sf::Color(50, 50, 50));
    textNumeJucator.setPosition({150.f, 740.f});

    textTimer.setString("Timp: 30.0");
    textTimer.setCharacterSize(24);
    textTimer.setFillColor(sf::Color::Black);
    textTimer.setPosition({550.f, 30.f});

    meniuGameOver.adaugaButon(new ButonIesire({570.f, 5.f}, fontPrincipal));
    meniuGameOver.adaugaButon(new ButonReset({20.f, 5.f}, fontPrincipal));

    meniuGameOver.adaugaButon(new ButonRestart({120.f, 600.f}, fontPrincipal));

    meniuGameOver.adaugaButon(new ButonStatistici({280.f, 600.f}, fontPrincipal));

    meniuGameOver.adaugaButon(new ButonMeniu({480.f, 600.f}, fontPrincipal));
}

void Joc::initializeazaUIPierdut()
{
    textMesajPierdut.setString("AI PIERDUT!");
    textMesajPierdut.setCharacterSize(60);
    textMesajPierdut.setFillColor(sf::Color(200, 0, 0));
    textMesajPierdut.setStyle(sf::Text::Bold);

    const sf::FloatRect bounds = textMesajPierdut.getLocalBounds();
    textMesajPierdut.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });

    textMesajPierdut.setPosition({
        static_cast<float>(Config::LATIME_FEREASTRA) / 2.f,
        300.f
    });


    meniuPierdut.adaugaButon(new ButonRestart({100.f, 450.f}, fontPrincipal));
    meniuPierdut.adaugaButon(new ButonStatistici({260.f, 450.f}, fontPrincipal));
    meniuPierdut.adaugaButon(new ButonMeniu({460.f, 450.f}, fontPrincipal));
    meniuPierdut.adaugaButon(new ButonIesire({290.f, 550.f}, fontPrincipal));
}

void Joc::initializeazaUIStatistici()
{
    textStatisticiTitlu.setFont(fontPrincipal);
    textStatisticiTitlu.setCharacterSize(30);
    textStatisticiTitlu.setFillColor(sf::Color::Black);
    textStatisticiTitlu.setString("--- STATISTICI SESIUNE ---");

    sf::FloatRect b = textStatisticiTitlu.getLocalBounds();
    textStatisticiTitlu.setOrigin({b.position.x + b.size.x/2.f, b.position.y + b.size.y/2.f});
    textStatisticiTitlu.setPosition({static_cast<float>(Config::LATIME_FEREASTRA)/2.f, 100.f});

    auto configText = [&](sf::Text& t, float y) {
        t.setFont(fontPrincipal);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color(50, 50, 50));
        t.setPosition({100.f, y});
    };

    configText(textStat1, 200.f);
    configText(textStat2, 250.f);
    configText(textStat3, 300.f);

    textVersiune.setFont(fontPrincipal);
    textVersiune.setCharacterSize(14);
    textVersiune.setFillColor(sf::Color(100, 100, 100));
    textVersiune.setString("v0.2");
    textVersiune.setPosition({520.f, 770.f});

    meniuStatistici.adaugaButon(new ButonMeniu({120.f, 600.f}, fontPrincipal));

    meniuStatistici.adaugaButon(new ButonClipboard({420.f, 600.f}, fontPrincipal));
}



void Joc::setDificultate(const DificultateJoc dif) {
    nivelDificultate = dif;
}

void Joc::incepeJoc() {
    stareCurenta = StareJoc::Jucand;
    scor.reset();
    ceasJoc.restart();
    scorSalvat = false;
    numeJucator = "";
    textNumeJucator.setString("");
    textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");
    ManagerSesiune::getInstance().marcheazaInceput();
    timpLimita = sf::seconds(45.f);
    if (nivelDificultate == DificultateJoc::Usor) {
        hpMaxim = 100;
    }
    else if (nivelDificultate == DificultateJoc::Mediu) {
        hpMaxim = 50;
    }
    else {
        hpMaxim = 1;
    }

    hpCurent = hpMaxim;
    textHP.setString("HP: " + std::to_string(hpCurent));

    cuvant.reseteaza();

    muzicaMeniu.stop();
    muzicaPierdut.stop();

    std::string cale;
    if (nivelDificultate == DificultateJoc::Usor) cale = Config::CALE_MUZICA_USOR;
    else if (nivelDificultate == DificultateJoc::Mediu) cale = Config::CALE_MUZICA_MEDIU;
    else cale = Config::CALE_MUZICA_GREU;

    if (muzicaJoc.openFromFile(cale)) {
        muzicaJoc.setLooping(true);
        if (sunetOprit) muzicaJoc.setVolume(0);
        else muzicaJoc.setVolume(100);
        muzicaJoc.play();
    }

    scrieInLog("Joc inceput. Dificultate setata.");
}

void Joc::reseteazaClasament()
{
    scorBoard.SBresetare();
    scorSalvat = false;
    numeJucator = "";
    textNumeJucator.setString(numeJucator);
    textIntroduNume.setString("Scor resetat! Introdu numele:");
}

void Joc::restartJoc()
{
    incepeJoc();
}

void Joc::mergiLaMeniu()
{
    numeJucator = "";
    textNumeJucator.setString("");
    scorSalvat = false;
    textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");

    stareCurenta = StareJoc::SelectieDificultate;

    muzicaJoc.stop();
    muzicaPierdut.stop();

    if (muzicaMeniu.getStatus() != sf::SoundSource::Status::Playing) {
        if (sunetOprit) muzicaMeniu.setVolume(0);
        else muzicaMeniu.setVolume(100);
        muzicaMeniu.play();
    }
}

void Joc::tranzitieLaGameOver()
{
    stareCurenta = StareJoc::GameOver;

    if (!scorSalvat) {
        textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");
    }

    const std::string text = "Scorul tau final: " + std::to_string(scor.getValoare());
    textScorFinal.setString(text);

    const sf::FloatRect b1 = textGameOver.getLocalBounds();
    textGameOver.setOrigin({b1.position.x + b1.size.x / 2.f, b1.position.y + b1.size.y / 2.f});
    textGameOver.setPosition({static_cast<float>(Config::LATIME_FEREASTRA) / 2.f, 80.f});

    const sf::FloatRect b2 = textScorFinal.getLocalBounds();
    textScorFinal.setOrigin({b2.position.x + b2.size.x / 2.f, b2.position.y + b2.size.y / 2.f});
    textScorFinal.setPosition({static_cast<float>(Config::LATIME_FEREASTRA) / 2.f, 150.f});

    muzicaJoc.stop();
    if (muzicaMeniu.getStatus() != sf::SoundSource::Status::Playing) {
        if (sunetOprit) muzicaMeniu.setVolume(0);
        else muzicaMeniu.setVolume(100);
        muzicaMeniu.play();
    }
}

void Joc::tranzitieLaPierdut()
{
    stareCurenta = StareJoc::Pierdut;

    muzicaJoc.stop();
    if (sunetOprit) muzicaPierdut.setVolume(0);
    else muzicaPierdut.setVolume(100);

    muzicaPierdut.setLooping(false);
    muzicaPierdut.play();
}

void Joc::tranzitieLaStatistici() {
    stareCurenta = StareJoc::Statistici;

    const std::string dataCurenta = obtineTimestamp();

    const StatisticaSesiune<int> statScor("Scor Final", scor.getValoare());
    const StatisticaSesiune<std::string> statData("Data Sesiune", dataCurenta);

    textStat1.setString(statData.genereazaText());
    textStat3.setString(statScor.genereazaText());
}



void Joc::ruleaza()
{
    while (window.isOpen())
    {
        gestioneazaEvenimente();
        actualizeaza();
        afiseaza();
    }
}

void Joc::gestioneazaEvenimente()
{
    while (auto event = window.pollEvent())
    {
        if (event->getIf<sf::Event::Closed>())
            window.close();

        if (stareCurenta == StareJoc::SelectieDificultate)      gestioneazaEvenimenteStart(*event);
        else if (stareCurenta == StareJoc::Jucand)              gestioneazaEvenimenteJucand(*event);
        else if (stareCurenta == StareJoc::GameOver)            gestioneazaEvenimenteGameOver(*event);
        else if (stareCurenta == StareJoc::Pierdut)             gestioneazaEvenimentePierdut(*event);
        else if (stareCurenta == StareJoc::Statistici)          gestioneazaEvenimenteStatistici(*event);
    }
}

void Joc::gestioneazaEvenimenteStart(const sf::Event& event)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuStart.actualizeazaHover(worldPos);

    if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuStart.gestioneazaClick(worldPos, *this);
        }
    }
}

void Joc::gestioneazaEvenimenteJucand(const sf::Event& event) {
    TipCuvant bonus = cuvant.gestioneazaEvenimente(event, scor, nivelDificultate);

    if (bonus == TipCuvant::BonusHP) {
        hpCurent += 10;

        hpCurent = clamp<int>(hpCurent, 0, 200);

        textHP.setString("HP: " + std::to_string(hpCurent));

    }
}

void Joc::gestioneazaEvenimenteGameOver(const sf::Event& event)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuGameOver.actualizeazaHover(worldPos);

    if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuGameOver.gestioneazaClick(worldPos, *this);
        }
    }

    if (auto textEv = event.getIf<sf::Event::TextEntered>()) {
        if (scorSalvat) return;

        uint32_t unicode = textEv->unicode;

        if (unicode == 8 && !numeJucator.empty()) {
            numeJucator.pop_back();
        }
        else if (unicode == 13 && !numeJucator.empty()) {
            scorBoard.adaugaScor(numeJucator, scor.getValoare());
            scorSalvat = true;
            textIntroduNume.setString("Scor salvat! Felicitari!");
        }
        else if (unicode >= 32 && unicode < 128 && numeJucator.size() < 15) {
            numeJucator += static_cast<char>(unicode);
        }
        textNumeJucator.setString(numeJucator);
    }
}

void Joc::gestioneazaEvenimentePierdut(const sf::Event& event)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuPierdut.actualizeazaHover(worldPos);

    if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuPierdut.gestioneazaClick(worldPos, *this);
        }
    }
}

void Joc::gestioneazaEvenimenteStatistici(const sf::Event& event)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuStatistici.actualizeazaHover(worldPos);

    if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuStatistici.gestioneazaClick(worldPos, *this);
        }
    }
}

void Joc::actualizeaza()
{
    if (stareCurenta == StareJoc::Jucand) {
        actualizeazaJucand();
    }
}

void Joc::actualizeazaJucand()
{
    float timpRamas = timpLimita.asSeconds() - ceasJoc.getElapsedTime().asSeconds();
    if (timpRamas < 0) timpRamas = 0;

    constexpr float dt = 1.0f / 60.0f;

    if (const int cuvintePierdute = cuvant.actualizeaza(dt, nivelDificultate, scor); cuvintePierdute > 0) {
        hpCurent -= cuvintePierdute * 10;
        if (hpCurent < 0) hpCurent = 0;
        textHP.setString("HP: " + std::to_string(hpCurent));
    }

    if (hpCurent <= 0) {
        tranzitieLaPierdut();
    }
    else if (timpRamas == 0) {
        tranzitieLaGameOver();
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << timpRamas;
    textTimer.setString("Timp: " + ss.str());
}

void Joc::afiseaza()
{
    window.clear(fundal);

    if (stareCurenta == StareJoc::SelectieDificultate)      afiseazaStart();
    else if (stareCurenta == StareJoc::Jucand)              afiseazaJucand();
    else if (stareCurenta == StareJoc::GameOver)            afiseazaGameOver();
    else if (stareCurenta == StareJoc::Pierdut)             afiseazaPierdut();
    else if (stareCurenta == StareJoc::Statistici)          afiseazaStatistici();

    window.display();
}

void Joc::afiseazaStart()
{
    window.draw(textTitluStart);
    meniuStart.deseneaza(window);
}

void Joc::afiseazaJucand()
{
    cuvant.afiseaza(window);
    scor.afiseaza(window);
    window.draw(textTimer);
    window.draw(textHP);
}

void Joc::afiseazaGameOver()
{
    window.draw(textGameOver);
    window.draw(textScorFinal);
    scorBoard.afiseaza(window, fontPrincipal);
    window.draw(textIntroduNume);
    window.draw(textNumeJucator);
    meniuGameOver.deseneaza(window);
}

void Joc::afiseazaPierdut()
{
    window.draw(textMesajPierdut);
    meniuPierdut.deseneaza(window);
}

void Joc::afiseazaStatistici()
{
    window.draw(textStatisticiTitlu);
    window.draw(textStat1);
    window.draw(textStat2);
    window.draw(textStat3);
    window.draw(textVersiune);
    meniuStatistici.deseneaza(window);
}

void Joc::incarcaMuzica() {
    if (!muzicaMeniu.openFromFile(Config::CALE_MUZICA_MENIU)) std::cerr << "Eroare meniu music\n";
    if (!muzicaPierdut.openFromFile(Config::CALE_MUZICA_PIERDUT)) std::cerr << "Eroare defeat music\n";
}

void Joc::toggleMute() {
    sunetOprit = !sunetOprit;
    float vol = sunetOprit ? 0.f : 100.f;
    muzicaMeniu.setVolume(vol);
    muzicaJoc.setVolume(vol);
    muzicaPierdut.setVolume(vol);
}

void Joc::copiazaStatisticiInClipboard() const {
    std::string continut = "=== Keyboard Master Stats ===\n";

    continut += textStat1.getString().toAnsiString() + "\n";
    continut += textStat2.getString().toAnsiString() + "\n";
    continut += textStat3.getString().toAnsiString() + "\n";

    continut += "=============================\n";
    continut += "Joaca si tu Keyboard Master!";

    sf::Clipboard::setString(continut);

    scrieInLog("Statisticile au fost copiate in Clipboard!");
}

std::ostream& operator<<(std::ostream& out, const Joc& j)
{
    out << j.scor << "\n";
    return out;
}