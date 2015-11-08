/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
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
#include "undocommand.h"
#include "exceptions.h"

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

UndoCommand::UndoCommand(const QString &text, UndoCommand* parent) throw (Exception) :
    mRedoCount(0), mUndoCount(0), mParent(parent), mText(text), mChilds()
{
    if (mParent)
        mParent->appendChild(this);
}

UndoCommand::~UndoCommand() noexcept
{
    for (int i = mChilds.count()-1; i >= 0; i--) // reverse
        delete mChilds[i]; // this will call removeChild() to remove the entry in the list

    if (mParent)
        mParent->removeChild(this);
}

/*****************************************************************************************
 *  Getters
 ****************************************************************************************/

bool UndoCommand::isExecuted() const noexcept
{
    Q_ASSERT(mRedoCount >= mUndoCount);
    Q_ASSERT(mRedoCount <= mUndoCount+1);
    return mRedoCount > mUndoCount;
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void UndoCommand::undo() throw (Exception)
{
    Q_ASSERT(isExecuted());

    int i;

    try
    {
        for (i = mChilds.count()-1; i >= 0; i--) // from top to bottom
            mChilds[i]->undo();

        mUndoCount++;
    }
    catch (Exception& e)
    {
        // could not undo the child with index "i"!
        // try to revert the whole action --> redo the undoed commands
        for (; i < mChilds.count(); i++) // from "i" to top
            mChilds[i]->redo();

        throw;
    }
}

void UndoCommand::redo() throw (Exception)
{
    Q_ASSERT(!isExecuted());

    int i;

    try
    {
        for (i = 0; i < mChilds.count(); i++) // from bottom to top
            mChilds[i]->redo();

        mRedoCount++;
    }
    catch (Exception& e)
    {
        // could not undo the child with index "i"!
        // try to revert the whole action --> undo the redoed commands
        for (; i >= 0; i--) // from "i" to bottom
            mChilds[i]->undo();

        throw;
    }
}

bool UndoCommand::mergeWith(const UndoCommand* other) noexcept
{
    Q_UNUSED(other);
    return false;
}

/*****************************************************************************************
 *  Internal Methods
 ****************************************************************************************/

void UndoCommand::appendChild(UndoCommand* child) noexcept
{
    Q_ASSERT(!mChilds.contains(child)); // check if the child is not in the list already
    mChilds.append(child);
}

void UndoCommand::removeChild(UndoCommand* child) noexcept
{
    Q_ASSERT(child == mChilds.last()); // check if the order of append/remove is correct
    mChilds.removeOne(child);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/
