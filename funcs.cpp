#include "headers.hpp"



void StartRoll(std::vector<NamedImg>& textures, sf::Text& rollText, bool& isSpinning, sf::Time& subSpinTime, sf::Sprite& sprite1, sf::Sprite& sprite2, int& canRoll)
{
    canRoll = 1;
    if (textures.size() == 0)
    {
        fmt::print("Need textures to roll!\n");
        canRoll = 2;
        return;
    }

    fmt::print("\nRolling...\n");
    rollText.setPosition(335, 97);
    rollText.setString("Rolling...");


    isSpinning = true;
    subSpinTime = sf::seconds(0.0005f);

    // Set up sprites

    sf::Texture tex1;
    sprite1.setTexture(textures.at(0).tex, true);
    sprite1.setPosition(222, 192);
    resize(sprite1);

    sf::Texture tex2;
    sprite2.setTexture(textures.at(0).tex);
    sprite2.setPosition(432, 192);
    resize(sprite2);
}

void resize(sf::Sprite& sprite)
{
    sf::Vector2f targetSpriteSize = { 135, 135 };
    sprite.setScale(1.f, 1.f);
    sf::FloatRect spriteSize = sprite.getLocalBounds();
    sf::Vector2f scaleBy = { targetSpriteSize.x / spriteSize.width,  targetSpriteSize.y / spriteSize.height };
    sprite.setScale(scaleBy);
}

std::string randomTexture(sf::Sprite& sprite, std::vector<NamedImg>& textures, sf::Text& text)
{
    int choices = textures.size();
    int choice = std::rand() % choices;
    sprite.setTexture(textures.at(choice).tex, true);
    text.setString(textures.at(choice).cleaned_name);
    return textures.at(choice).str;
}

void setMatchup(const Matchup& matchup, std::vector<NamedImg>& textures, sf::Sprite& s1, sf::Sprite& s2, sf::Text& t1, sf::Text& t2, sf::Vector2f text1loc)
{

    auto p1 = std::find(textures.begin(), textures.end(), matchup.first());
    auto p2 = std::find(textures.begin(), textures.end(), matchup.second());

    if (p1 == textures.end() || p2 == textures.end())
    {
        fmt::print("Matchup not found.\n");
        return;
    }

    s1.setTexture(p1->tex, true);
    resize(s1);
    t1.setString(p1->cleaned_name);
    t1.setOrigin({ t1.getLocalBounds().width, 0 });
    t1.setPosition(text1loc);

    s2.setTexture(p2->tex, true);
    resize(s2);
    t2.setString(p2->cleaned_name);

}


void Log(sf::Text& diagnosticText, std::string text)
{
    diagnosticText.setString(sf::String(text));
}

void FolderTextUpdate(std::vector<char>& folderText, sf::Text& folderTextText,
    sf::RectangleShape& imgFolderEntryBox, float& imgFolderEntryBoxWidth, sf::RectangleShape& entryLineCursor)
{
    std::string theString = std::string(folderText.begin(), folderText.end());
    fmt::print("\n{}\n", theString);
    folderTextText.setString(theString);
    if (folderTextText.getGlobalBounds().width > imgFolderEntryBoxWidth)
    {
        fmt::print("{}, {}", folderTextText.getGlobalBounds().top, folderTextText.getGlobalBounds().height);
        folderTextText.setPosition({
            (imgFolderEntryBox.getGlobalBounds().left + imgFolderEntryBox.getGlobalBounds().width) // Right side
                - folderTextText.getGlobalBounds().width - 3, // New left of text
                imgFolderEntryBox.getGlobalBounds().top + 5
            });
    }
    else
    {
        folderTextText.setPosition({ 300.f, imgFolderEntryBox.getGlobalBounds().top + 5 });
    }
    entryLineCursor.setPosition(
        { folderTextText.getGlobalBounds().left + folderTextText.getGlobalBounds().width,
        entryLineCursor.getGlobalBounds().top });
}

const char* const winnerToString(Winner w, bool isReversed)
{
    if (isReversed)
    {
        if (w == Person1)
        {
            w = Person2;
        }
        else if (w == Person2)
        {
            w = Person1;
        }
    }
    return WINNER_STRINGS[w];
}

void DoSpinner(bool& isSpinning, sf::Clock& subSpinStarted, sf::Time& subSpinTime, sf::Text& text1, sf::Text& text2, sf::Sprite& sprite1, std::vector<NamedImg>& textures, sf::Sprite& sprite2, sf::Vector2f& text1loc, bool speedmode, sf::Text& rollText, Matchup& currentMatchup, bool& isReversed)
{
    if (isSpinning && subSpinStarted.getElapsedTime() > subSpinTime)
    {
        text1.setCharacterSize(30);
        text2.setCharacterSize(30);

        std::string choice1 = randomTexture(sprite1, textures, text1);
        resize(sprite1);

        std::string choice2 = randomTexture(sprite2, textures, text2);
        resize(sprite2);

    // Redo text 1 position
        text1.setOrigin({ text1.getLocalBounds().width, 0 });
        text1.setPosition(text1loc);

        subSpinStarted.restart();
        subSpinTime += subSpinTime * .12f;
        if (subSpinTime.asSeconds() > .36f || speedmode)
        {
            isSpinning = false;
            rollText.setPosition(371, 97);
            rollText.setString("Roll");


            // Set up matchup
            currentMatchup = Matchup(choice1, choice2);
            if (currentMatchup.first() == choice2)
            {
                isReversed = true;
                fmt::print("Reversed.\n");
            }
            else
            {
                isReversed = false;
                fmt::print("Not Reversed.\n");

            }
            fmt::print("Current Matchup: {} vs {}\n", choice1, choice2);
        }
    }
}