/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "libraryelement.h"
#include <librepcb/common/fileio/xmldomdocument.h>
#include <librepcb/common/fileio/xmldomelement.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

LibraryElement::LibraryElement(const QString& shortElementName,
                               const QString& longElementName, const Uuid& uuid,
                               const Version& version, const QString& author,
                               const QString& name_en_US, const QString& description_en_US,
                               const QString& keywords_en_US) throw (Exception) :
    LibraryBaseElement(true, shortElementName, longElementName, uuid, version, author,
                       name_en_US, description_en_US, keywords_en_US)
{
}

LibraryElement::LibraryElement(const FilePath& elementDirectory, const QString& shortElementName,
                               const QString& longElementName, bool readOnly) throw (Exception) :
    LibraryBaseElement(elementDirectory, true, shortElementName, longElementName, readOnly)
{
    // read category UUIDs
    XmlDomElement& root = mLoadingXmlFileDocument->getRoot();
    for (XmlDomElement* node = root.getFirstChild("meta/category", true, false);
         node; node = node->getNextSibling("category"))
    {
        mCategories.append(node->getText<Uuid>(true));
    }
}

LibraryElement::~LibraryElement() noexcept
{
}

/*****************************************************************************************
 *  Protected Methods
 ****************************************************************************************/

XmlDomElement* LibraryElement::serializeToXmlDomElement() const throw (Exception)
{
    QScopedPointer<XmlDomElement> root(LibraryBaseElement::serializeToXmlDomElement());
    foreach (const Uuid& uuid, mCategories) {
        root->getFirstChild("meta", true)->appendTextChild("category", uuid);
    }
    return root.take();
}

bool LibraryElement::checkAttributesValidity() const noexcept
{
    if (!LibraryBaseElement::checkAttributesValidity()) return false;
    foreach (const Uuid& uuid, mCategories) {
        if (uuid.isNull()) return false;
    }
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
