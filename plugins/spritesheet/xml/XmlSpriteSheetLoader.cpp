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
#include "engine/image/SpriteSheet.h"
#include "plugins/utility/xmlutils/XmlUtils.h"
#include "engine/utility/FileUtils.h"
#include "engine/utility/StringUtils.h"
#include <tinyxml.h>

namespace B3D
{
    static const float PIXEL_PERFECTNESS_OFFSET = 0.375f;

    bool XmlSpriteSheetLoader::canLoadSpriteSheet(IFile* file)
    {
        return file && StringUtils::endsWith(file->name(), ".xml");
    }

    bool XmlSpriteSheetLoader::loadSpriteSheet(IFile* file, SpriteSheet* sheet)
    {
        if (!file)
            return false;

        try
        {
            TiXmlDocument doc = XmlUtils::parseFile(file);

            const TiXmlElement* root = doc.RootElement();
            XmlUtils::assertTagNameEquals(root, "TextureAtlas");

            std::string textureName = XmlUtils::getStringAttribute(root, "imagePath");
            std::string texturePath = FileUtils::makeFullPath(textureName, file->name());
            int atlasWidth = XmlUtils::getIntAttribute(root, "width");
            int atlasHeight = XmlUtils::getIntAttribute(root, "height");

            for (auto spriteElement : root) {
                XmlUtils::assertTagNameEquals(spriteElement, "sprite");

                SpriteSheet::Element element;
                element.textureName.reset(new std::string(texturePath));

                std::string name = XmlUtils::getStringAttribute(spriteElement, "n");
                bool rotated = XmlUtils::getStringAttribute(spriteElement, "r", std::string()) == "y";

                int textureX = XmlUtils::getIntAttribute(spriteElement, "x");
                int textureY = XmlUtils::getIntAttribute(spriteElement, "y");
                int textureW = XmlUtils::getIntAttribute(spriteElement, "w");
                int textureH = XmlUtils::getIntAttribute(spriteElement, "h");
                float anchorX = XmlUtils::getFloatAttribute(spriteElement, "pX");
                float anchorY = XmlUtils::getFloatAttribute(spriteElement, "pY");

                int xOffset = XmlUtils::getIntAttribute(spriteElement, "oX", 0);
                int yOffset = XmlUtils::getIntAttribute(spriteElement, "oY", 0);
                int width = XmlUtils::getIntAttribute(spriteElement, "oW", rotated ? textureH : textureW);
                int height = XmlUtils::getIntAttribute(spriteElement, "oH", rotated ? textureW : textureH);
                element.originalSize = glm::vec2(width, height);

                float originalX = -float(width) * anchorX;
                float originalY = -float(height) * anchorY;
                element.originalQuad = Quad::fromTopLeftAndSize(originalX, originalY, float(width), float(height));

                float trimmedX = originalX + float(xOffset);
                float trimmedY = originalY + float(yOffset);
                element.trimmedQuad = Quad::fromTopLeftAndSize(trimmedX, trimmedY,
                    float(rotated ? textureH : textureW), float(rotated ? textureW : textureH));

                float tX1 = (float(textureX) + PIXEL_PERFECTNESS_OFFSET) / float(atlasWidth);
                float tY1 = (float(textureY) + PIXEL_PERFECTNESS_OFFSET) / float(atlasHeight);
                float tX2 = (float(textureX + textureW) + PIXEL_PERFECTNESS_OFFSET) / float(atlasWidth);
                float tY2 = (float(textureY + textureH) + PIXEL_PERFECTNESS_OFFSET) / float(atlasHeight);
                if (!rotated)
                    element.textureCoordinates = Quad::fromTopLeftAndBottomRight(tX1, tY1, tX2, tY2);
                else {
                    element.textureCoordinates.topLeft = glm::vec2(tX2, tY1);
                    element.textureCoordinates.topRight = glm::vec2(tX2, tY2);
                    element.textureCoordinates.bottomRight = glm::vec2(tX1, tY2);
                    element.textureCoordinates.bottomLeft = glm::vec2(tX1, tY1);
                }

                sheet->setSprite(name, std::move(element));
            }
        }
        catch (const XmlUtils::ParseError&)
        {
            return false;
        }

        return true;
    }
}
