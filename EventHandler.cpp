#pragma once
#include "headers.hpp"
void HandleEvents(sf::RenderWindow& window, MatchupHolder2& matchups, sf::RectangleShape& rollBtn, std::vector<NamedImg>& textures, sf::Text& rollText, bool& isSpinning, sf::Time& subSpinTime, sf::Sprite& sprite1, sf::Sprite& sprite2, sf::RectangleShape& imgFolderEntryBox, bool& entryLineIsActive, bool& entryLineIsShowing, sf::Clock& blinkTimer, std::vector<char>& folderText, sf::Text& folderTextText, float& imgFolderEntryBoxWidth, sf::RectangleShape& entryLineCursor, bool& speedmode, Matchup& currentMatchup, sf::Text& text1, sf::Text& text2, const sf::Vector2f& text1loc, sf::Text& diagnosticText, bool isReversed)
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
}
