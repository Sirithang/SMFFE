#include "helpers.h"

#include <fstream>
#include <iostream>
#include <dirent.h>

const sf::String gDebugLog = "debug.log";

std::map<sf::String, sf::String> parseDataFile(const sf::String &pFile)
{
	std::map<sf::String, sf::String> lReturn;

	std::ifstream t(pFile.ToAnsiString());

	if(!t.is_open())
		return lReturn;

	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

	std::string lStrData(str);

	lStrData.erase(std::remove_if(lStrData.begin(), lStrData.end(), ::isspace), lStrData.end());
	
	std::vector<std::string> lLine = split(lStrData, ';');

	for(int i = 0; i < lLine.size(); i++)
	{
		std::vector<std::string> lValue = split(lLine.at(i), '=');

		lReturn[lValue.at(0)] = lValue.at(1);
	}

	return lReturn;
}

//--------------------------------------------------------------
// 
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;

    while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }

    return elems;
}

//-----------------------------------------------------------------

std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

//------------------------------------------------------------------

float stringToFloat(const sf::String &pStr)
{
	return atof(pStr.ToAnsiString().c_str());
}

//-------------------------------------------------------------------

void initDebugLog()
{
#ifndef DEBUG
	std::ofstream lfile;
	lfile.open(gDebugLog.ToAnsiString(), std::ios::out);
	lfile.close();
#endif
}

//-------------------------------------------------------------------

void toDebugLog(const sf::String &pStr)
{
#ifndef DEBUG
	std::ofstream lfile;
	lfile.open(gDebugLog.ToAnsiString(), std::ios::app);
	lfile<<pStr.ToAnsiString()<<std::endl;
	lfile.close();
#else
	//std::cout<<pStr.ToAnsiString()<<std::endl;
#endif
}

//-------------------------------------------------------------------

std::vector<std::string> getAllFolders(const sf::String &pFolder)
{
	DIR *lDir;
	struct dirent *ent;

	std::vector<std::string> mReturn;

	// *********** IMAGE LOADING ****/////////////////////////
	lDir = opendir (pFolder.ToAnsiString().c_str());

	if (lDir != NULL) 
	{

	  /* print all the files and directories within directory */
	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_DIR)
		  {
			  sf::String lStr(ent->d_name);
			  if(lStr == "." || lStr == "..")
				  continue;

			  mReturn.push_back(lStr.ToAnsiString());
		  }
	  }
	  closedir (lDir);
	}

	  return mReturn;
}

//-----------------------------------------------------------

InputType getInputFromString(const sf::String &pString)
{
	if(pString == "DOWN")
		return DOWN;

	if(pString == "DOWNLEFT")
		return DOWNLEFT;

	if(pString == "DOWNRIGHT")
		return DOWNRIGHT;

	if(pString == "UP")
		return UP;

	if(pString == "UPLEFT")
		return UPLEFT;

	if(pString == "UPRIGHT")
		return UPRIGHT;

	if(pString == "LEFT")
		return LEFT;

	if(pString == "RIGHT")
		return RIGHT;


	if(pString == "LP")
		return C_LP;

	if(pString == "HP")
		return C_HP;

	if(pString == "LK")
		return C_LK;

	if(pString == "HK")
		return C_HK;

	return NONE;
}

//------------------------------------------------------------

std::string intToString(int pValue)
{
	std::stringstream StrStream;
	StrStream << pValue;
	return StrStream.str();
}

//--------------------------------------------------------------

bool fexists(const char* filename)
{
	std::ifstream ifile(filename);
	return ifile;
}

//---------------------------------------------------------------

sf::String getNameOfKey(sf::Keyboard::Key pKey)
{
	sf::String lString[sf::Keyboard::KeyCount] = 
	{	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P",
		"Q", "R", "S", "T", "U", "V", "W", "X","Y", "Z", "0", "1", "2", "3", "4", "5", "6",
		"7", "8", "9", "Escape", "LControl", "lShift", "lAlt", "lSystem", "RControl", "RShift",
		"RAlt", "RSystem", "Menu", "LBracket", "RBracket", "SemiColon", "Comma","Period", "Quote",
		"Slash", "BackSlash", "Tilde", "Equal", "Dash", "Space", "Return", "Back", "Tab", "PageUp", "PageDown", 
		"End", "Home", "Insert", "Delete", "Add", "Substract", "Multiply", "Divide", "Left", "Right", "Up", "Down",
		"Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpadè", "Numpad8", "Numpad9", 
		"F1","F2", "F3", "F4","F5","F6","F7","F8","F9","F10","F11","F12", "F13", "F14", "F15", "Pause" };

	return lString[pKey];
}