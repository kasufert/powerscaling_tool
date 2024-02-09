#include "headers.hpp"
#include "types.hpp"

MatchupHolder2::MatchupHolder2()
{
}
bool MatchupHolder2::readFile()
{
    std::ifstream in(matchupsPath, std::ios::in);

    try
    {
        while (!in.eof())
        {
            std::string line1;
            std::string line2;
            std::string line3;
            std::getline(in, line1);
            std::getline(in, line2);
            std::getline(in, line3);
            if (line3 == "")
            {
                break;
            }
            Matchup ma(line1, line2);
            Winner w = (Winner)line3.at(0);
            matchups.insert_or_assign(ma, w);
        }
    }
    catch (std::exception e)
    {
        fmt::print("Pause!!!!\n");
    }

    fmt::print("read file\n");

    return true;
}
void MatchupHolder2::dumpContents() const
{
    std::ofstream out(matchupsPath, std::ios::out | std::ios::trunc);

    for (const auto& m : matchups)
    {
        const Matchup& ma = m.first;
        Winner w = m.second;

        out << ma.first() << '\n' << ma.second() << '\n' << (char)w << '\n';
    }
    fmt::print("Wrote to file\n");

}
Winner MatchupHolder2::setMatchup(Matchup m, Winner w)
{
    auto current = matchups.find(m);
    Winner oldWinner = None;
    if (current != matchups.end())
    {
        oldWinner = current->second;
    }
    else
    {
        seenMatchups.push_back(m);
    }

    matchups.insert_or_assign(m, w);
    return oldWinner;

}
Winner MatchupHolder2::findMatchup(Matchup m) const
{
    Winner w = None;
    auto current = matchups.find(m);
    if (current != matchups.end())
    {
        w = current->second;
    }
    return w;
}
Matchup MatchupHolder2::randomSeenMatchup() const
{
    int cnt = seenMatchups.size();
    if (cnt == 0)
    {
        return Matchup();
    }
    int matchup = rand() % cnt;
    return seenMatchups.at(matchup);
}
Record MatchupHolder2::findRecord(std::string person)
{
    Record r;
    for (auto p : matchups)
    {
        Matchup m = p.first;
        Winner w = p.second;

        if (m.first() == person)
        {
            if (w == Person1)
            {
                r.wins++;
            }
            else if (w == Person2)
            {
                r.losses++;
            }
            else if (w == Even)
            {
                r.evens++;
            }
        }
        else if (m.second() == person)
        {
            if (w == Person2)
            {
                r.wins++;
            }
            else if (w == Person1)
            {
                r.losses++;
            }
            else if (w == Even)
            {
                r.evens++;
            }
        }
    }
    return r;
}

void MatchupHolder2::populateRecords()
{
    for (auto p : matchups)
    {
        std::string p1 = p.first.first();
        std::string p2 = p.first.second();
        Winner w = p.second;

        auto r1i = records.find(p1);
        auto r2i = records.find(p2);

        Record r1 = (r1i != records.end()) ? r1i->second : Record();
        Record r2 = (r2i != records.end()) ? r2i->second : Record();

        if (w == Person1)
        {
            r1.wins++;
            r2.losses++;
        }
        else if (w == Person2)
        {
            r1.losses++;
            r2.wins++;
        }
        else if (w == Even)
        {
            r1.evens++;
            r2.evens++;
        }

        records.insert_or_assign(p1, r1);
        records.insert_or_assign(p2, r2);
    }

}

void MatchupHolder2::allRecords() const
{
    for (auto r : records)
    {
        fmt::print("{:.<30}{}\n", r.first, r.second.str());
    }
}
