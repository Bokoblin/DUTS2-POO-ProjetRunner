#include "Dialog.h"

using namespace std;


Dialog::Dialog() : GraphicElement(0, 0)
{
    hide();
}


/**
 * Constructs a dialog with x and y coordinates,
 * a textHandler and a description
 * @author Arthur
 * @date 02/01/2017 - 07/01/17
 */
Dialog::Dialog(float x, float y, float w, float h, TextHandler *textHandler, string id)  :
        GraphicElement(x, y, w, h), m_id{id}
{
    setTextureFromImage(DIALOG_IMAGE);
    setPosition(x, y);
    resize(w, h);

    //=== Initialize text

    m_title.setTextFont(textHandler->getBoldFont(), 19, sf::Color::Black);
    m_title.setPosition( x + 24, y + 24);

    m_content.setTextFont(textHandler->getRegularFont(), 15, sf::Color::Black);
    m_content.setPosition( x + 24, y + h/3);

    m_cancelButtonText.setTextFont(textHandler->getBoldFont(), 14, MATERIAL_TEAL_COLOR);
    m_cancelButtonText.setPosition((float) (x + 0.5*m_width), y + h - 30);

    m_okButtonText.setTextFont(textHandler->getBoldFont(), 14, MATERIAL_TEAL_COLOR);
    m_okButtonText.setPosition((float) (x + 0.8*m_width), y + h - 30);
}


/**
 * Constructs a dialog with x and y coordinates,
 * a shopItem, a textHandler and a description
 * @author Arthur
 * @date 01/01/2017 - 07/01/17
 */
Dialog::Dialog(float x, float y, float w, float h, ShopItem *item, TextHandler *textHandler, string description)  :
        GraphicElement(x, y, w, h), m_shopItem{item}, m_id{description}
{
    setTextureFromImage(DIALOG_IMAGE);
    setPosition(x, y);
    resize(w, h);

    //=== Initialize text

    m_title.setTextFont(textHandler->getBoldFont(), 19, sf::Color::Black);
    m_title.setPosition( x + 24, y + 24);

    m_content.setTextFont(textHandler->getRegularFont(), 15, sf::Color::Black);
    m_content.setPosition( x + 24, y + h/3);

    m_cancelButtonText.setTextFont(textHandler->getBoldFont(), 14, MATERIAL_TEAL_COLOR);
    m_cancelButtonText.setPosition((float) (x + 0.5*m_width), y + h - 30);

    m_okButtonText.setTextFont(textHandler->getBoldFont(), 14, MATERIAL_TEAL_COLOR);
    m_okButtonText.setPosition((float) (x + 0.8*m_width), y + h - 30);
}


/**
 * Destructor
 * @author Arthur
 * @date 16/05/17 - 02/01/17
 */
Dialog::~Dialog()
{}


//=== Getters
string Dialog::getId() const { return m_id; }
const Text& Dialog::getCancelButtonText() const { return m_cancelButtonText; }
const Text& Dialog::getOkButtonText() const { return m_okButtonText; }
ShopItem *Dialog::getLinkedShopItem() const {return m_shopItem; }


/**
 * Synchronization Function :
 * sync dialog's labels
 * @author Arthur
 * @date 23/12/16 - 07/01/17
 */
void Dialog::sync(DataBase *dataBase)
{
    if ( m_id == "shopAskDialog")
    {
        m_title.setString(dataBase->getStringFromFile("shop_dialog_title"));
        string content = dataBase->getStringFromFile("shop_dialog_content");
        content.replace(content.find("$NAME"), 5, m_shopItem->getName());
        content.replace(content.find("$PRICE"), 6, to_string(m_shopItem->getPrice()));
        m_content.setString(content);
        m_cancelButtonText.setString(dataBase->getStringFromFile("shop_dialog_cancel"));
        m_okButtonText.setString(dataBase->getStringFromFile("shop_dialog_ok"));
    }
    else if ( m_id == "shopSuccess")
    {
        m_title.setString("");
        m_content.setString(dataBase->getStringFromFile("shop_dialog_success"));
        m_cancelButtonText.setString("");
        m_okButtonText.setString(dataBase->getStringFromFile("shop_dialog_ok"));
    }
    else if ( m_id == "shopFailure")
    {
        m_title.setString("");
        m_content.setString(dataBase->getStringFromFile("shop_dialog_failure"));
        m_cancelButtonText.setString("");
        m_okButtonText.setString(dataBase->getStringFromFile("shop_dialog_ok"));
    }
    else
    {
        m_title.setString("");
        m_content.setString("");
        m_cancelButtonText.setString("");
        m_okButtonText.setString("");
    }
}

/**
 * Draw function
 * @author Arthur
 * @date 16/05/16 - 02/01/17
 */
void Dialog::draw(sf::RenderWindow *window) const
{
    if ( isShowing() )
    {
        window->draw(*this);
        window->draw(m_title);
        window->draw(m_content);
        window->draw(m_cancelButtonText);
        window->draw(m_okButtonText);
    }
}


