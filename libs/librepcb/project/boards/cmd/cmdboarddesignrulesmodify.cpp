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
#include "cmdboarddesignrulesmodify.h"
#include "../board.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdBoardDesignRulesModify::CmdBoardDesignRulesModify(Board& board, const BoardDesignRules& newRules) noexcept :
    UndoCommand(tr("Modify board design rules")), mBoard(board),
    mOldRules(), mNewRules(newRules)
{
}

CmdBoardDesignRulesModify::~CmdBoardDesignRulesModify() noexcept
{
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdBoardDesignRulesModify::performExecute() throw (Exception)
{
    mOldRules = mBoard.getDesignRules(); // memorize current design rules

    performRedo(); // can throw

    return true; // TODO: determine if the design rules were really modified
}

void CmdBoardDesignRulesModify::performUndo() throw (Exception)
{
    mBoard.getDesignRules() = mOldRules;
    emit mBoard.attributesChanged();
}

void CmdBoardDesignRulesModify::performRedo() throw (Exception)
{
    mBoard.getDesignRules() = mNewRules;
    emit mBoard.attributesChanged();
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
} // namespace librepcb

