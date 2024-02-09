#include "headers.hpp"

MatchupHolder::MatchupHolder(int contestants)
{
    this->contestants = contestants;
    for (int i = 0; i < MAX_SIZE; ++i)
    {
        for (int j = 0; j < MAX_SIZE; ++j)
        {
            matchups.at(i).at(j) = Winner::None;
        }
    }
}
Winner MatchupHolder::setMatchup(int person1, int person2, Winner w)
{
    if (person1 > MAX_SIZE || person2 > MAX_SIZE)
    {
        throw;
    }
    if (person1 > person2)
    {
        int tmp = person1;
        person1 = person2;
        person2 = tmp;

        if (w == Person1)
        {
            w = Person2;
        }
        else if (w == Person2)
        {
            w = Person1;
        }
    }

    Winner prev = matchups.at(person1).at(person2);
    matchups.at(person1).at(person2) = w;

    // Add only if present
    if (std::find(seenMatchups.begin(), seenMatchups.end(), std::pair(person1, person2)) == seenMatchups.end())
    {
        seenMatchups.push_back(std::pair(person1, person2));
    }
    return prev;
}
Winner MatchupHolder::findMatchup(int person1, int person2)
{
    if (person1 > MAX_SIZE || person2 > MAX_SIZE)
    {
        throw;
    }
    if (person1 > person2)
    {
        int tmp = person1;
        person1 = person2;
        person2 = tmp;
    }

    return matchups.at(person1).at(person2);
}
std::pair<int, int> MatchupHolder::randomSeenMatchup()
{
    int cnt = seenMatchups.size();
    int matchup = rand() % cnt;
    return seenMatchups.at(matchup);
}
void MatchupHolder::dumpContents()
{
    char buf[MAX_SIZE * MAX_SIZE];
    int spot = 0;
    for (const std::array<Winner, MAX_SIZE>& winArr : matchups) // Write matchups
    {
        for (const Winner winner : winArr)
        {
            buf[spot] = winner;
            spot++;
        }
    }

    std::ofstream mu_file;
    mu_file.open(muFilepath, std::ios::out | std::ios::binary | std::ios::trunc);
    mu_file << buf;
    mu_file.close();

    std::ofstream sm_file;
    sm_file.open(smFilepath, std::ios::out | std::ios::binary | std::ios::trunc);
    sm_file << seenMatchups.size() << '\n';
    for (std::pair<int, int> matchup : seenMatchups)
    {
        sm_file << matchup.first << '\n';
        sm_file << matchup.second << '\n';
    }
    sm_file.close();
}
bool MatchupHolder::readContents()
{
    std::ifstream mu_file;
    mu_file.open(muFilepath, std::ios::in | std::ios::binary);
    char dataBuf[MAX_SIZE * MAX_SIZE];
    mu_file.read(dataBuf, MAX_SIZE * MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; ++i)
    {
        for (int j = 0; j < MAX_SIZE; ++j)
        {
            matchups.at(i).at(j) = (Winner)dataBuf[(i * MAX_SIZE) + j];
        }
    }
    mu_file.close();

    seenMatchups.clear();
    std::ifstream sm_file;
    sm_file.open(smFilepath, std::ios::in | std::ios::binary);
    int size;
    sm_file >> size;
    for (int i = 0; i < size; ++i)
    {
        int person1 = -1;
        int person2 = -1;
        sm_file >> person1;
        sm_file >> person2;
        seenMatchups.push_back(std::pair(person1, person2));
    }
    sm_file.close();


    return true;
}