#include "Joc.hpp"
#include "Buton.hpp"
#include "Static.hpp"
#include "Exceptii.hpp"
#include "CuvantSpecial.hpp"
#include "Achievements.hpp"
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <SFML/Window/Clipboard.hpp>
#include "Shop.hpp"

std::string formateazaSecunde(float secundeTotal) {
    const int minute = static_cast<int>(secundeTotal) / 60;
    const int secunde = static_cast<int>(secundeTotal) % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minute << ":"
       << std::setw(2) << std::setfill('0') << secunde;
    return ss.str();
}

Joc::Joc()
    :window(sf::VideoMode({static_cast<unsigned int>(Config::LATIME_FEREASTRA),
                            static_cast<unsigned int>(Config::INALTIME_FEREASTRA)}),
             Config::TITLU_FEREASTRA),
      fundal(52, 235, 137),
      fontPrincipal(),
      cuvant(),
      scor(),
      scorBoard(Config::CALE_SCORURI),
      scorBoardEndless(Config::CALE_SCORURI_ENDLESS),
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
      textInfoTitlu(fontPrincipal, ""),

      textInfoLabels{
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
                    {fontPrincipal, ""},
      },
      textInfoDescs{
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
                      {fontPrincipal, ""},
          },
      waveCurent(1),
      textWave(fontPrincipal, ""),
      textStatisticiTitlu(fontPrincipal, ""),
      textStat1(fontPrincipal, ""),
      textStat2(fontPrincipal, ""),
      textStat3(fontPrincipal, ""),
      textStat4(fontPrincipal, ""),
      textStat5(fontPrincipal, ""),
      textStat6(fontPrincipal, ""),
      textVersiune(fontPrincipal, ""),
      magazin(),
      texturaCoin(),
      spriteCoin(texturaCoin),
      spriteFundal(texturaCoin),
      textMonedeMenu(fontPrincipal, "")

