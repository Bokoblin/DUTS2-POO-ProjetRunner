#include "GameView.h"
#include "libs/MDC-SFML/src/ClipRectUtils.h"

using std::string;
using std::vector;
using Bokoblin::SimpleLogger::Logger;
namespace ViewRes = Bokoblin::BokoRunner::Resources::View;

//------------------------------------------------
//          CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------

/**
 * @brief Constructor
 *
 * @param window the app window
 * @param textManager the text manager
 * @param gameModel the game model counterpart
 *
 * @author Arthur
 * @date 26/03/2016 - 11/09/2018
 */
GameView::GameView(sf::RenderWindow* window, AppTextManager* textManager, GameModel* gameModel) :
        AbstractView(window, textManager), m_game{gameModel},
        m_xPixelIntensity{INITIAL_PIXEL_INTENSITY}, m_yPixelIntensity{INITIAL_PIXEL_INTENSITY}
{
    GameView::loadSprites();
    m_pixelShader = new PixelShader();

    //=== change default game music if in master mode

    string game_music = (m_game->getGameDifficulty() == EASY)
            ? ViewRes::GAME_MUSIC_THEME_EASY_MODE
            : ViewRes::GAME_MUSIC_THEME_HARD_MODE;

    if (!m_gameThemeMusic.openFromFile(game_music)) {
        Logger::printError("Music loading failed for \"" + game_music + "\"");
    } else {
        handleMusic();
        m_gameThemeMusic.play();
        m_gameThemeMusic.setLoop(true);
    }

    if (!m_coinMusic.openFromFile(ViewRes::COINS_COLLECTED_MUSIC)) {
        Logger::printError("Music loading failed for \"" + ViewRes::COINS_COLLECTED_MUSIC + "\"");
    }

    if (!m_destructedEnemiesMusic.openFromFile(ViewRes::ENEMIES_DESTRUCTED_MUSIC)) {
        Logger::printError("Music loading failed for \"" + ViewRes::ENEMIES_DESTRUCTED_MUSIC + "\"");
    }
}

/**
 * @brief Destructor
 *
 * @author Arthur
 * @date 26/03/2016 - 13/07/2019
 */
GameView::~GameView()
{
    //=== Delete Game Element

    delete m_parallaxBackground;
    delete m_farTransitionBackground;
    delete m_bottomBarImage;
    delete m_lifeBoxImage;
    delete m_remainingLifeImage;
    delete m_shieldImage;
    delete m_pixelShader;
    for (auto& it : m_typeToSpriteMap) {
        delete it.second;
    }
    for (auto& it : m_movableElementToSpriteMap) {
        delete it.second;
    }

    //=== Delete Pause and End Elements

    delete m_pauseBackground;
    delete m_distanceIcon;
    delete m_endBackground;
    delete m_resumeButton;
    delete m_restartButton;
    delete m_goHomeButton;
    delete m_musicButton;
    delete m_saveScoreButton;

    //=== Remove game reference

    m_game = nullptr; //Model memory mustn't be freed by the view
}

//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * @brief Load all sprites used by the game (backgrounds, UI, elements)
 *
 * @author Arthur
 * @date 26/03/2016 - 03/05/2020
 */
