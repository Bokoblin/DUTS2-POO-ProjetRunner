/* Copyright 2016-2019 Jolivet Arthur & Laronze Florian

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

#ifndef LEADERBOARD_VIEW_H
#define LEADERBOARD_VIEW_H

#include <libs/MDC-SFML/src/RaisedButton.h>
#include "libs/MDC-SFML/src/Dialog.h"
#include "app/model/models/LeaderboardModel.h"
#include "app/view/utils/DialogBuilder.h"
#include "AbstractView.h"

/**
 * The LeaderboardView class inherits AbstractView.
 * It is the view counterpart of the class LeaderboardModel.
 * It handles the leaderboard screen's rendering.
 *
 * @author Arthur
 * @date 21/05/16 - 13/01/19
 */
class LeaderboardView: public AbstractView
{
public:
    //=== CTORs / DTORs
    LeaderboardView(sf::RenderWindow* window, AppTextManager* textManager, LeaderboardModel* leaderboardModel);
    ~LeaderboardView() override;

    //=== METHODS
    void synchronize() override;
    void draw() const override;
    bool handleEvents(sf::Event& event) override;

private:
    //=== ATTRIBUTES
    LeaderboardModel* m_leaderboard;

    //UI components
    mdsf::RaisedButton* m_homeButton;
    mdsf::RaisedButton* m_clearLeaderboardButton;
    mdsf::Dialog* m_confirmDialog;

    //Constants
    const int HOME_BTN_SIZE = 30;
    const int DIALOG_WIDTH = 280;
    const int DIALOG_HEIGHT = 150;
    const int RESET_BTN_WIDTH = 150;
    const int RESET_BTN_HEIGHT = 36;

    //=== METHODS
    void loadSprites() override;
    float getDialogXPosition(int width) const;
    float getDialogYPosition(int height) const;
};

#endif
