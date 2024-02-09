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