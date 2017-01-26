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

#ifndef MODEL_H
#define MODEL_H

#include "DataBase.h"

enum AppState
{
    INTRO,
    MENU,
    GAME,
    RESET_GAME,
    QUIT
};

/**
 * The Model class provides common
 * attributes and methods to other model classes
 * and contains App's current state
 * @author Arthur, Florian
 * @date 21/02/16 - 20/12/16
 */
class Model
{
public:
    //=== CTORs / DTORs
    Model(int w, int h);
    Model( const Model & model);
    virtual ~Model();

    //=== GETTERS
    DataBase *getDataBase() const;
    AppState getAppState() const;

    //=== SETTERS
    void setDataBase(DataBase *data);
    void setAppState(AppState state);

    //=== METHODS
    virtual void nextStep();

protected:
    //=== ATTRIBUTES
    int m_width, m_height;
    DataBase *m_dataBase;

private:
    AppState m_appState;
};

#endif
