/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include "XmlSpriteSheetLoader.h"
#include "engine/core/Log.h"
#include "engine/sprites/SpriteSheet.h"
#include "engine/utility/FileUtils.h"
#include "engine/utility/XmlUtils.h"
#include <tinyxml.h>

namespace Engine
{
    bool XmlSpriteSheetLoader::canLoadSpriteSheet(IFile* file)
    {
        // FIXME
        return true;
    }

    bool XmlSpriteSheetLoader::loadSpriteSheet(IFile* file, SpriteSheet* sheet)
    {
        if (!file)
            return false;

        try
        {
            TiXmlDocument doc = XmlUtils::parseFile(file);

            TiXmlElement* root = doc.RootElement();
            XmlUtils::assertTagNameEquals(root, "TextureAtlas");

            int atlasWidth = XmlUtils::getIntAttribute(root, "width");
            int atlasHeight = XmlUtils::getIntAttribute(root, "height");

            std::string textureName = XmlUtils::getStringAttribute(root, "imagePath");
            std::string texturePath = FileUtils::makeFullPath(textureName, file->name());
            TexturePtr texture = Services::resourceManager()->getTexture(texturePath);

            for (auto spriteElement : root) {
                XmlUtils::assertTagNameEquals(spriteElement, "sprite");

                auto element = std::make_shared<SpriteSheet::Element>();
                element->texture = texture;

                std::string name = XmlUtils::getStringAttribute(spriteElement, "n");

                int textureX = XmlUtils::getIntAttribute(spriteElement, "x");
                int textureY = XmlUtils::getIntAttribute(spriteElement, "y");
                int textureW = XmlUtils::getIntAttribute(spriteElement, "w");
                int textureH = XmlUtils::getIntAttribute(spriteElement, "h");
                float anchorX = XmlUtils::getFloatAttribute(spriteElement, "pX");
                float anchorY = XmlUtils::getFloatAttribute(spriteElement, "pY");

                int xOffset = XmlUtils::getIntAttribute(spriteElement, "oX", 0);
                int yOffset = XmlUtils::getIntAttribute(spriteElement, "oY", 0);
                int width = XmlUtils::getIntAttribute(spriteElement, "oW", textureW);
                int height = XmlUtils::getIntAttribute(spriteElement, "oH", textureH);

                bool rotated = XmlUtils::getStringAttribute(spriteElement, "r", std::string()) == "r";

                float originalX = -float(width) * anchorX;
                float originalY = -float(height) * anchorY;
                element->originalQuad = Quad::fromTopLeftAndSize(originalX, originalY, float(width), float(height));

                float trimmedX = originalX + float(xOffset);
                float trimmedY = originalY + float(yOffset);
                element->trimmedQuad = Quad::fromTopLeftAndSize(trimmedX, trimmedY, float(textureW), float(textureH));

                float tX1 = float(textureX) / float(atlasWidth - 1);
                float tY1 = float(textureY) / float(atlasHeight - 1);
                float tX2 = float(textureX + textureW - 1) / float(atlasWidth - 1);
                float tY2 = float(textureY + textureH - 1) / float(atlasHeight - 1);
                element->textureCoordinates = Quad::fromTopLeftAndBottomRight(tX1, tY1, tX2, tY2);

                sheet->addSprite(name, std::move(element));
            }
        }
        catch (const XmlUtils::ParseError&)
        {
            return false;
        }

        return true;
    }
}
