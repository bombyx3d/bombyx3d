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
#include "Builder.h"
#include "builders/BinaryFileBuilder.h"
#include <QWidget>
#include <QBoxLayout>

namespace
{
    template <class TYPE> class BuilderFactory : public Builder::Factory
    {
    public:
        BuilderFactory() : Builder::Factory(TYPE::staticName(), TYPE::staticXmlId(), TYPE::staticIcon()) {}
        BuilderPtr createBuilder() const override { return std::make_shared<TYPE>(); }
    };
}

Builder::Factory::Factory(const QString& name, const QString& xmlId, const QIcon& icon)
    : builderName(name)
    , builderXmlId(xmlId)
    , builderIcon(icon)
{
}

static std::unique_ptr<Builder::FactoryList> g_FactoryList;
static std::unique_ptr<Builder::FactoryHash> g_FactoryHash;

static void initFactoryList()
{
    if (!g_FactoryList) {
        g_FactoryList.reset(new Builder::FactoryList);
        g_FactoryList->emplace_back(std::make_shared<BuilderFactory<BinaryFileBuilder>>());
    }

    if (!g_FactoryHash) {
        g_FactoryHash.reset(new Builder::FactoryHash);
        for (const auto& factory : *g_FactoryList)
            g_FactoryHash->insert(factory->builderXmlId, factory);
    }
}

const Builder::FactoryList& Builder::factories()
{
    initFactoryList();
    return *g_FactoryList;
}

Builder::FactoryPtr Builder::factoryWithXmlId(const QString& xmlId)
{
    initFactoryList();
    return g_FactoryHash->value(xmlId);
}

QIcon Builder::staticIcon()
{
    return QIcon(":/icons/emblem-system.png");
}

QIcon Builder::icon() const
{
    return Builder::staticIcon();
}

QWidget* Builder::createEditor(QWidget* parent)
{
    QWidget* editor = new QWidget(parent);
    editor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    connect(this, SIGNAL(destroyed()), editor, SLOT(deleteLater()));

    new QVBoxLayout(editor);

    return editor;
}

bool Builder::load(const QDomElement& element, const QDir& projectDir, QString* errorMessage)
{
    Q_UNUSED(element);
    Q_UNUSED(projectDir);
    Q_UNUSED(errorMessage);
    return true;
}

bool Builder::save(QDomElement& element, const QDir& projectDir, QString* errorMessage)
{
    Q_UNUSED(element);
    Q_UNUSED(projectDir);
    Q_UNUSED(errorMessage);
    return true;
}
