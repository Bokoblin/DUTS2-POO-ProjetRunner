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

#ifndef GRAPHIC_ELEMENT_H
#define GRAPHIC_ELEMENT_H

#include <iostream>
#include <SFML/Graphics.hpp>

/**
 * GraphicElement Class
 * @author Arthur, Florian
 * @date 21/02/16 - 24/01/17
 */
class GraphicElement : public sf::Sprite
{
public:
    //=== CTORs / DTORs
    GraphicElement( float w, float h);
    GraphicElement( float x, float y, float w, float h);
    GraphicElement(float x, float y, float w, float h, std::string image);
    GraphicElement(GraphicElement const& element);
    ~GraphicElement();

    //=== GETTERS
    bool isShowing() const;

    //=== SETTERS
    void show();
    void hide();

    //=== METHODS
    virtual void sync();
    virtual void draw(sf::RenderWindow *window)const;
    virtual void resize(float w, float h);
    virtual void setTextureFromImage(std::string image);
    virtual bool contains(float x, float y) const;
    void setTextureFromImage(std::string image, sf::IntRect intRect);

protected:
    //=== ATTRIBUTES
    float m_width;
    float m_height;
    bool m_isShowing;
    sf::Texture m_texture;
};

#endif