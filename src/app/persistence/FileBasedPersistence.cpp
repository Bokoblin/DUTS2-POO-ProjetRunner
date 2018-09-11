#include "FileBasedPersistence.h"
#include "app/model/utils/StringHash.h"

using std::string;
using std::to_string;
using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;
using Bokoblin::SimpleLogger::Logger;

//------------------------------------------------
//          STATIC MEMBERS
//------------------------------------------------

AppCore* FileBasedPersistence::m_appCore = nullptr;
xml_document FileBasedPersistence::m_doc;


//------------------------------------------------
//          GETTERS
//------------------------------------------------

AppCore* FileBasedPersistence::getAppCore() { return m_appCore; }


//------------------------------------------------
//          SETTERS
//------------------------------------------------

void FileBasedPersistence::setAppCore(AppCore* appCore) { m_appCore = appCore; }

//------------------------------------------------
//          PUBLIC METHODS
//------------------------------------------------

/**
 * @brief Creates the config file if it doesn't exit
 *
 * @author Arthur
 * @date 02/05/16 - 17/07/18
 */
bool FileBasedPersistence::createConfigFile()
{
    nullSafeGuard();

    bool success = false;
    if (XMLHelper::checkXMLFileExistence(m_appCore->getConfigFile())) {
        Logger::printWarningOnConsole("Configuration file already exists, aborting...");
    } else if (XMLHelper::createXMLFile(m_appCore->getConfigFile(), DEFAULT_CONFIG_CONTENT)) {
        Logger::printInfoOnConsole("Configuration file successfully created");
        success = true;
    } else {
        Logger::printErrorOnConsole("Configuration file creation failure");
    }

    return success;
}

bool FileBasedPersistence::loadConfigFile()
{
    nullSafeGuard();
    return XMLHelper::loadXMLFile(m_doc, m_appCore->getConfigFile());
}

bool FileBasedPersistence::removeConfigFile()
{
    nullSafeGuard();
    return XMLHelper::removeXMLFile(m_appCore->getConfigFile());
}

/**
 * @brief Checks a stream integrity
 * @details Checks if a stream is corrupted
 * by verifying number of lines, number of each item
 *
 * @return a boolean indicating if file is corrupted
 *
 * @author Arthur
 * @date 02/05/16 - 22/07/18
 */
bool FileBasedPersistence::checkStreamIntegrity(std::istream& stream)
{
    string content;
    string line;

    //=== Count lines / elements test
    bool isPresentConfig = false; //should be true
    bool isPresentStats = false; //should be true
    bool isPresentShop = false; //should be true
    bool isPresentScoreEasy = false; //should be true
    bool isPresentScoreHard = false; //should be true
    int nbLines = 0; //should be 52 or 53
    int nbConfigChildren = 0; //should be 6
    int nbStatsChildren = 0; //should be 7
    int nbShopChildren = 0; //should be 6
    int nbScoreChildren = 0; //should be 20

    do {
        getline(stream, line, '\n');
        content.append(line);
        nbLines++;

        isPresentConfig = line.find("<config>") != string::npos ? true : isPresentConfig;
        isPresentStats = line.find("<stats>") != string::npos ? true : isPresentStats;
        isPresentShop = line.find("<shop>") != string::npos ? true : isPresentShop;
        isPresentScoreEasy = line.find("<scoresEasy>") != string::npos ? true : isPresentScoreEasy;
        isPresentScoreHard = line.find("<scoresHard>") != string::npos ? true : isPresentScoreHard;

        nbConfigChildren = line.find("<configItem") != string::npos ? nbConfigChildren + 1 : nbConfigChildren;
        nbStatsChildren = line.find("<statItem") != string::npos ? nbStatsChildren + 1 : nbStatsChildren;
        nbShopChildren = line.find("<shopItem") != string::npos ? nbShopChildren + 1 : nbShopChildren;
        nbScoreChildren = line.find("<scoreItem") != string::npos ? nbScoreChildren + 1 : nbScoreChildren;
    } while (!stream.eof());

    //To overcome the limitation of "stream" being emptied by XMLHelper::checkXMLStreamIntegrity()
    std::stringstream ss;
    ss << content.c_str();
    if (!XMLHelper::checkXMLStreamIntegrity(ss)) {
        return false;
    }

    return isPresentConfig && isPresentStats && isPresentShop && isPresentScoreEasy && isPresentScoreHard
            && nbConfigChildren == 6 && nbStatsChildren == 7 && nbShopChildren == 6 && nbScoreChildren == 20
            && (nbLines == 52 || nbLines == 53);
}


/**
 * @brief Checks config file integrity
 *
 * @return a boolean indicating if file is corrupted
 *
 * @author Arthur
 * @date 02/05/16 - 17/07/18
 */
bool FileBasedPersistence::checkStreamIntegrityFromConfigFile()
{
    nullSafeGuard();

    std::fstream f;
    f.open(m_appCore->getConfigFile().c_str(), std::ios::in);

    if (!f.fail()) {
        return checkStreamIntegrity(f);
    } else {
        return false;
    }
}


