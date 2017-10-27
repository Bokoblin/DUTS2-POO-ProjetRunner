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

#ifndef SLIDING_BACKGROUND_H
#define SLIDING_BACKGROUND_H

#include "GraphicElement.h"
#include "../utils/constants.h"

/**
 * SlidingBackground Class
 * @author Arthur, Florian
 * @date 3/03/16 - 16/04/17
 */
class ScrollingBackground : public GraphicElement
{
public:
    //=== CTORs / DTORs
    ScrollingBackground(float width, float height, float scrollingSpeed, std::string image);
    ScrollingBackground(ScrollingBackground const& element) = delete;
    virtual ~ScrollingBackground();

    sf::Vector2f getLeftPosition() const;
    float getSeparationPositionX() const;

    //=== SETTERS
    void setScrollingSpeed(float speed);
    void setPositions(float x, float y);

    //=== METHODS
    virtual void sync() override;
    virtual void draw(sf::RenderWindow *window) const override;
    virtual void resize(float width, float height) override;
    virtual void setTextureFromImage(std::string image);

private:
    //=== ATTRIBUTES
    GraphicElement *m_left, *m_right;
    float m_scrollingSpeed;

};

#endif