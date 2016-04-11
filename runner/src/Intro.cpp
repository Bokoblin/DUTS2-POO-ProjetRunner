#include "../header/Intro.h"

using namespace std;

/********************************************
    Parameterized Constructor
*********************************************
    @author Arthur  @date 27/03
*********************************************/
Intro::Intro(float w, float h, sf::RenderWindow *window): View(w, h, window)
{
    m_window->create( sf::VideoMode(w, h, 32), "Boko Runner", sf::Style::None );
    m_window->setFramerateLimit(30);
    m_window->setPosition(sf::Vector2i( (sf::VideoMode::getDesktopMode().width - m_width)/2, (sf::VideoMode::getDesktopMode().height - m_height)/2 ));

    loadImages();
}


/********************************************
    Destructor
*********************************************
    @author Arthur  @date 27/03
*********************************************/
Intro::~Intro()
{
    if(m_introGraphic!= NULL)
        delete m_introGraphic;
}


/********************************************
    Image Loading
*********************************************
    @author Arthur  @date 27/03
*********************************************/
void Intro::loadImages()
{
    if (!m_introTexture.loadFromFile(INTRO_IMAGE))
        cerr << "ERROR when loading image file: " << INTRO_IMAGE << endl;
    else
    {
        m_introTexture.setSmooth(true);
        m_introGraphic = new GraphicElement(m_introTexture, 0,0, 400, 200);
    }
}


/********************************************
    Synchronization function
*********************************************
    @author Arthur  @date 27/03
*********************************************/
void Intro::synchronize()
{ }


/********************************************
    View Drawing
*********************************************
    @author Arthur  @date 27/03
*********************************************/
void Intro::draw() const
{
    m_window->clear();

    m_window->draw(*m_introGraphic);

    m_window->display();
}


/********************************************
    Events treating
*********************************************
    @author Arthur  @date 27/03
*********************************************/
bool Intro::treatEvents()
{
    bool result = false;

    if(m_window->isOpen())
    {
        result = true;

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_model->getIntroState() == true)
            {
                m_model->setIntroState(false);
                m_model->setMenuState(true);
            }
        }
    }
    return result;
}