{
    window.setFramerateLimit(60);

    if (!fontPrincipal.openFromFile(Config::CALE_FONT)) {
        window.close();
        throw EroareInitializare("Font principal (" + Config::CALE_FONT + ")");
    }

    scor.initializareFont(fontPrincipal);
    cuvant.initializeaza(fontPrincipal);
    magazin.initializeaza(fontPrincipal);

    if (texturaCoin.loadFromFile(Config::CALE_ICON_COIN)) {
        spriteCoin.setTexture(texturaCoin, true);
        spriteCoin.setScale({0.3f, 0.3f});

        spriteCoin.setPosition({20.f, 10.f});
    } else {
        sf::Image img;
        img.resize({32, 32}, sf::Color::Yellow);

        if (!texturaCoin.loadFromImage(img)) {
            std::cerr << "Err fallback coin\n";
        }

        spriteCoin.setTexture(texturaCoin, true);
        spriteCoin.setScale({0.3f, 0.3f});
        spriteCoin.setPosition({20.f, 10.f});
    }

    textMonedeMenu.setFont(fontPrincipal);
    textMonedeMenu.setCharacterSize(24);
    textMonedeMenu.setStyle(sf::Text::Bold);
    textMonedeMenu.setFillColor(sf::Color::Black);
    textMonedeMenu.setPosition({85.f, 25.f});

    textTimer.setFont(fontPrincipal);
    textTimer.setCharacterSize(30);
    textTimer.setFillColor(sf::Color::Black);
    textTimer.setPosition({525.f, 30.f});

    textHP.setFont(fontPrincipal);
    textHP.setCharacterSize(30);
    textHP.setFillColor(sf::Color::Red);
    textHP.setPosition({20.f, 60.f});
    textHP.setStyle(sf::Text::Bold);

    scorBoard.SBincarcare();
    incarcaMuzica();

    initializeazaUIGameOver();
    initializeazaUIStart();
    initializeazaUIPierdut();
    initializeazaUIStatistici();
    initializeazaUIInfo();

    muzicaMeniu.setLooping(true);
    muzicaMeniu.play();

    textWave.setFont(fontPrincipal);
    textWave.setCharacterSize(30);
    textWave.setFillColor(sf::Color::Red);
    textWave.setPosition({525.f, 60.f});

    efecte.incarcaSunete();

    managerAchievements.initializeaza(fontPrincipal);
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
    LogManager::log(mesaj);
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

    meniuStart.adaugaButon(ButonFactory::creaza<ButonDificultate>(sf::Vector2f{250.f, 300.f}, fontPrincipal, DificultateJoc::Usor, "Usor"));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonDificultate>(sf::Vector2f{250.f, 400.f}, fontPrincipal, DificultateJoc::Mediu, "Mediu"));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonDificultate>(sf::Vector2f{250.f, 500.f}, fontPrincipal, DificultateJoc::Greu, "Greu"));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonDificultate>(sf::Vector2f{250.f, 600.f}, fontPrincipal, DificultateJoc::Endless, "Endless"));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonMute>(sf::Vector2f{590.f, 10.f}, fontPrincipal, &sunetOprit));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonInfo>(sf::Vector2f{20.f, 740.f}, fontPrincipal));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonMagazin>(sf::Vector2f{530.f, 680.f}, fontPrincipal));
    meniuStart.adaugaButon(ButonFactory::creaza<ButonAchievements>(sf::Vector2f{530.f, 740.f}, fontPrincipal));
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

    meniuGameOver.adaugaButon(ButonFactory::creaza<ButonIesire>(sf::Vector2f{570.f, 5.f}, fontPrincipal));
    meniuGameOver.adaugaButon(ButonFactory::creaza<ButonReset>(sf::Vector2f{20.f, 5.f}, fontPrincipal));
    meniuGameOver.adaugaButon(ButonFactory::creaza<ButonRestart>(sf::Vector2f{75.f, 600.f}, fontPrincipal));
    meniuGameOver.adaugaButon(ButonFactory::creaza<ButonStatistici>(sf::Vector2f{255.f, 600.f}, fontPrincipal));
    meniuGameOver.adaugaButon(ButonFactory::creaza<ButonMeniu>(sf::Vector2f{475.f, 600.f}, fontPrincipal));
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


    meniuPierdut.adaugaButon(ButonFactory::creaza<ButonRestart>(sf::Vector2f{100.f, 450.f}, fontPrincipal));
    meniuPierdut.adaugaButon(ButonFactory::creaza<ButonMeniu>(sf::Vector2f{290.f, 450.f}, fontPrincipal));
    meniuPierdut.adaugaButon(ButonFactory::creaza<ButonIesire>(sf::Vector2f{500.f, 450.f}, fontPrincipal));
}

void Joc::initializeazaUIStatistici()
{
    textStatisticiTitlu.setFont(fontPrincipal);
    textStatisticiTitlu.setCharacterSize(34);
    textStatisticiTitlu.setFillColor(sf::Color::Black);
    textStatisticiTitlu.setString("--- STATISTICI SESIUNE ---");

    sf::FloatRect b = textStatisticiTitlu.getLocalBounds();
    textStatisticiTitlu.setOrigin({b.position.x + b.size.x/2.f, b.position.y + b.size.y/2.f});
    textStatisticiTitlu.setPosition({static_cast<float>(Config::LATIME_FEREASTRA)/2.f, 80.f});

    auto configText = [&](sf::Text& t, float y) {
        t.setFont(fontPrincipal);
        t.setCharacterSize(26);
        t.setFillColor(sf::Color(40, 40, 40));
        t.setPosition({120.f, y});
    };

    configText(textStat1, 180.f);
    configText(textStat2, 240.f);
    configText(textStat3, 300.f);
    configText(textStat4, 360.f);
    configText(textStat5, 420.f);
    configText(textStat6, 480.f);

    textVersiune.setFont(fontPrincipal);
    textVersiune.setCharacterSize(14);
    textVersiune.setFillColor(sf::Color(120, 120, 120));
    textVersiune.setString("v0.3 - Keyboard Master");
    textVersiune.setPosition({500.f, 760.f});


    meniuStatistici.adaugaButon(ButonFactory::creaza<ButonMeniu>(sf::Vector2f{80.f, 600.f}, fontPrincipal));
    meniuStatistici.adaugaButon(ButonFactory::creaza<ButonInapoi>(sf::Vector2f{270.f, 600.f}, fontPrincipal));
    meniuStatistici.adaugaButon(ButonFactory::creaza<ButonClipboard>(sf::Vector2f{460.f, 600.f}, fontPrincipal));
}

