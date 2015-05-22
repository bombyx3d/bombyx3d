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
#include "BuilderFactory.h"
#include "BinaryFileBuilder.h"

namespace
{
    template <class TYPE> class StandardBuilderFactory : public BuilderFactory
    {
    public:
        StandardBuilderFactory(const QString& name, const QIcon& icon) : m_Icon(icon), m_Name(name) {}

        const QIcon& builderIcon() const override { return m_Icon; }
        const QString& builderName() const override { return m_Name; }

        Builder* createBuilder() const override { return new TYPE; }

    private:
        QIcon m_Icon;
        QString m_Name;
    };
}

const std::vector<std::unique_ptr<BuilderFactory>>& BuilderFactory::allFactories()
{
    static std::unique_ptr<std::vector<std::unique_ptr<BuilderFactory>>> g_AllFactories;
    if (!g_AllFactories) {
        g_AllFactories.reset(new std::vector<std::unique_ptr<BuilderFactory>>);

        #define DEF(CLASS, NAME, ICON) \
            g_AllFactories->emplace_back(new StandardBuilderFactory<CLASS>(QObject::NAME, QIcon(":/icons/" ICON)))

        DEF(BinaryFileBuilder, tr("Binary file"), "fatcow_modified/compile_dark.png");

        #undef DEF
    }
    return *g_AllFactories;
}

const QIcon& BuilderFactory::builderIcon() const
{
    static std::unique_ptr<QIcon> icon;
    if (!icon)
        icon.reset(new QIcon(":/icons/emblem-system.png"));
    return *icon;
}
