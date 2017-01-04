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

#ifndef SHOP_H
#define SHOP_H

#include <vector>
#include "ShopItem.h"
#include "DataBase.h"

/**
 * Shop Class
 * @author Arthur
 * @date 11/05/16 - 04/01/17
 */
class Shop
{
public:
    //=== CTORs / DTORs
    Shop(DataBase *dataBase);
    ~Shop();

    //=== GETTERS
    DataBase* getDataBase() const;
    std::vector<ShopItem*> getShopItemsArray() const;

    //=== METHODS
    bool buyItem(ShopItem *item);

private:
    //=== ATTRIBUTES
    DataBase *m_dataBase;
    std::vector<ShopItem*> m_shopItemsArray;

    void fetchBuyableItemsFromFile();
    std::string getStringFromLangFile(std::string name);
};

#endif
