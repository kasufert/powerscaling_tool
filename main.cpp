#include "headers.hpp"

#include "MatchupHolder.cpp"
#include "MatchupHolder2.cpp"
#include "funcs.cpp"
#include "types.cpp"
#include "EventHandler.cpp"
#include "main.h"

int main()
{

    std::vector<NamedImg> textures;
    MatchupHolder2 matchups;
    Matchup currentMatchup;
    bool isReversed = false;
    const char* storage = "results.bin";
    srand(time(NULL));

    fmt::print("VS Battler\n");
    sf::RenderWindow window(sf::VideoMode(800, 450), "VS Battler", sf::Style::Close);

#pragma region setup

    sf::RectangleShape rollBtn;
    rollBtn.setSize(sf::Vector2f(WIDTH * .3f, HEIGHT * .2f));
    rollBtn.setOutlineColor(sf::Color(0x114433FF));
    rollBtn.setOutlineThickness(3.f);
    rollBtn.setFillColor(sf::Color(0x228844FF));
    rollBtn.setOrigin(WIDTH * .3f * .5f, 0);
    rollBtn.setPosition(sf::Vector2f(WIDTH * .5f, HEIGHT * .15f));

// Image sprites
    sf::Sprite sprite1;
    sf::Sprite sprite2;

    sf::Text versusText;
    versusText.setString("VS");
    sf::Font font;
    font.loadFromFile("../../fonts/LCDM2B__.TTF");
    versusText.setFont(font);
    versusText.setPosition(375, 243);
    versusText.setCharacterSize(25);

    // Image names
    sf::Text text1("Text 1", font, 0);
    text1.setOrigin({ text1.getLocalBounds().width, text1.getLocalBounds().top });
    sf::Text text2("Text 2", font, 0);

    // Name locations
    sf::Vector2f text1loc = { 204.f, 245.f };
    sf::Vector2f text2loc = { 579.f, 245.f };
    text2.setPosition(text2loc);

    sf::Text rollText;
    rollText.setFont(font);
    rollText.setCharacterSize(16);
    rollText.setPosition(335, 97);
    rollText.setString("Enter image\nfolder first");

    sf::Text diagnosticText;
    diagnosticText.setFont(font);
    diagnosticText.setCharacterSize(18);
    diagnosticText.setPosition({ 300.f, 400.f });

    sf::RectangleShape imgFolderEntryBox;
    float imgFolderEntryBoxWidth = 197.f;
    imgFolderEntryBox.setSize({ imgFolderEntryBoxWidth, 34.f });
    imgFolderEntryBox.setFillColor(sf::Color(0xEEEEEEFF));
    imgFolderEntryBox.setOutlineColor(sf::Color(0xAAAAAAFF));
    imgFolderEntryBox.setOutlineThickness(3.f);
    imgFolderEntryBox.setPosition({ 296.f,361.f });

    sf::RectangleShape entryLineCursor;
    float entryLineCursorPosX = 300.f;
    bool entryLineIsActive = false;
    bool entryLineIsShowing = false;
    sf::Clock blinkTimer;
    sf::Time blinkTime = sf::seconds(.5f);
    entryLineCursor.setSize({ 2.f, 28.f });
    entryLineCursor.setFillColor(sf::Color::Black);
    entryLineCursor.setPosition({ entryLineCursorPosX, 363.f });

    std::vector<char> folderText;
    sf::Text folderTextText;
    folderTextText.setString("");
    folderTextText.setPosition({ 300.f, imgFolderEntryBox.getGlobalBounds().top + 5 });
    folderTextText.setCharacterSize(24);
    folderTextText.setFillColor(sf::Color::Black);
    folderTextText.setFont(font);

    sf::Time spinTime = sf::seconds(3.5f);
    sf::Time subSpinTime = sf::seconds(0.0005f);
    sf::Clock spinStarted;
    sf::Clock subSpinStarted;
    int spins = 0;
    bool isSpinning = false;
    bool speedmode = false;

#pragma endregion

    while (window.isOpen())
    {
        HandleEvents(window, matchups, rollBtn, textures, rollText, isSpinning, subSpinTime, sprite1, sprite2, imgFolderEntryBox, entryLineIsActive, entryLineIsShowing, blinkTimer, folderText, folderTextText, imgFolderEntryBoxWidth, entryLineCursor, speedmode, currentMatchup, text1, text2, text1loc, diagnosticText, isReversed);

        DoSpinner(isSpinning, subSpinStarted, subSpinTime, text1, text2, sprite1, textures, sprite2, text1loc, speedmode, rollText, currentMatchup, isReversed);
            // }

                // Update entry line blinker
        {
            if (blinkTimer.getElapsedTime().asSeconds() - blinkTime.asSeconds() > 0.f)
            {
                blinkTimer.restart();
                entryLineIsShowing = !entryLineIsShowing;
            }
        }

        window.clear(sf::Color(0x113355));

        window.draw(rollBtn);
        window.draw(sprite1);
        window.draw(sprite2);

        window.draw(imgFolderEntryBox);
        if (entryLineIsActive && entryLineIsShowing)
        {
            window.draw(entryLineCursor);
        }

        window.draw(folderTextText);

        window.draw(versusText);
        window.draw(rollText);

        window.draw(text1);
        window.draw(text2);

        window.display();

    }

    return 0;
}