void GameView::loadSprites()
{
    //=== Initialize backgrounds

    m_parallaxBackground = new ParallaxBackground(1.33f * m_width, m_height);
    m_parallaxBackground->addBackground(SCROLLING_BACKGROUND, 1, ViewRes::GAME_FAR_HILL_BACKGROUND);
    m_parallaxBackground->addBackground(SCROLLING_FOREGROUND, 2, ViewRes::GAME_NEAR_HILL_BACKGROUND);

    m_farTransitionBackground = new mdsf::Sprite(m_width, m_height, m_width, m_height, ViewRes::GAME_FAR_T1_BACKGROUND);
    m_bottomBarImage = new mdsf::Sprite(0, 0.87f * m_height, m_width, m_height, ViewRes::BOTTOM_BAR_IMAGE);
    m_bottomBarImage->resize(m_width, m_height);

    m_pauseBackground = new mdsf::Sprite(0, 0, m_width, m_height, ViewRes::PAUSE_HILL_BACKGROUND);
    m_pauseBackground->resize(m_width, m_height);
    m_endBackground = new mdsf::Sprite(0, 0, m_width, m_height, ViewRes::END_SCREEN_BACKGROUND);
    m_endBackground->resize(m_width, m_height);

    //=== Initialize UI elements

    m_lifeBoxImage = new mdsf::Sprite(0.117f * m_width, 0.89f * m_height, LIFE_BOX_WIDTH, LIFE_BOX_HEIGHT);
    m_lifeBoxImage->loadAndApplyTextureFromImageFile(ViewRes::LIFE_BOX_IMAGE, sf::IntRect(0, 0, 300, 50));

    m_remainingLifeImage = new mdsf::Sprite(0.118f * m_width, 0.89f * m_height, LIFE_LEVEL_WIDTH, LIFE_LEVEL_HEIGHT);
    m_remainingLifeImage->loadAndApplyTextureFromImageFile(ViewRes::LIFE_BOX_IMAGE, sf::IntRect(0, 51, 300, 50));

    m_distanceIcon = new mdsf::Sprite(0.033f * m_width, 0.055f * m_height, ORIGINAL_DISTANCE_ICON_SIZE);
    m_distanceIcon->loadAndApplyTextureFromImageFile(ViewRes::FLAG_IMAGE);
    m_distanceIcon->resize(25); //TODO [2.0.x] Move resize call to ctor/sync() after setting the target size in ctor
    m_distanceIcon->setFillColor(mdsf::Color::MaterialOrange);

    //=== Initialize PLAYER sprite

    int playerYOffset = m_game->getPlayerSkin() == "moblin" ? 0 : (m_game->getPlayerSkin() == "morphing" ? 50 : 100);
    auto playerClipRect = mdsf::ClipRectUtils::generate(0, playerYOffset, 50, 50, 1, NB_PLAYER_CLIPS);
    auto playerSprite = new AnimatedSprite(30, 30, ViewRes::PLAYER_IMAGE, playerClipRect);
    playerSprite->setOrigin(0, 50);

    //=== Initialize ENEMIES sprite

    auto stdEnemyClipRect = mdsf::ClipRectUtils::generate(0, 0, 50, 50, 1, 2);
    m_stdEnemySprite = new AnimatedSprite(30, 30, ViewRes::ENEMIES_IMAGE, stdEnemyClipRect);
    m_stdEnemySprite->setOrigin(0, 50);

    auto totemEnemyClipRect = mdsf::ClipRectUtils::generate(0, 0, 50, 150, 1, 2);
    auto totemEnemySprite = new AnimatedSprite(30, 90, ViewRes::ENEMIES_IMAGE, totemEnemyClipRect);
    totemEnemySprite->setOrigin(0, 150);

    auto blockEnemyClipRect = mdsf::ClipRectUtils::generate(0, 150, 50, 50, 1, 2);
    auto blockEnemySprite = new AnimatedSprite(50, 50, ViewRes::ENEMIES_IMAGE, blockEnemyClipRect);
    blockEnemySprite->setOrigin(0, 50);

    //=== Initialize COINS & BONUSES sprite

    m_shieldImage = new mdsf::Sprite(SHIELD_SIZE, ViewRes::SHIELD_IMAGE);
    m_shieldImage->setOrigin(0, 50);

    auto coinClipRect = mdsf::ClipRectUtils::generate(0, 0, 50, 50, 1, 5);
    m_coinSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, coinClipRect);
    m_coinSprite->setOrigin(0, 50);

    auto pvClipRect = mdsf::ClipRectUtils::generate(0, 50, 50, 50, 1, 5);
    auto pvPlusBonusSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, pvClipRect);
    pvPlusBonusSprite->setOrigin(0, 50);

    auto megaClipRect = mdsf::ClipRectUtils::generate(0, 100, 50, 50, 1, 5);
    auto megaBonusSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, megaClipRect);
    megaBonusSprite->setOrigin(0, 50);

    auto flyClipRect = mdsf::ClipRectUtils::generate(0, 150, 50, 50, 1, 5);
    auto flyBonusSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, flyClipRect);
    flyBonusSprite->setOrigin(0, 50);

    auto slowClipRect = mdsf::ClipRectUtils::generate(0, 200, 50, 50, 1, 5);
    auto slowSpeedBonusSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, slowClipRect);
    slowSpeedBonusSprite->setOrigin(0, 50);

    auto shieldClipRect = mdsf::ClipRectUtils::generate(0, 250, 50, 50, 1, 5);
    auto shieldBonusSprite = new AnimatedSprite(25, 25, ViewRes::BONUS_IMAGE, shieldClipRect);
    shieldBonusSprite->setOrigin(0, 50);

    //=== Initialize buttons

    m_resumeButton = new mdsf::ToggleButton(PAUSE_FORM_X, 0.592f * m_height, PAUSE_ICONS_SIZE, PAUSE_ICONS_SIZE,
                                            "pause_resume", ViewRes::RESUME_BUTTON_IMAGE);
    m_resumeButton->resize(PAUSE_BUTTONS_SIZE);
    m_resumeButton->retrieveLabel(LocalizationManager::fetchLocalizedString);

    m_restartButton = new mdsf::ToggleButton(PAUSE_FORM_X, 0.675f * m_height, PAUSE_ICONS_SIZE, PAUSE_ICONS_SIZE,
                                             "pause_restart", ViewRes::RESTART_BUTTON_IMAGE);
    m_restartButton->resize(PAUSE_BUTTONS_SIZE);
    m_restartButton->retrieveLabel(LocalizationManager::fetchLocalizedString);

    m_goHomeButton = new mdsf::ToggleButton(PAUSE_FORM_X, 0.758f * m_height, PAUSE_ICONS_SIZE, PAUSE_ICONS_SIZE,
                                            "pause_go_to_home", ViewRes::HOME_BUTTON_IMAGE);
    m_goHomeButton->resize(PAUSE_BUTTONS_SIZE);
    m_goHomeButton->retrieveLabel(LocalizationManager::fetchLocalizedString);

    m_musicButton = new mdsf::ToggleButton(PAUSE_FORM_X, 0.9f * m_height, PAUSE_ICONS_SIZE, PAUSE_ICONS_SIZE,
                                           "pause_music", ViewRes::MUSIC_BUTTON_IMAGE);
    m_musicButton->resize(PAUSE_BUTTONS_SIZE);
    m_musicButton->retrieveLabel(LocalizationManager::fetchLocalizedString);

    m_saveScoreButton = new mdsf::RaisedButton(getHalfXPosition() - (0.5f * BUTTON_WIDTH), 0.72f * m_height,
                                               BUTTON_WIDTH, BUTTON_HEIGHT, "end_save_button");
    m_saveScoreButton->retrieveLabel(LocalizationManager::fetchLocalizedString);
    m_saveScoreButton->setFillColor(mdsf::Color::MaterialGreenA700);


    //=== Associate element type to sprite

    m_typeToSpriteMap[PLAYER] = playerSprite;
    m_typeToSpriteMap[STANDARD_ENEMY] = m_stdEnemySprite;
    m_typeToSpriteMap[TOTEM_ENEMY] = totemEnemySprite;
    m_typeToSpriteMap[BLOCK_ENEMY] = blockEnemySprite;
    m_typeToSpriteMap[COIN] = m_coinSprite;
    m_typeToSpriteMap[PV_PLUS_BONUS] = pvPlusBonusSprite;
    m_typeToSpriteMap[MEGA_BONUS] = megaBonusSprite;
    m_typeToSpriteMap[FLY_BONUS] = flyBonusSprite;
    m_typeToSpriteMap[SLOW_SPEED_BONUS] = slowSpeedBonusSprite;
    m_typeToSpriteMap[SHIELD_BONUS] = shieldBonusSprite;
}