void Joc::initializeazaUIInfo() {
    textInfoTitlu.setFont(fontPrincipal);
    textInfoTitlu.setString("--- CUM SE JOACA ---");
    textInfoTitlu.setCharacterSize(34);
    textInfoTitlu.setFillColor(sf::Color::Black);
    textInfoTitlu.setPosition({160.f, 30.f});

    struct InfoLinie {
        std::string label;
        std::string desc;
        sf::Color culoare;
    };

    const InfoLinie date[] = {
        {"OBIECTIV:", "Scrie cuvintele inainte sa expire timpul.", sf::Color(50, 50, 50)},
        {"CONDITIE:", "Daca HP-ul ajunge la 0, ai pierdut.", sf::Color::Red},
        {"IESIRE:", "Tine apasat ESC pentru a iesi.", sf::Color(100, 100, 100)},
        {"Cuvant ROSU:", "Cuvant normal de baza.", sf::Color::Red},
        {"Cuvant NEGRU:", "Se scrie automat dupa prima litera.", sf::Color::Black},
        {"Cuvant VERDE:", "Iti reface HP-ul cu +10 puncte.", sf::Color(0, 150, 0)},
        {"Cuvant ALBASTRU:", "Primesti bonus de +3 secunde.", sf::Color::Blue},
        {"Cuvant AURIU:",    "Primesti TRIPLU scor pe cuvant.",    sf::Color(255, 215, 0)},
        {"Cuvant MOV:",      "Seteaza COMBO-ul direct la x3.",     sf::Color(255, 0, 255)},
        {"Cuvant GRI:",    "NU il tasta! Iti scade viata!",      sf::Color(80, 80, 80)},
    };

    for (int i = 0; i < 10; ++i) {
        textInfoLabels[i].setFont(fontPrincipal);
        textInfoLabels[i].setString(date[i].label);
        textInfoLabels[i].setFillColor(date[i].culoare);
        textInfoLabels[i].setCharacterSize(24);
        textInfoLabels[i].setPosition({50.f, 130.f + (static_cast<float>(i) * 60.f)});

        textInfoDescs[i].setFont(fontPrincipal);
        textInfoDescs[i].setString(date[i].desc);
        textInfoDescs[i].setFillColor(sf::Color(60, 60, 60));
        textInfoDescs[i].setCharacterSize(24);

        textInfoDescs[i].setPosition({280.f, 130.f + (static_cast<float>(i) * 60.f)});
    }

    meniuInfo.adaugaButon(ButonFactory::creaza<ButonMeniu>(sf::Vector2f{270.f, 700.f}, fontPrincipal));
}

void Joc::setDificultate(const DificultateJoc dif) {
    nivelDificultate = dif;
}

