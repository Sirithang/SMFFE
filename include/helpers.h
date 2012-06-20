#ifndef SMFFE_HELPERS_H
#define SMFFE_HELPERS_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <map>
#include <vector>
#include <sstream>

#include "CommandInput.h"

void initDebugLog();
void toDebugLog(const sf::String &pStr);

std::map<sf::String, sf::String> parseDataFile(const sf::String &pFile);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

float stringToFloat(const sf::String &pStr);

std::string intToString(int pValue);

std::vector<std::string> getAllFolders(const sf::String &pFolder);

InputType getInputFromString(const sf::String &pString);

sf::String getNameOfKey(sf::Keyboard::Key pKey);

bool fexists(const char *filename);

#endif