/**
 * @brief Checks XML document integrity
 *
 * @return a boolean indicating if file is corrupted
 *
 * @author Arthur
 * @date 17/07/2018
 */
bool FileBasedPersistence::checkStreamIntegrityFromXMLDocument()
{
    std::stringstream stream;
    m_doc.save(stream);

    return checkStreamIntegrity(stream);
}


/**
 * @brief Updates configuration values from file
 *
 * @author Arthur
 * @date 24/10/16 - 26/01/18
 */
bool FileBasedPersistence::fetchConfigurationFromConfigFile()
{
    nullSafeGuard();

    xml_node config = m_doc.child("runner").child("config");

    for (xml_node configItem: config.children("configItem")) {
        string nodeKey = string(configItem.attribute("name").value());
        xml_attribute nodeValue = configItem.attribute("value");

        switch (hash(nodeKey)) {
            case hash("language"):
                m_appCore->m_currentLanguage = XMLHelper::safeRetrieveXMLValue<string>
                        (nodeValue, "en|fr|es", ENGLISH);
                break;
            case hash("difficulty"):
                m_appCore->m_currentDifficulty = XMLHelper::safeRetrieveXMLValue<int>
                        (nodeValue, "1|2", Difficulty::HARD);
                break;
            case hash("ball_skin"):
                m_appCore->m_currentBallSkin = XMLHelper::safeRetrieveXMLValue<string>
                        (nodeValue, "default|morphing|capsule", "default");
                break;
            case hash("wallet"):
                m_appCore->m_wallet = XMLHelper::safeRetrieveXMLValue<int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("menu_music"):
                m_appCore->m_isMenuMusicEnabled = XMLHelper::safeRetrieveXMLValue<bool>
                        (nodeValue, BOOLEAN_REGEX, false);
                break;
            case hash("game_music"):
                m_appCore->m_isGameMusicEnabled = XMLHelper::safeRetrieveXMLValue<bool>
                        (nodeValue, BOOLEAN_REGEX, false);
                break;
            default:
                break;
        }
    }

    return true;
}


/**
 * @brief Updates statistics values from file
 *
 * @author Arthur
 * @date 24/10/16 - 26/01/18
 */
bool FileBasedPersistence::fetchStatisticsFromConfigFile()
{
    nullSafeGuard();

    xml_node stats = m_doc.child("runner").child("stats");

    for (xml_node statItem: stats.children("statItem")) {
        string nodeKey = string(statItem.attribute("name").value());
        xml_attribute nodeValue = statItem.attribute("value");

        switch (hash(nodeKey)) {
            case hash("total_coins_collected"):
                m_appCore->m_totalCoinsCollected = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("total_distance_travelled"):
                m_appCore->m_totalDistance = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("total_enemies_destroyed"):
                m_appCore->m_totalFlattenedEnemies = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("per_game_coins_collected"):
                m_appCore->m_perGameCoinsCollected = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("per_game_distance_travelled"):
                m_appCore->m_perGameDistance = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("per_game_enemies_destroyed"):
                m_appCore->m_perGameFlattenedEnemies = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            case hash("total_games_played"):
                m_appCore->m_totalGamesPlayed = XMLHelper::safeRetrieveXMLValue<unsigned int>
                        (nodeValue, INTEGER_REGEX, 0);
                break;
            default:
                break;
        }
    }

    return true;
}


/**
 * @brief Updates each score array
 * with values from config file
 *
 * @author Arthur
 * @date 23/10/16 - 26/01/18
 */
bool FileBasedPersistence::fetchLeaderboardFromConfigFile()
{
    nullSafeGuard();

    xml_node scoresEasy = m_doc.child("runner").child("scoresEasy");
    xml_node scoresHard = m_doc.child("runner").child("scoresHard");

    m_appCore->m_scoresEasyArray.clear();
    m_appCore->m_scoresHardArray.clear();

    for (xml_node scoreItem: scoresEasy.children("scoreItem")) {
        if (string(scoreItem.attribute("value").value()) != "0") {
            m_appCore->m_scoresEasyArray.insert(XMLHelper::safeRetrieveXMLValue<unsigned int>
                                                        (scoreItem.attribute("value"), INTEGER_REGEX, 0));
        }
    }
    for (xml_node scoreItem: scoresHard.children("scoreItem")) {
        if (string(scoreItem.attribute("value").value()) != "0") {
            m_appCore->m_scoresHardArray.insert(XMLHelper::safeRetrieveXMLValue<unsigned int>
                                                        (scoreItem.attribute("value"), INTEGER_REGEX, 0));
        }
    }

    return true;
}


/**
 * @brief Updates activated items array
 * with values from config file
 *
 * @author Arthur
 * @date 14/05/16 - 26/01/18
 */
