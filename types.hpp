#pragma once
#include "headers.hpp"



enum Winner : char
{
    None = 1, Person1, Person2, Even, NotEnoughInformation
};

class Matchup
{
    std::string person1;
    std::string person2;
public:
    bool exists = false;
    Matchup();
    Matchup(std::string p1, std::string p2);
    bool operator==(const Matchup& other) const;
    std::string first() const;
    std::string second() const;
};

template<>
struct std::hash<Matchup>
{
    std::size_t operator()(const Matchup& m) const;
};


class MatchupHolder
{
private:
    static constexpr std::size_t MAX_SIZE = 500;
    std::vector<std::pair<int, int>> seenMatchups;
    std::array <std::array<Winner, MAX_SIZE>, MAX_SIZE> matchups;
    int contestants;
    const char* const smFilepath = "sm.bin";
    const char* const muFilepath = "matchups.bin";
public:
    MatchupHolder(int contestants);

    bool readContents();

    Winner setMatchup(int person1, int person2, Winner w);

    Winner findMatchup(int person1, int person2);

    void dumpContents();

    std::pair<int, int> randomSeenMatchup();

};

struct Record {
    int wins = 0;
    int losses = 0;
    int evens = 0;
    std::string str() const;
};

class MatchupHolder2
{
private:
    std::unordered_map<Matchup, Winner> matchups;
    std::unordered_map<std::string, Record> records;
    std::vector<Matchup> seenMatchups;
    const char* const matchupsPath = "matchups2.bin";
    const char* const seenPath = "seen.bin";
public:
    MatchupHolder2();
    bool readFile();
    void dumpContents() const;
    Winner setMatchup(Matchup m, Winner w);
    Winner findMatchup(Matchup m) const;
    Matchup randomSeenMatchup() const;
    Record findRecord(std::string person);
    void populateRecords();
    void allRecords() const;
};

struct NamedImg
{
    sf::Texture tex;
    std::string str;
    sf::String cleaned_name;
    int id;
    bool operator==(const std::string& str_) const;
};

inline std::size_t std::hash<Matchup>::operator()(const Matchup& m) const
{
    return ((std::hash<std::string>()(m.first()))
        ^ (std::hash<std::string>()(m.second())) << 1);
}
