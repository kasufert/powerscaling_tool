#include "types.hpp"

bool NamedImg::operator==(const std::string& str_) const
{
    return str_ == this->str;
}
Matchup::Matchup()
{
    exists = false;
}
Matchup::Matchup(std::string p1, std::string p2)
{
    const int result = p1.compare(p2);
    if (result == 0)
    {
        fmt::print("Mirror match!\n");
        exists = false;
        return;
    }
    if (result < 0)
    {
        exists = true;
        person1 = p1;
        person2 = p2;
        return;
    }
    else
    {
        exists = true;
        person1 = p2;
        person2 = p1;
        return;
    }
}

bool Matchup::operator==(const Matchup& other) const
{
    return ((person1 == other.person1) && (person2 == other.person2));
}

std::string Matchup::first() const
{
    return person1;
}
std::string Matchup::second() const
{
    return person2;
}



std::string Record::str() const
{
    using namespace fmt;
    return format("{}/{}/{}, Total {}, Ratio {}",
        styled(wins, fg(color::green)),
        styled(losses, fg(color::red)),
        styled(evens, fg(color::brown)),
        styled(wins + losses + evens, fg(color::yellow)),
        styled((float)(wins + (float)evens / 2.f) / (float)(wins + losses), fg(color::purple))
    );
}
