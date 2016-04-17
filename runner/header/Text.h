/* Copyright 2016 Jolivet Arthur & Laronze Florian

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef TEXT_H
#define TEXT_H

#include "GameModel.h"
#include "MenuModel.h"
#include "SFML/Graphics.hpp"
#include <map>
#include <fstream>

/********************************************
    Constant Variables
********************************************/
const std::string OLIVE_FONT = "Fonts/Antique_Olive.ttf";
const std::string ROBOTO_FONT = "Fonts/Roboto_Condensed.ttf";
const std::string ENGLISH_STRINGS = "Fonts/english.xml";
const std::string FRENCH_STRINGS = "Fonts/french.xml";
const std::string SPANISH_STRINGS = "Fonts/spanish.xml";

/********************************************
    Text Class
*********************************************
    @author Arthur  @date 02/04 - 17/04
*********************************************/
class Text
{
    public:
    //=== CTORs / DTORs
    Text();
    Text(const Text& myText) = delete;
    ~Text();

    //=== GETTERS
    sf::Text *getPauseResumeText();
    sf::Text *getRestartText();
    sf::Text *getHomeText();

    //=== METHODS
    void loadText();
    void changeLanguage(std::string currentLanguage);
    void updateString(std::string file, sf::Text *currentText, std::string currentName);
    void syncMenuHomeText(int width, int height);
    void syncMenuSettingsText(int width, int height);
    void syncGameMainText(GameModel *gameModel);
    void syncGamePauseText(GameModel *gameModel);
    void syncGameEndText(GameModel *gameModel);
    void drawMenuHomeText(sf::RenderWindow *window);
    void drawMenuSettingsText(sf::RenderWindow *window);
    void drawGameText(sf::RenderWindow *window);
    void drawPauseText(sf::RenderWindow *window);
    void drawEndText(sf::RenderWindow *window);

private:
    //=== ATTRIBUTES
    sf::Font *m_font;
    std::map<sf::Text*, std::string> m_textVector;

    //Menu Text
    sf::Text *m_playButtonText;
    sf::Text *m_quitButtonText;
    //Settings
    sf::Text *m_settingsLabel;
    sf::Text *m_configurationLabel;
    sf::Text *m_configLanguageLabel;
    sf::Text *m_configLanguageEnLabel;
    sf::Text *m_configLanguageFrLabel;
    sf::Text *m_configLanguageEsLabel;
    sf::Text *m_configDifficultyLabel;
    sf::Text *m_configDifficultyNormalLabel;
    sf::Text *m_configDifficultyMasterLabel;
    //Game Text
    sf::Text *m_distanceText;
    sf::Text *m_playerLifeLabel;
    sf::Text *m_bonusTimeoutLabel;
    sf::Text *m_bonusTimeoutText;
    //Pause Text
    sf::Text *m_pauseResumeLabel;
    sf::Text *m_restartLabel;
    sf::Text *m_homeLabel;
    sf::Text *m_coinsCollectedNumberText;
    //End Text
    sf::Text *m_endTitleLabel;
    sf::Text *m_gameSpeedmultiplicatorLabel;
    sf::Text *m_gameSpeedmultiplicatorText;
    sf::Text *m_distanceLabel;
    sf::Text *m_coinsCollectedNumberLabel;
    sf::Text *m_enemyDestructedBonusLabel;
    sf::Text *m_enemyDestructedBonusText;
    sf::Text *m_scoreLabel;
    sf::Text *m_scoreText;
};

#endif // TEXT_H