#include "GameView.h"

namespace ViewResources = Bokoblin::BokoRunner::Resources::View;

//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * @brief Link a model movable element to new sprite matching its type
 *
 * @author Arthur
 * @date 18/03/2016 - 24/12/2017
 */
void GameView::linkElements()
{
    for (auto& element : m_game->getNewMElementsArray()) {
        m_movableElementToSpriteMap[element] = new AnimatedSprite(*(m_typeToSpriteMap[element->getType()]));
    }

    m_game->clearNewMovableElementList();
}

/**
 * @brief Process the transition between zones
 *
 * @author Arthur
 * @date 25/04/2016 - 14/07/2019
 */
void GameView::processZonesTransition()
{
    //=== [Always] Set background speed and position

    m_farTransitionBackground->setPosition(m_farTransitionBackground->getX() - TRANSITION_SPEED, 0);
    m_parallaxBackground->getBackground(SCROLLING_BACKGROUND)->setScrollingSpeed(TRANSITION_SPEED);
    m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->decreaseAlpha(5);
    if (m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->getAlpha() == 0) {
        m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->setScrollingSpeed(0);
    } else {
        m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->setScrollingSpeed(TRANSITION_SPEED);
    }

    //=== [Transition half only] Update zone background image and position

    if (m_farTransitionBackground->getX() <= 5
            && m_farTransitionBackground->getX() >= -5) {
        if (m_game->getCurrentZone() == HILL) {
            m_parallaxBackground->getBackground(SCROLLING_BACKGROUND)->
                    loadAndApplyTextureFromImageFile(ViewResources::GAME_FAR_PLAIN_BACKGROUND);
            m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->
                    loadAndApplyTextureFromImageFile(ViewResources::GAME_NEAR_PLAIN_BACKGROUND);
        } else {
            m_parallaxBackground->getBackground(SCROLLING_BACKGROUND)->
                    loadAndApplyTextureFromImageFile(ViewResources::GAME_FAR_HILL_BACKGROUND);
            m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->
                    loadAndApplyTextureFromImageFile(ViewResources::GAME_NEAR_HILL_BACKGROUND);
        }

        m_parallaxBackground->getBackground(SCROLLING_BACKGROUND)->setPositions(-300, 0);
        m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->setPositions(0, 0);
    }

    //=== [Transition 3/4 until end] Update pixel creation of near background

    if (m_farTransitionBackground->getX() < getHalfXPosition() && m_xPixelIntensity >= 0) {
        m_xPixelIntensity -= 0.009;
        m_yPixelIntensity -= 0.009;
        m_pixelShader->update(m_xPixelIntensity, m_yPixelIntensity);
    }

    //=== [Transition end]

    if (m_farTransitionBackground->getX()
            + m_farTransitionBackground->getLocalBounds().width <= 0) {
        //Update Transition status
        m_game->setTransitionState(false);
        m_game->disableTransitionPossibility();

        //Set current zone and change pause background
        if (m_game->getCurrentZone() == HILL) {
            m_game->setCurrentZone(PLAIN);
            m_pauseBackground->loadAndApplyTextureFromImageFile(ViewResources::PAUSE_PLAIN_BACKGROUND);
        } else {
            m_game->setCurrentZone(HILL);
            m_pauseBackground->loadAndApplyTextureFromImageFile(ViewResources::PAUSE_HILL_BACKGROUND);
        }
    }
}

/**
* @brief Setup the transition between zones
*
* @author Arthur
* @date 25/04/2016 - 13/07/2019
*/
void GameView::setupTransition()
{
    m_game->setTransitionState(true);
    m_xPixelIntensity = 1;
    m_yPixelIntensity = 1;
    m_farTransitionBackground->setPosition(m_parallaxBackground->getBackground(
            SCROLLING_BACKGROUND)->getLeftPosition().x + 1200, 0);

    if (m_game->getCurrentZone() == HILL) {
        m_pixelShader->load(ViewResources::GAME_NEAR_T1_BACKGROUND);
        m_farTransitionBackground->loadAndApplyTextureFromImageFile(ViewResources::GAME_FAR_T1_BACKGROUND);
    } else {
        m_pixelShader->load(ViewResources::GAME_NEAR_T2_BACKGROUND);
        m_farTransitionBackground->loadAndApplyTextureFromImageFile(ViewResources::GAME_FAR_T2_BACKGROUND);
    }
}