void Joc::incepeJoc() {
    stareCurenta = StareJoc::Jucand;

    int mDif = 1;
    if (nivelDificultate == DificultateJoc::Mediu) mDif = 2;
    else if (nivelDificultate == DificultateJoc::Greu) mDif = 3;
    else if (nivelDificultate == DificultateJoc::Endless) mDif = 5;

    scor = ScorBuilder()
            .cuValoareStart(0)
            .cuComboStart(1)
            .cuDificultate(mDif)
            .build();

    scor.initializareFont(fontPrincipal);

    ceasJoc.restart();
    scorSalvat = false;
    numeJucator = "";
    waveCurent = 0;
    textWave.setString("Wave: 1");
    textNumeJucator.setString("");
    textIntroduNume.setString("Introdu numele: (apasa Enter pt. a salva)");
    ManagerSesiune::getInstance().marcheazaInceput();

    if (nivelDificultate == DificultateJoc::Endless) {
        hpMaxim = 100;
        timpLimita = sf::seconds(0.f);
    }
    else if (nivelDificultate == DificultateJoc::Usor) {
        hpMaxim = 100;
        timpLimita = sf::seconds(45.f);
    }
    else if (nivelDificultate == DificultateJoc::Mediu) {
        hpMaxim = 50;
        timpLimita = sf::seconds(45.f);
    }
    else {
        hpMaxim = 1;
        timpLimita = sf::seconds(45.f);
    }

    hpCurent = hpMaxim;
    textHP.setString("HP: " + std::to_string(hpCurent));

    cuvant.reseteaza();

    muzicaMeniu.stop();
    muzicaPierdut.stop();

    nrCuvintePrinse = 0;
    nrGrupuriPrinse = 0;
    nrCuvinteSpeciale = 0;
    totalTasteApasate = 0;
    totalTasteCorecte = 0;

    std::string cale;
    if (nivelDificultate == DificultateJoc::Usor)
        cale = Config::CALE_MUZICA_USOR;
    else if (nivelDificultate == DificultateJoc::Mediu)
        cale = Config::CALE_MUZICA_MEDIU;
    else if (nivelDificultate == DificultateJoc::Greu)
        cale = Config::CALE_MUZICA_GREU;
    else
        cale = Config::CALE_MUZICA_ENDLESS;

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

    float acuratete = (totalTasteApasate > 0) ?
        ((static_cast<float>(totalTasteCorecte) / static_cast<float>(totalTasteApasate) * 100.f)) : 0.f;

    float timpScurs = ceasJoc.getElapsedTime().asSeconds();

    const StatisticaSesiune<std::string> statData("Data", obtineTimestamp());
    const StatisticaSesiune<int> statScor("Scor Final", scor.getValoare());

    textStat1.setString(statData.genereazaText());
    textStat2.setString(statScor.genereazaText());
    textStat3.setString("Cuvinte Totale: " + std::to_string(nrCuvintePrinse));
    textStat4.setString("Cuvinte Speciale: " + std::to_string(nrCuvinteSpeciale));


    std::stringstream ssAcu;

    ssAcu << "Acuratete: " << std::fixed << std::setprecision(0) << acuratete << "%";
    textStat5.setString(ssAcu.str());

    if (nivelDificultate == DificultateJoc::Endless) {
        textStat6.setString("Timp Rezistat: " + formateazaSecunde(timpScurs));
    } else {
        const float minute = timpLimita.asSeconds() / 60.0f;
        const float wpm = (minute > 0) ? (static_cast<float>(nrCuvintePrinse) / minute) : 0;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << "Ritm: " << wpm << " WPM";
        textStat6.setString(ss.str());
    }
}

void Joc::tranzitieLaInfo() {
    stareCurenta = StareJoc::Info;
}

void Joc::tranzitieLaAchievements() {
    stareCurenta = StareJoc::Achievements;
}

void Joc::tranzitieLaMagazin() {
    stareCurenta = StareJoc::Magazin;
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
        else if (stareCurenta == StareJoc::Info)                gestioneazaEvenimenteInfo(* event);
        else if (stareCurenta == StareJoc::Achievements)        gestioneazaEvenimenteAchievements(*event);
        else if (stareCurenta == StareJoc::Magazin)             gestioneazaEvenimenteMagazin(*event);
        }
    }

void Joc::gestioneazaEvenimenteStart(const sf::Event& event)
{
    const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuStart.actualizeazaHover(worldPos);

    if (const auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuStart.gestioneazaClick(worldPos, *this);
        }
    }

    if (const auto textEv = event.getIf<sf::Event::TextEntered>()) {
        if (textEv->unicode < 128) {

            bufferCheat += static_cast<char>(textEv->unicode);

            if (bufferCheat.size() > 20) {
                bufferCheat.erase(0, 1);
            }
            if (bufferCheat.find("300bani") != std::string::npos) {

                magazin.adaugaMonede(300);

                efecte.playWin();
                scrieInLog("CHEAT CODE ACTIVAT: +300 Bani!");

                bufferCheat.clear();
            }
        }
    }
}

