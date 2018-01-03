#include "ScrollingBackground.h"

//------------------------------------------------
//          CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------

/**
 * Constructs a scrolling background with
 * a size, a scrolling speed and a background image.
 *
 * @author Arthur, Florian
 * @date 3/03/16 - 29/12/17
 */
ScrollingBackground::ScrollingBackground(float width, float height, float scrollingSpeed, const std::string &image) :
    Sprite(width, height), m_scrollingSpeed{scrollingSpeed}
{
    m_left = new mdsf::Sprite(0, 0, width, height);
    m_left->setTextureFromImage(image);
    m_right = new mdsf::Sprite(width, 0, width, height);
    m_right->setTextureFromImage(image);

    m_left->resize(width, height);
    m_right->resize(width, height);
}


/**
 * Destructor
 * @author Arthur
 * @date 5/03/16 - 15/04/16
 */
ScrollingBackground::~ScrollingBackground()
{
    delete m_left;
    delete m_right;
}


//------------------------------------------------
//          GETTERS
//------------------------------------------------

float ScrollingBackground::getWidth() const { return m_left->getWidth() + m_right->getWidth(); }
float ScrollingBackground::getHeight() const { return m_left->getHeight() + m_right->getHeight(); }
sf::Vector2f ScrollingBackground::getLeftPosition() const { return m_left->getPosition(); }
float ScrollingBackground::getSeparationPositionX() const {
    /*
     * Depending on the current displaying (left-right or right-left),
     * it returns the visible separation position
     */
    if (m_left->getPosition().x >= -(4*SCREEN_WIDTH/3) && m_left->getPosition().x <= SCREEN_WIDTH)
        return m_left->getPosition().x + m_left->getLocalBounds().width;
    else
        return m_right->getPosition().x + m_right->getLocalBounds().width;
}


//------------------------------------------------
//          SETTERS
//------------------------------------------------

void ScrollingBackground::setScrollingSpeed(float speed) { m_scrollingSpeed = speed; }
void ScrollingBackground::setPositions(float x, float y) {
    m_left->setPosition(x, y);
    m_right->setPosition(x + m_left->getWidth(), y);
}


//------------------------------------------------
//          METHODS
//------------------------------------------------

/**
 * Synchronization Function
 * @author Arthur
 * @date 3/03/16 - 29/12/17
 */
void ScrollingBackground::sync()
{
    Sprite::sync();

    m_left->setPosition(m_left->getPosition().x - m_scrollingSpeed, m_left->getPosition().y);
    m_right->setPosition(m_right->getPosition().x - m_scrollingSpeed, m_right->getPosition().y);

    if (m_left->getPosition().x + m_left->getWidth() < 0)
    {
        m_left->setPosition(0, 0);
        m_right->setPosition(m_left->getWidth(), 0);
    }

    applyColor();
}


/**
 * Drawing Function
 * @author Arthur
 * @date 3/03/16 - 03/04/16
 */
void ScrollingBackground::draw(sf::RenderWindow *window) const
{
    window->draw(*m_left);
    window->draw(*m_right);
}


/**
 * Resizing function
 * @author Arthur
 * @date 30/04/16
 */
void ScrollingBackground::resize(float width, float height)
{
    m_left->resize(width, height);
    m_right->resize(width, height);
}


/**
 * Sets the SlidingBackground's texture from an image
 * @author Arthur
 * @date 02/01/17
 */
void ScrollingBackground::setTextureFromImage(const std::string &image)
{
    m_left->setTextureFromImage(image);
    m_right->setTextureFromImage(image);
}

/**
 * Apply light and alpha values to color
 * @author Arthur
 * @date 24/12/17
 */
void ScrollingBackground::applyColor()
{
    m_left->setColor(sf::Color(m_light, m_light, m_light, m_alpha));
    m_right->setColor(sf::Color(m_light, m_light, m_light, m_alpha));
}