bool FileBasedPersistence::fetchActivatedBonusFromConfigFile()
{
    nullSafeGuard();

    xml_node shop = m_doc.child("runner").child("shop");

    for (xml_node shopItem: shop.children("shopItem")) {
        if (string(shopItem.attribute("bought").value()) == "true") {
            const string ITEM_REGEX = "shop_[a-z]+[_]{0,1}[a-z]+";
            m_appCore->m_activatedItemsArray.insert(XMLHelper::safeRetrieveXMLValue<string>
                                                            (shopItem.attribute("id"), ITEM_REGEX, ""));
        }
    }
    return true;
}


/**
 * @brief Pushes Configuration data to file
 *
 * @author Arthur
 * @date 02/05/16 - 26/01/18
 */
bool FileBasedPersistence::persistConfigurationToConfigFile()
{
    nullSafeGuard();

    xml_node config = m_doc.child("runner").child("config");
    xml_node stats = m_doc.child("runner").child("stats");
    xml_node shop = m_doc.child("runner").child("shop");
    xml_node scoresEasy = m_doc.child("runner").child("scoresEasy");
    xml_node scoresHard = m_doc.child("runner").child("scoresHard");

    //=== Save configuration

    for (xml_node configItem: config.children("configItem")) {
        string nodeKey = string(configItem.attribute("name").value());
        xml_attribute nodeValue = configItem.attribute("value");

        switch (hash(nodeKey)) {
            case hash("language"):
                nodeValue.set_value(m_appCore->m_currentLanguage.c_str());
                break;
            case hash("difficulty"):
                nodeValue.set_value(to_string(m_appCore->m_currentDifficulty).c_str());
                break;
            case hash("ball_skin"):
                nodeValue.set_value(m_appCore->m_currentBallSkin.c_str());
                break;
            case hash("wallet"):
                nodeValue.set_value(to_string(m_appCore->m_wallet).c_str());
                break;
            case hash("menu_music"):
                nodeValue.set_value(m_appCore->m_isMenuMusicEnabled);
                break;
            case hash("game_music"):
                nodeValue.set_value(m_appCore->m_isGameMusicEnabled);
                break;
            default:
                break;
        }
    }

    //=== Save stats

    for (xml_node statItem: stats.children("statItem")) {
        string nodeKey = string(statItem.attribute("name").value());
        xml_attribute nodeValue = statItem.attribute("value");

        switch (hash(nodeKey)) {
            case hash("total_coins_collected"):
                nodeValue.set_value(to_string(m_appCore->m_totalCoinsCollected).c_str());
                break;
            case hash("total_distance_travelled"):
                nodeValue.set_value(to_string(m_appCore->m_totalDistance).c_str());
                break;
            case hash("total_enemies_destroyed"):
                nodeValue.set_value(to_string(m_appCore->m_totalFlattenedEnemies).c_str());
                break;
            case hash("per_game_coins_collected"):
                nodeValue.set_value(to_string(m_appCore->m_perGameCoinsCollected).c_str());
                break;
            case hash("per_game_distance_travelled"):
                nodeValue.set_value(to_string(m_appCore->m_perGameDistance).c_str());
                break;
            case hash("per_game_enemies_destroyed"):
                nodeValue.set_value(to_string(m_appCore->m_perGameFlattenedEnemies).c_str());
                break;
            case hash("total_games_played"):
                nodeValue.set_value(to_string(m_appCore->m_totalGamesPlayed).c_str());
                break;
            default:
                break;
        }
    }

    //=== Save shop activated items

    for (xml_node shopItem: shop.children("shopItem")) {
        if (m_appCore->findActivatedItem(string(shopItem.attribute("id").value()))) {
            shopItem.attribute("bought").set_value(true);
        }
    }

    //=== Save leaderboard

    auto it = m_appCore->m_scoresEasyArray.begin();
    for (xml_node scoreItem: scoresEasy.children("scoreItem")) {
        xml_attribute nodeValue = scoreItem.attribute("value");
        if (it != m_appCore->m_scoresEasyArray.end()) {
            nodeValue.set_value(to_string(*it).c_str());
            ++it;
        } else {
            nodeValue.set_value("0");
        }
    }

    it = m_appCore->m_scoresHardArray.begin();
    for (xml_node scoreItem: scoresHard.children("scoreItem")) {
        xml_attribute nodeValue = scoreItem.attribute("value");
        if (it != m_appCore->m_scoresHardArray.end()) {
            nodeValue.set_value(to_string(*it).c_str());
            ++it;
        } else {
            nodeValue.set_value("0");
        }
    }

    return m_doc.save_file(m_appCore->getConfigFile().c_str());
}


//------------------------------------------------
//          PRIVATE METHODS
//------------------------------------------------

void FileBasedPersistence::nullSafeGuard()
{
    if (m_appCore == nullptr) {
        Logger::printErrorOnConsole("Illegal usage of FileBasedPersistence class");
        exit(EXIT_FAILURE);
    }
}