void Joc::gestioneazaEvenimenteJucand(const sf::Event& event) {
    if (event.getIf<sf::Event::TextEntered>()) {
        totalTasteApasate++;
    }

    TipCuvant bonus = cuvant.gestioneazaEvenimente(event, scor, nivelDificultate, totalTasteCorecte, efecte);

    if (bonus != TipCuvant::Niciunul) {
        nrCuvintePrinse++;

        if (bonus == TipCuvant::Capcana) {
            hpCurent -= 30;
            if (hpCurent < 0) hpCurent = 0;
            textHP.setString("HP: " + std::to_string(hpCurent));
            scrieInLog("Ai lovit o capcana! -30 HP");
            return;
        }
        if (bonus != TipCuvant::Normal) {
            nrCuvinteSpeciale++;
        }

        if (bonus == TipCuvant::BonusHP) {
            hpCurent = clamp<int>(hpCurent + 10, 0, 1000);
            textHP.setString("HP: " + std::to_string(hpCurent));
        }
        else if (bonus == TipCuvant::BonusTimp) {
            auto timpNou = clamp<float>(timpLimita.asSeconds() + 3.f, 0.f, 300.f);
            timpLimita = sf::seconds(timpNou);
            scrieInLog("Bonus Timp activat! +3 secunde");
        }
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

            float timpFinalSeconds = 0.f;
            if (nivelDificultate == DificultateJoc::Endless) {
                timpFinalSeconds = ceasJoc.getElapsedTime().asSeconds();
            } else {
                timpFinalSeconds = ceasJoc.getElapsedTime().asSeconds();
            }
            const std::string timpString = formateazaSecunde(timpFinalSeconds);

            if (nivelDificultate == DificultateJoc::Endless) {
                scorBoardEndless.adaugaScor(numeJucator, scor.getValoare(), timpString);
            } else {
                scorBoard.adaugaScor(numeJucator, scor.getValoare(), timpString);
            }

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

    if (const auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuStatistici.gestioneazaClick(worldPos, *this);
        }
    }
}

void Joc::gestioneazaEvenimenteInfo(const sf::Event& event) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    meniuInfo.actualizeazaHover(worldPos);

    if (const auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            meniuInfo.gestioneazaClick(worldPos, *this);
        }
    }

    if (const auto keyEv = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEv->code == sf::Keyboard::Key::Escape) {
            mergiLaMeniu();
        }
    }
}

void Joc::gestioneazaEvenimenteAchievements(const sf::Event& event) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    managerAchievements.actualizeaza(0.f, worldPos);

    if (const auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            if (managerAchievements.aDatClickMeniu(worldPos)) {
                mergiLaMeniu();
            }
            if (managerAchievements.aDatClickReset(worldPos)) {
                ResetAchievements();
            }
        }
    }

    if (const auto keyEv = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEv->code == sf::Keyboard::Key::Escape) {
            mergiLaMeniu();
        }
    }
}

void Joc::gestioneazaEvenimenteMagazin(const sf::Event& event) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    magazin.actualizeaza(worldPos);

    if (const auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEv->button == sf::Mouse::Button::Left) {
            if (magazin.aDatClickIesire(worldPos)) {
                mergiLaMeniu();
            } else {
                magazin.gestioneazaClick(worldPos);
            }
        }
    }
    if (const auto keyEv = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEv->code == sf::Keyboard::Key::Escape) mergiLaMeniu();
    }
}

void Joc::actualizeaza()
{
    constexpr float dt = 1.0f / 60.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        timpApasareEsc += 1.0f / 60.0f;

        if (timpApasareEsc >= 1.0f)
            window.close();
    }
    else
        timpApasareEsc = 0.0f;
    if (stareCurenta == StareJoc::Jucand)
        actualizeazaJucand();
    else if (stareCurenta == StareJoc::GameOver) {
        efecte.actualizeaza(dt);
        managerAchievements.actualizeaza(dt, {0,0});
    }
    else if (stareCurenta == StareJoc::Magazin) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        magazin.actualizeaza(worldPos);
    }
}