/**
 * @brief Update elements of a running game
 *
 * @author Arthur
 * @date 6/03/2016 - 13/07/2019
 */
void GameView::updateRunningGameElements()
{
    //=== Handle Transitions between zones

    if (m_game->isTransitionRunning()) {
        processZonesTransition();
    } else {
        m_parallaxBackground->getBackground(SCROLLING_BACKGROUND)->setScrollingSpeed(0.5f * m_game->getGameSpeed());
        m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->setScrollingSpeed(m_game->getGameSpeed());
        m_parallaxBackground->getBackground(SCROLLING_FOREGROUND)->setAlpha(255);

        if (m_game->isTransitionPossible() && m_parallaxBackground->getBackground(
                SCROLLING_BACKGROUND)->getSeparationPositionX(m_width) > m_width - 100) {
            setupTransition();
        }
    }

    //=== Update Game Elements

    m_parallaxBackground->sync();

    m_remainingLifeImage->resize(0.01f * LIFE_LEVEL_WIDTH * m_game->getPlayer()->getLife(), LIFE_LEVEL_HEIGHT);

    for (auto& it : m_movableElementToSpriteMap) {
        m_game->moveMovableElement(it.first);

        float position_x = (it.first)->getPosX();
        float position_y = (it.first)->getPosY();

        it.second->setPosition(position_x, position_y);
        it.second->sync();
        it.second->resize(it.first->getWidth(), it.first->getHeight());
    }

    //=== Update shield sprite

    if (m_game->getPlayer()->getState() == SHIELDED || m_game->getPlayer()->getState() == HARD_SHIELDED) {
        if (m_game->getPlayer()->getState() == SHIELDED) {
            m_shieldImage->setTextureRect(sf::IntRect(0, 0, 50, 50));
        } else {
            m_shieldImage->setTextureRect(sf::IntRect(50, 0, 50, 50));
        }

        m_shieldImage->setPosition(m_game->getPlayer()->getPosX() - 5, m_game->getPlayer()->getPosY() + 5);
        m_shieldImage->resize(SHIELD_SIZE);
    }
}

/**
 * @brief Deletes the Sprite that collided with the player
 * @details We consider that the player only collide with one element per complete loop,
 * it allows to reduce average complexity instead doing n loops each time (worst case)
 *
 * @author Arthur
 * @date 12/03/2016 - 20/03/2016
 */
void GameView::deleteElements()
{
    auto it = m_movableElementToSpriteMap.begin();
    bool found = false;

    while (!found && it != m_movableElementToSpriteMap.end()) {
        if ((it->first)->isColliding()) {
            if ((it->first)->getType() == COIN) {
                m_coinMusic.play();
            }

            if ((it->first)->getType() == STANDARD_ENEMY
                    || (it->first)->getType() == TOTEM_ENEMY
                    || (it->first)->getType() == BLOCK_ENEMY) {
                m_destructedEnemiesMusic.play();
            }

            delete it->second;
            m_movableElementToSpriteMap.erase(it);
            found = true;
        } else {
            ++it;
        }
    }
}

/**
 * @brief Draw elements of a running game
 *
 * @author Arthur
 * @date 24/12/2017 - 13/07/2019
 */
void GameView::drawRunningGame() const
{
    //=== Standalone Sprites drawing

    m_parallaxBackground->draw(m_window);

    if (m_game->isTransitionRunning()) {
        m_window->draw(*m_farTransitionBackground);
        if (m_farTransitionBackground->getX() < getHalfXPosition()) {
            m_window->draw(*m_pixelShader);
        }
    }

    m_window->draw(*m_bottomBarImage);
    m_window->draw(*m_remainingLifeImage);
    m_window->draw(*m_lifeBoxImage);

    //=== Array's Sprites drawing

    for (const auto& it : m_movableElementToSpriteMap) {
        it.second->draw(m_window);
    }

    if (m_game->getPlayer()->getState() == SHIELDED || m_game->getPlayer()->getState() == HARD_SHIELDED) {
        m_window->draw(*m_shieldImage);
    }

    //=== Standalone Text drawing

    m_textManager->drawGameRunningText(m_window);
}

/**
 * @brief Handle running game events
 *
 * @param event sfml event object
 * @return true if app state is unchanged
 *
 * @author Arthur
 * @date 26/12/2017 - 13/01/2019
 */
bool GameView::handleRunningGameEvents(const sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased) {
        m_game->getPlayer()->decelerate();
    }
    return true;
}
