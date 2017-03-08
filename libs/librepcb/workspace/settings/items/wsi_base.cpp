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
#include <QtWidgets>
#include "wsi_base.h"
#include <librepcb/common/fileio/xmldomelement.h>
#include "../workspacesettings.h"
#include "../../workspace.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace workspace {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

WSI_Base::WSI_Base(const QString& xmlTagName, XmlDomElement* xmlElement) throw (Exception) :
    QObject(0), mXmlElementTagName(xmlTagName)
{
    Q_ASSERT((!xmlElement) || (xmlElement->getName() == xmlTagName));
}

WSI_Base::~WSI_Base() noexcept
{
}

/*****************************************************************************************
 *  Public Methods
 ****************************************************************************************/

XmlDomElement* WSI_Base::serializeToXmlDomElement() const throw (Exception)
{
    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);
    return new XmlDomElement(mXmlElementTagName);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace workspace
} // namespace librepcb