void Joc::actualizeazaJucand()
{
    constexpr float dt = 1.0f / 60.0f;
    efecte.actualizeaza(dt);
    managerAchievements.actualizeaza(dt, {0,0});
    float acuratete = (totalTasteApasate > 0) ?
        ((static_cast<float>(totalTasteCorecte) / static_cast<float>(totalTasteApasate) * 100.f)) : 0.f;
    managerAchievements.verificaConditii(
        scor.getValoare(),
        scor.getCombo(),
        waveCurent,
        nrCuvintePrinse,
        acuratete,
        false,
        static_cast<int>(nivelDificultate)
    );
    int cuvintePierdute = cuvant.actualizeaza(dt, nivelDificultate, scor, waveCurent);

    if (nivelDificultate == DificultateJoc::Endless && cuvant.aGeneratGrupNou()) {
        waveCurent++;
        textWave.setString("Wave: " + std::to_string(waveCurent));
        Joc::scrieInLog("Wave nou: " + std::to_string(waveCurent));
    }

    if (cuvintePierdute > 0) {
        hpCurent -= cuvintePierdute * 10;
        if (hpCurent < 0) hpCurent = 0;
        textHP.setString("HP: " + std::to_string(hpCurent));
    }

    if (hpCurent <= 0) {
        if (nivelDificultate == DificultateJoc::Endless) {
            int baniCastigati = scor.getValoare() / 10;

            if (baniCastigati > 0) {
                magazin.adaugaMonede(baniCastigati);
                scrieInLog("Nivel complet! Ai castigat " + std::to_string(baniCastigati) + " monede!");
            }
            tranzitieLaGameOver();
        } else {
            tranzitieLaPierdut();
        }
        return;
    }

    if (nivelDificultate == DificultateJoc::Endless) {
        float timpScurs = ceasJoc.getElapsedTime().asSeconds();
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << timpScurs;
        textTimer.setString("Timp: " + ss.str() + "s");
    }
    else {
        float timpRamas = timpLimita.asSeconds() - ceasJoc.getElapsedTime().asSeconds();
        if (timpRamas <= 0) {
            timpRamas = 0;
            managerAchievements.verificaConditii(
            scor.getValoare(),
            scor.getCombo(),
            waveCurent,
            nrCuvintePrinse,
            acuratete,
            true,
            static_cast<int>(nivelDificultate)
            );

            efecte.playWin();
            efecte.spawnConfetti();

            int baniCastigati = scor.getValoare() / 10;
            if (baniCastigati > 0) {
                magazin.adaugaMonede(baniCastigati);
                scrieInLog("Nivel complet! Ai castigat " + std::to_string(baniCastigati) + " monede!");
            }
            tranzitieLaGameOver();
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << timpRamas;
        textTimer.setString("Timp: " + ss.str());
    }

}

void Joc::afiseaza()
{
    sf::Color culoareCurenta = magazin.getCuloareFundal();

    window.clear(culoareCurenta);

    const sf::Texture* tex = magazin.getTexturaFundal();

    if (tex != nullptr) {
        spriteFundal.setTexture(*tex, true);

        sf::Vector2u marimeTex = tex->getSize();
        float scaleX = static_cast<float>(Config::LATIME_FEREASTRA) / static_cast<float>(marimeTex.x);
        float scaleY = static_cast<float>(Config::INALTIME_FEREASTRA) / static_cast<float>(marimeTex.y);

        spriteFundal.setScale({scaleX, scaleY});
        spriteFundal.setPosition({0.f, 0.f});

        window.draw(spriteFundal);
    }

    if (stareCurenta == StareJoc::SelectieDificultate)      afiseazaStart();
    else if (stareCurenta == StareJoc::Jucand)              afiseazaJucand();
    else if (stareCurenta == StareJoc::GameOver)            afiseazaGameOver();
    else if (stareCurenta == StareJoc::Pierdut)             afiseazaPierdut();
    else if (stareCurenta == StareJoc::Statistici)          afiseazaStatistici();
    else if (stareCurenta == StareJoc::Info)                afiseazaInfo();
    else if (stareCurenta == StareJoc::Achievements)        afiseazaAchievements();
    else if (stareCurenta == StareJoc::Magazin)             afiseazaMagazin();

    if (timpApasareEsc > 0.0f)
    {
        sf::RectangleShape fundalBara({200.f, 20.f});
        fundalBara.setPosition({Config::LATIME_FEREASTRA / 2.f - 100.f, 20.f});
        fundalBara.setFillColor(sf::Color(50, 50, 50, 150));

        sf::RectangleShape incarcareBara({(timpApasareEsc / 1.0f) * 200.f, 20.f});
        incarcareBara.setPosition({Config::LATIME_FEREASTRA / 2.f - 100.f, 20.f});
        incarcareBara.setFillColor(sf::Color::Red);

        window.draw(fundalBara);
        window.draw(incarcareBara);
    }

    window.display();
}

void Joc::afiseazaStart()
{
    window.draw(textTitluStart);
    window.draw(spriteCoin);
    window.draw(textMonedeMenu);
    textMonedeMenu.setString(std::to_string(magazin.getMonede()));
    meniuStart.deseneaza(window);
}

void Joc::afiseazaJucand()
{
    cuvant.afiseaza(window);
    efecte.deseneaza(window);
    scor.afiseaza(window);
    window.draw(textTimer);
    window.draw(textHP);

    if (nivelDificultate == DificultateJoc::Endless) {
        window.draw(textWave);
    }
    managerAchievements.deseneazaNotificare(window);

}

void Joc::afiseazaGameOver()
{
    efecte.deseneaza(window);

    window.draw(textGameOver);
    window.draw(textScorFinal);

    if (nivelDificultate == DificultateJoc::Endless) {
        scorBoardEndless.afiseaza(window, fontPrincipal,true);
    } else {
        scorBoard.afiseaza(window, fontPrincipal,false);
    }

    window.draw(textIntroduNume);
    window.draw(textNumeJucator);
    meniuGameOver.deseneaza(window);

    managerAchievements.deseneazaNotificare(window);
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
    window.draw(textStat4);
    window.draw(textStat5);
    window.draw(textStat6);
    window.draw(textVersiune);
    meniuStatistici.deseneaza(window);
}

void Joc::afiseazaInfo()
{
    window.draw(textInfoTitlu);
    for (const auto & textInfoLabel : textInfoLabels) {
        window.draw(textInfoLabel);
    }
    for (const auto & textInfoDesc : textInfoDescs) {
        window.draw(textInfoDesc);
    }
    meniuInfo.deseneaza(window);
}

void Joc::afiseazaAchievements() {
    managerAchievements.deseneazaMeniu(window);
}

void Joc::afiseazaMagazin() {
    magazin.deseneaza(window);
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

    std::string detaliiMod;
    if (nivelDificultate == DificultateJoc::Endless) {
        detaliiMod = "Endless";
    } else {
        detaliiMod = "Normal - ";
        if (nivelDificultate == DificultateJoc::Usor) detaliiMod += "Usor";
        else if (nivelDificultate == DificultateJoc::Mediu) detaliiMod += "Mediu";
        else if (nivelDificultate == DificultateJoc::Greu) detaliiMod += "Greu";
    }

    continut += "Mod: " + detaliiMod + "\n\n";

    continut += textStat1.getString().toAnsiString() + "\n";
    continut += textStat2.getString().toAnsiString() + "\n";
    continut += textStat3.getString().toAnsiString() + "\n";
    continut += textStat4.getString().toAnsiString() + "\n";
    continut += textStat6.getString().toAnsiString() + "\n";

    continut += "\n=============================\n";
    continut += "Joaca si tu Keyboard Master!";

    sf::Clipboard::setString(continut);
}

void Joc::ResetAchievements() {
    managerAchievements.reseteazaProgres();
    Joc::scrieInLog("Achievements resetate manual!");
}

std::ostream& operator<<(std::ostream& out, const Joc& j)
{
    out << j.scor << "\n";
    return out;
}