/**
 * @brief Synchronize game elements
 *
 * @author Arthur
 * @date 26/03/2016 - 30/01/2017
 */
void GameView::synchronize()
{
    switch (m_game->getGameState()) {
        case RUNNING :
        case RUNNING_SLOWLY :
            linkElements();
            deleteElements();
            updateRunningGameElements();
            m_textManager->syncGameRunningText(m_game->getBonusTimeout());
            break;
        case PAUSED:
            updatePausedGameElements();
            m_textManager->syncGamePausedText();
            sf::sleep(sf::milliseconds(140)); //limit CPU usage for this thread
            break;
        case OVER:
            //Stop game music if still playing
            if (m_gameThemeMusic.getStatus() == sf::Music::Status::Playing) {
                m_gameThemeMusic.stop();
            }
            updateGameOverElements();
            m_textManager->syncGameOverText(static_cast<int>(m_game->getGameSpeed()));
            break;
        default:
            break;
    }
}


/**
 * @brief Draw game elements on the window
 *
 * @author Arthur
 * @date 26/03/2016 - 24/12/2017
 */
void GameView::draw() const
{
    m_window->clear();

    switch (m_game->getGameState()) {
        case RUNNING :
        case RUNNING_SLOWLY :
            drawRunningGame();
            break;
        case PAUSED:
            drawPausedGame();
            break;
        case OVER:
            drawGameOver();
            break;
        default:
            break;
    }

    m_window->display();
}

