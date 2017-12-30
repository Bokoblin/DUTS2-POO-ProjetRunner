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

#ifndef ANIMATED_GRAPHIC_ELEMENT_H
#define ANIMATED_GRAPHIC_ELEMENT_H

#include "GraphicElement.h"
#include <chrono>

/**
 * The AnimatedGraphicElement class adds to a Graphic element
 * multiple clips from texture for them to change on events,
 * a timePoint of the last time the animation has changed
 * and an animation delay
 * @author Arthur
 * @date 3/03/16 - 10/04/17
 */
class AnimatedGraphicElement : public GraphicElement
{
public:
    //=== CTORs / DTORs
    AnimatedGraphicElement(float x, float y, float w, float h, const std::string &image,
                           const std::vector<sf::IntRect> &clipRect);
    AnimatedGraphicElement( const AnimatedGraphicElement& other);
    ~AnimatedGraphicElement() override;

    //=== SETTERS
    void setClipRectArray(std::vector<sf::IntRect> clipRectsArray);

    //=== METHODS
    void sync() override;

private:
    //=== ATTRIBUTES
    std::vector<sf::IntRect> m_clipRectArray;
    unsigned int m_currentClipRect;
    std::chrono::system_clock::time_point m_lastAnimationTime;
    const int ANIM_DELAY = 200;
};

#endif