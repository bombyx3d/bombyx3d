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

#pragma once
#include <QObject>
#include <QDomElement>
#include <QDir>
#include <QHash>
#include <QIcon>
#include <vector>
#include <memory>


#define Z_BUILDER_XMLID(ID) \
    public: \
    static QString staticXmlId() { return ID; } \
    virtual QString xmlId() const { return ID; }

#define Z_BUILDER_NAME(NAME) \
    public: \
    static QString staticName() { return NAME; } \
    virtual QString name() const override { return NAME; }

#define Z_BUILDER_ICON(ICON) \
    public: \
    static QIcon staticIcon() { return QIcon(ICON); } \
    QIcon icon() const override { return QIcon(ICON); }


class Builder;
using BuilderPtr = std::shared_ptr<Builder>;

class Builder : public QObject
{
    Q_OBJECT

public:
    class Factory
    {
    public:
        const QString builderName;
        const QString builderXmlId;
        const QIcon builderIcon;

        Factory(const QString& name, const QString& xmlId, const QIcon& icon);
        virtual ~Factory() = default;

        virtual BuilderPtr createBuilder() const = 0;
    };

    using FactoryPtr = std::shared_ptr<Builder::Factory>;
    using FactoryList = std::vector<FactoryPtr>;
    using FactoryHash = QHash<QString, FactoryPtr>;

    static const FactoryList& factories();
    static FactoryPtr factoryWithXmlId(const QString& xmlId);


    Builder() = default;
    ~Builder() = default;

    virtual QString xmlId() const = 0;
    virtual QString name() const = 0;

    static QIcon staticIcon();
    virtual QIcon icon() const;

    virtual QWidget* createEditor(QWidget* parent = nullptr);

    virtual bool load(const QDomElement& element, const QDir& projectDir, QString* errorMessage);
    virtual bool save(QDomElement& element, const QDir& projectDir, QString* errorMessage);

signals:
    void setModified();
};
