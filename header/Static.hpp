#pragma once
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <type_traits>

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& getInstance() {
        static T instance;
        return instance;
    }
};

class ButonFactory {
public:
    template <typename TipButon, typename... Args>
    static TipButon* creaza(Args&&... args) {
        return new TipButon(std::forward<Args>(args)...);
    }
};

template <typename T>
T clamp(T valoare, T minim, T maxim) {
    if (valoare < minim) return minim;
    if (valoare > maxim) return maxim;
    return valoare;
}

template <typename T>
class StatisticaSesiune {
    std::string eticheta;
    T valoare;
public:
    StatisticaSesiune(std::string  e, const T& v) : eticheta(std::move(e)), valoare(v) {}

    [[nodiscard]] std::string genereazaText() const {
        return eticheta + ": " + std::to_string(valoare);
    }
};

template <>
inline std::string StatisticaSesiune<std::string>::genereazaText() const {
    return eticheta + ": " + valoare;
}

struct Config {
    static constexpr int LATIME_FEREASTRA = 700;
    static constexpr int INALTIME_FEREASTRA = 800;
    static const std::string TITLU_FEREASTRA;
    static const std::string CALE_FONT;
    static const std::string CALE_SCORURI;
    static const std::string CALE_MUZICA_MENIU;
    static const std::string CALE_MUZICA_USOR;
    static const std::string CALE_MUZICA_MEDIU;
    static const std::string CALE_MUZICA_GREU;
    static const std::string CALE_MUZICA_PIERDUT;
    static const std::string CALE_SCORURI_ENDLESS;
    static const std::string CALE_MUZICA_ENDLESS;
    static const std::string CALE_SUNET_TASTA;
    static const std::string CALE_SUNET_BOOM;
    static const std::string CALE_SUNET_ACHIEVEMENT;
    static const std::string CALE_SUNET_WIN;
    static const std::string CALE_ICON_COIN;
    static const std::string CALE_SHOP_DATA;
    static const std::string CALE_TEXTURA_BG_1;
    static const std::string CALE_TEXTURA_BG_2;
    static const std::string CALE_TEXTURA_BG_3;
};


class Random {
private:
    static std::mt19937 generator;

public:
    static int getInt(int min, int max);
    static float getFloat(float min, float max);
};

class ManagerSesiune : public Singleton<ManagerSesiune> {
    friend class Singleton<ManagerSesiune>;
private:
    ManagerSesiune() : jocInceput(false) {}
    bool jocInceput;
public:
    void marcheazaInceput() { jocInceput = true; }
};

class LogManager : public Singleton<LogManager> {
    friend class Singleton<LogManager>;
private:
    LogManager() = default;
public:
    static void log(const std::string& mesaj) {
        std::cout << "[LOG SYSTEM] " << mesaj << std::endl;
    }
};