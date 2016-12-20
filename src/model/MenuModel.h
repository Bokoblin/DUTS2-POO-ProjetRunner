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

#ifndef MENU_MODEL_H
#define MENU_MODEL_H

#include "Model.h"
#include "Leaderboard.h"
#include "Settings.h"
#include "Shop.h"

enum MenuState
{
    HOME,
    SETTINGS,
    LEADERBOARD,
    SHOP
};

/**
 * MenuModel Class
 * @author Arthur
 * @date 14/04 - 20/12
 */
class MenuModel : public Model
{
public:
    //=== CTORs / DTORs
    MenuModel(const Model& model);
    virtual ~MenuModel();

    //=== GETTERS
    MenuState getMenuState() const;

    //=== SETTERS
    void setMenuState(MenuState state);

    //=== METHODS
    virtual void nextStep() override;
    Leaderboard* launchLeaderboard();
    Shop* launchShop();
    Settings* launchSettings();

private:
    //=== ATTRIBUTES
    MenuState m_menuState;
    Leaderboard *m_leaderboard;
    Settings *m_settings;
    Shop *m_shop;
};

#endif