/**
 * @brief Handle music settings
 *
 * @author Arthur
 * @date 25/01/2017 - 02/05/2020
 */
void GameView::handleMusic()
{
    //change music volume
    if (m_game->isMusicEnabled()) {
        m_gameThemeMusic.setVolume(100);
        m_coinMusic.setVolume(100);
        m_destructedEnemiesMusic.setVolume(100);
    } else {
        m_gameThemeMusic.setVolume(0);
        m_coinMusic.setVolume(0);
        m_destructedEnemiesMusic.setVolume(0);
    }
}


/**
 * @brief Handle players inputs
 *
 * @author Arthur
 * @date 26/12/2017 - 30/10/2018
 */
void GameView::handlePlayerInput() const
{
    if (m_game->getGameState() == RUNNING ||
            m_game->getGameState() == RUNNING_SLOWLY) {
        //=== Player Controls in Game Screen

        if (EventUtils::wasKeyboardLeftPressed()) {
            m_game->getPlayer()->controlPlayerMovements(MOVE_LEFT);
        } else if (EventUtils::wasKeyboardRightPressed()) {
            m_game->getPlayer()->controlPlayerMovements(MOVE_RIGHT);
        }

        if (EventUtils::wasKeyboardJumpPressed()) {
            m_game->getPlayer()->jump();
        }
    }
}

/**
 * @brief Handle the user interaction events (mouse, keyboard, title bar buttons)
 *
 * @param event sfml event object
 * @return true if app state is unchanged
 *
 * @author Arthur, Florian
 * @date 21/02/2016 - 13/01/2019
 */
bool GameView::handleEvents(sf::Event& event)
{
    handlePlayerInput();

    while (m_window->pollEvent(event)) {
        //=== Window event handling

        if (event.type == sf::Event::Closed) {
            m_game->setAppState(QUIT);
            return false;
        }

        //=== Pause opening/quitting handling

        if (EventUtils::wasKeyboardEscapePressed(event)) {
            switch (m_game->getGameState()) {
                case RUNNING :
                case RUNNING_SLOWLY :
                    m_game->setGameState(PAUSED);
                    m_gameThemeMusic.pause();
                    break;
                case PAUSED:
                    m_game->setGameState(RUNNING_SLOWLY);
                    m_gameThemeMusic.play();
                    break;
                default:
                    break;
            }
        }

        //=== Specific game state handling

        switch (m_game->getGameState()) {
            case RUNNING :
            case RUNNING_SLOWLY :
                return handleRunningGameEvents(event);
            case PAUSED:
                return handlePausedGameEvents(event);
            case OVER:
                return handleGameOverEvents(event);
            default:
                break;
        }
    }
    return true;
}
