#pragma once
#include "headers.hpp"


void resize(sf::Sprite& sprite);

std::string randomTexture(sf::Sprite& sprite, std::vector<NamedImg>& textures, sf::Text& text);

void Log(sf::Text& diagnosticText, std::string text);

void FolderTextUpdate(std::vector<char>& folderText, sf::Text& folderTextText,
    sf::RectangleShape& imgFolderEntryBox, float& imgFolderEntryBoxWidth, sf::RectangleShape& entryLineCursor);

void StartRoll(std::vector<NamedImg>& textures, sf::Text& rollText, bool& isSpinning, sf::Time& subSpinTime, sf::Sprite& sprite1, sf::Sprite& sprite2, int& retFlag);

const char* const winnerToString(Winner w, bool isReversed);

void setMatchup(const Matchup& matchup, std::vector<NamedImg>& textures, sf::Sprite& s1, sf::Sprite& s2, sf::Text& t1, sf::Text& t2, sf::Vector2f text1loc);

void HandleEvents(sf::RenderWindow& window, MatchupHolder2& matchups, sf::RectangleShape& rollBtn, std::vector<NamedImg>& textures, sf::Text& rollText, bool& isSpinning, sf::Time& subSpinTime, sf::Sprite& sprite1, sf::Sprite& sprite2, sf::RectangleShape& imgFolderEntryBox, bool& entryLineIsActive, bool& entryLineIsShowing, sf::Clock& blinkTimer, std::vector<char>& folderText, sf::Text& folderTextText, float& imgFolderEntryBoxWidth, sf::RectangleShape& entryLineCursor, bool& speedmode, Matchup& currentMatchup, sf::Text& text1, sf::Text& text2, const sf::Vector2f& text1loc, sf::Text& diagnosticText, bool isReversed);

void DoSpinner(bool& isSpinning, sf::Clock& subSpinStarted, sf::Time& subSpinTime, sf::Text& text1, sf::Text& text2, sf::Sprite& sprite1, std::vector<NamedImg>& textures, sf::Sprite& sprite2, sf::Vector2f& text1loc, bool speedmode, sf::Text& rollText, Matchup& currentMatchup, bool& isReversed);