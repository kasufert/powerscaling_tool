#include "headers.hpp"

#include "MatchupHolder.cpp"
#include "MatchupHolder2.cpp"
#include "funcs.cpp"
#include "types.cpp"

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
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::EventType::Closed:
            {
                fmt::print("Closed!");
                std::cout << "Closed.";
                matchups.dumpContents();
                window.close();
                break;
            }
            case sf::Event::EventType::MouseButtonPressed:
            {
                fmt::print("\nClick! Location: {}, {}", e.mouseButton.x, e.mouseButton.y);
                sf::Vector2f mousePos = { (float)e.mouseButton.x, (float)e.mouseButton.y };

                // Check if roll btn is clicked
                sf::FloatRect btnBounds = rollBtn.getGlobalBounds();
                if (btnBounds.contains(mousePos))
                {
                    int canRoll;
                    StartRoll(textures, rollText, isSpinning, subSpinTime, sprite1, sprite2, canRoll);
                    if (canRoll == 2)
                    {
                        break;
                    }
                }

                // Check if img entry box is clicked
                sf::FloatRect ieBounds = imgFolderEntryBox.getGlobalBounds();
                if (ieBounds.contains(mousePos))
                {
                    fmt::print("\nText box clicked\n");
                    entryLineIsActive = true;
                    entryLineIsShowing = true;
                    blinkTimer.restart();

                }
                else
                {
                    entryLineIsActive = false;
                }


                // Check if one of the images is clicked
                sf::FloatRect img1Bounds = sprite1.getGlobalBounds();
                sf::FloatRect img2Bounds = sprite2.getGlobalBounds();

                if (img1Bounds.contains(mousePos))
                {

                }

                break;
            }
            case sf::Event::EventType::TextEntered:
            {
                if (entryLineIsActive)
                {
                    fmt::print("Character pressed: {}\n", (char)e.text.unicode);
                    if (e.text.unicode == 8) // 8 is unicode for backspace
                    {
                        if (folderText.size() > 0)
                            folderText.pop_back();
                    }
                    else if (e.text.unicode == 22)
                    {
                        break;
                    }
                    else
                    {
                        folderText.push_back(e.text.unicode);
                    }
                    FolderTextUpdate(folderText, folderTextText, imgFolderEntryBox,
                        imgFolderEntryBoxWidth, entryLineCursor);

                }
                break;
            }
            case sf::Event::EventType::KeyPressed:
            {
                if (e.key.code == sf::Keyboard::R) // Read
                {
                    matchups.readFile();
                }
                if (e.key.code == sf::Keyboard::W) // Write
                {
                    matchups.dumpContents();
                }
                if (e.key.code == sf::Keyboard::S) // Speed mode
                {
                    speedmode = !speedmode;
                }
                if (e.key.code == sf::Keyboard::M) // Random seen matchup
                {
                    Matchup m = matchups.randomSeenMatchup();
                    if (m.exists)
                    {
                        currentMatchup = m;
                        setMatchup(currentMatchup, textures, sprite1, sprite2, text1, text2, text1loc);
                    }
                    else
                    {
                        fmt::print("No matchups have been seen yet.\n");
                    }
                }
                if (e.key.code == sf::Keyboard::D)
                {
                    if (currentMatchup.exists)
                    {
                        Record r1 = matchups.findRecord(currentMatchup.first());
                        Record r2 = matchups.findRecord(currentMatchup.second());
                        fmt::print("{}: {} \n {}: {}", currentMatchup.first(), r1.str(), currentMatchup.second(), r2.str());
                    }
                }
                if (e.key.code == sf::Keyboard::A)
                {
                    matchups.populateRecords();
                    matchups.allRecords();
                }

                if (e.key.code == sf::Keyboard::Enter && textures.size() == 0) // Load
                {
                    std::string texturesPath = "C:\\Users\\sutee\\pictures\\ft_chars\\images";
                    const int MAX_CHARS = 500;
                    int code = 0;
                    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(texturesPath))
                    {
                        std::filesystem::path path = entry.path();
                        std::string pathStr = path.string();
                        Log(diagnosticText, pathStr);
                        sf::Texture tex;
                        tex.loadFromFile(pathStr);
                        std::string name = path.stem().string();
                        std::string cleaned_name = name;
                        std::replace(cleaned_name.begin(), cleaned_name.end(), ' ', '\n');
                        NamedImg img(tex, name, sf::String(cleaned_name), code);
                        code += 1;
                        textures.push_back(img);
                        if (code == MAX_CHARS)
                        {
                            break;
                        }
                    }
                    rollText.setCharacterSize(30);
                    rollText.setString("Roll");
                    matchups.readFile();
                }

                if (e.key.code == sf::Keyboard::Enter && !textures.empty()) // Roll
                {
                    int canRoll;
                    StartRoll(textures, rollText, isSpinning, subSpinTime, sprite1, sprite2, canRoll);
                    if (canRoll == 2)
                    {
                        break;
                    }
                }

                // Declare winner
                if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down)
                {
                    Winner last = matchups.findMatchup(currentMatchup);
                    Winner w = None;
                    if (isReversed)
                    {
                        if (last == Person1)
                        {
                            last = Person2;
                        }
                        else if (last == Person2)
                        {
                            last = Person1;
                        }
                    }
                    switch (e.key.code)
                    {
                    case sf::Keyboard::Left:
                        w = Person1;
                        break;
                    case sf::Keyboard::Right:
                        w = Person2;
                        break;
                    case sf::Keyboard::Up:
                        w = Even;
                        break;
                    case sf::Keyboard::Down:
                        w = NotEnoughInformation;
                        break;
                    }
                    if (w == last)
                    {
                        fmt::print("\nYou agreed with the norm and said the winner was {}\n",
                            winnerToString(w, false));
                        break;
                    }
                    else
                    {
                        fmt::print("\nYou said the winner was {}, norm says it's {}\n",
                            winnerToString(w, false), winnerToString(last, isReversed));
                    }
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
                    matchups.setMatchup(currentMatchup, w);
                    break;
                }

                // Paste
                if (e.key.control && e.key.code == sf::Keyboard::V)
                {
                    std::string pastedText = sf::Clipboard::getString();
                    for (int i = 0; i < pastedText.length(); ++i)
                    {
                        folderText.push_back(pastedText.at(i));
                    }
                    FolderTextUpdate(folderText, folderTextText, imgFolderEntryBox,
                        imgFolderEntryBoxWidth, entryLineCursor);
                }
                break;
            }
            case sf::Event::EventType::MouseMoved:
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                // Check if mouse is hovering roll btn
                sf::FloatRect btnBounds = rollBtn.getGlobalBounds();
                static bool isBtnHovered = true;
                if (btnBounds.contains(sf::Vector2f(mousePos)))
                {
                    rollBtn.setFillColor(sf::Color(0x33AA66FF));
                    if (!isBtnHovered)
                    {
                        isBtnHovered = true;
                    }
                }
                else
                {
                    rollBtn.setFillColor(sf::Color(0x228844FF));
                    if (isBtnHovered)
                    {
                        isBtnHovered = false;
                    }
                }
                break;
            }
            }
        }


        // if (spinStarted.getElapsedTime() < spinTime)
        // {

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


