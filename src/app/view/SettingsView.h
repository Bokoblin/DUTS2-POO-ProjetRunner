/* Copyright 2016-2017 Jolivet Arthur & Laronze Florian

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

#ifndef SETTINGS_VIEW_H
#define SETTINGS_VIEW_H

#include "AbstractView.h"
#include "../model/Settings.h"
#include "../../libs/MDC-SFML/RadioButton.h"
#include "../../libs/MDC-SFML/Dialog.h"

/**
 * SettingsView Class
 * @author Arthur
 * @date 20/05/16 - 02/11/17
 */
class SettingsView : public AbstractView
{
public:
    //=== CTORs / DTORs
    SettingsView(sf::RenderWindow *window, TextHandler *textHandler, Settings *settingsModel);
    ~SettingsView() override;

    //=== METHODS
    void synchronize() override;
    void draw() const override;
    bool handleEvents(sf::Event event) override;

private:
    //=== ATTRIBUTES
    Settings *m_settings;
    mdsf::Dialog *m_confirmDialog;

    //Graphic Elements
    mdsf::Button *m_homeFormButton;
    mdsf::Button *m_englishLangRadio;
    mdsf::Button *m_frenchLangRadio;
    mdsf::Button *m_spanishLangRadio;
    mdsf::Button *m_easyModeRadio;
    mdsf::Button *m_hardModeRadio;
    mdsf::Button *m_defaultBallSkinRadio;
    mdsf::Button *m_morphBallSkinRadio;
    mdsf::Button *m_capsuleBallSkinRadio;
    mdsf::Button *m_resetRectButton;
    mdsf::Button *m_menuMusicButton;
    mdsf::Button *m_gameMusicButton;
    mdsf::Sprite *m_logoIUT;
    mdsf::Sprite *m_logoSFML;
    mdsf::Sprite *m_iconRepoLink;
    mdsf::Sprite *m_iconEmailLink;

    //Containers
    std::map<int, mdsf::RadioButton *> m_pageIndicators;
    std::vector<mdsf::Button*> m_buttonList;

    //=== METHODS
    void loadImages() override;
    void handleMusic();
};


#endif