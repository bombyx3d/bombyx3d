/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Core.h"
#include "core/interfaces/IGame.h"
#include "core/utility/debug.h"
#include <memory>

namespace Engine
{
    Core* Core::m_Instance;
    ISystem* Core::m_System;

    Core::Core()
    {
        Z_ASSERT(m_Instance == nullptr);
        m_Instance = this;
        Initializer::init(*this);
    }

    Core::~Core()
    {
        Z_ASSERT(m_Instance == this);
        m_System = nullptr;
        m_Instance = nullptr;
    }

    void Core::addSingleton(IUnknown* singleton)
    {
        m_SingletonList.push_back(singleton);

        IUnknown::InterfaceList interfaces;
        singleton->queryAllInterfaces(interfaces);
        for (const auto& it : interfaces)
            m_SingletonMap[it.first].push_back(it.second);
    }

    void* Core::querySingleton(TypeID typeID)
    {
        auto it = m_SingletonMap.find(typeID);
        if (it != m_SingletonMap.end() && !it->second.empty())
            return it->second.back();

        return nullptr;
    }

    const std::vector<void*>& Core::querySingletons(TypeID typeID)
    {
        auto it = m_SingletonMap.find(typeID);
        if (it != m_SingletonMap.end())
            return it->second;

        static const std::vector<void*> empty;
        return empty;
    }

    int Core::run(ISystemEventLoop* eventLoop)
    {
        Core core;

        std::vector<ISystem*> systems = core.querySingletons<ISystem>();
        Z_ASSERT(systems.size() == 1);
        m_System = systems.front();

        IGame* gameInterface = nullptr;
        for (const auto& p : core.allSingletons()) {
            IGame* game = p->queryInterface<IGame>();
            if (game) {
                Z_ASSERT(gameInterface == nullptr);
                gameInterface = game;
            }

            IInitializable* initializable = p->queryInterface<IInitializable>();
            if (initializable && !game)
                initializable->initialize();
        }

        Z_ASSERT(gameInterface != nullptr);
        gameInterface->initialize();

        return eventLoop->runEventLoop();
    }
}
