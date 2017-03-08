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
#include "boardgerberexport.h"
#include <librepcb/common/cam/gerbergenerator.h>
#include <librepcb/common/cam/excellongenerator.h>
#include <librepcb/common/boardlayer.h>
#include <librepcb/common/boarddesignrules.h>
#include <librepcb/common/geometry/hole.h>
#include <librepcb/library/pkg/footprint.h>
#include <librepcb/library/pkg/footprintpadsmt.h>
#include <librepcb/library/pkg/footprintpadtht.h>
#include "../project.h"
#include "board.h"
#include "items/bi_device.h"
#include "items/bi_footprint.h"
#include "items/bi_footprintpad.h"
#include "items/bi_via.h"
#include "items/bi_netpoint.h"
#include "items/bi_netline.h"
#include "items/bi_polygon.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

BoardGerberExport::BoardGerberExport(const Board& board, const FilePath& outputDir) noexcept :
    mProject(board.getProject()), mBoard(board), mOutputDirectory(outputDir)
{
}

BoardGerberExport::~BoardGerberExport() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void BoardGerberExport::exportAllLayers() const throw (Exception)
{
    exportDrillsPTH();
    exportLayerBoardOutlines();
    exportLayerTopCopper();
    exportLayerTopSolderMask();
    exportLayerTopOverlay();
    exportLayerBottomCopper();
    exportLayerBottomSolderMask();
    exportLayerBottomOverlay();
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void BoardGerberExport::exportDrillsPTH() const throw (Exception)
{
    ExcellonGenerator gen;

    // footprint holes and pads
    foreach (const BI_Device* device, mBoard.getDeviceInstances()) {
        const BI_Footprint& footprint = device->getFootprint();
        for (int i = 0; i < footprint.getLibFootprint().getHoleCount(); ++i) {
            const Hole* hole = footprint.getLibFootprint().getHole(i); Q_ASSERT(hole);
            gen.drill(footprint.mapToScene(hole->getPosition()), hole->getDiameter());
        }
        foreach (const BI_FootprintPad* pad, footprint.getPads()) {
            const library::FootprintPad& libPad = pad->getLibPad();
            if (libPad.getTechnology() == library::FootprintPad::Technology_t::THT) {
                const library::FootprintPadTht* tht = dynamic_cast<const library::FootprintPadTht*>(&libPad); Q_ASSERT(tht);
                gen.drill(pad->getPosition(), tht->getDrillDiameter());
            }
        }
    }

    // vias
    foreach (const BI_Via* via, mBoard.getVias()) {
        gen.drill(via->getPosition(), via->getDrillDiameter());
    }

    gen.generate();
    gen.saveToFile(getOutputFilePath("DRILLS-PTH.drl"));
}

void BoardGerberExport::exportLayerBoardOutlines() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::BoardOutlines);
    gen.generate();
    gen.saveToFile(getOutputFilePath("OUTLINES.gbr"));
}

void BoardGerberExport::exportLayerTopCopper() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::TopCopper);
    gen.generate();
    gen.saveToFile(getOutputFilePath("COPPER-TOP.gbr"));
}

void BoardGerberExport::exportLayerTopSolderMask() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::TopStopMask);
    gen.generate();
    gen.saveToFile(getOutputFilePath("SOLDERMASK-TOP.gbr"));
}

void BoardGerberExport::exportLayerTopOverlay() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::TopOverlay);
    gen.setLayerPolarity(GerberGenerator::LayerPolarity::Negative);
    drawLayer(gen, BoardLayer::TopStopMask);
    gen.generate();
    gen.saveToFile(getOutputFilePath("SILKSCREEN-TOP.gbr"));
}

void BoardGerberExport::exportLayerBottomCopper() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::BottomCopper);
    gen.generate();
    gen.saveToFile(getOutputFilePath("COPPER-BOTTOM.gbr"));
}

void BoardGerberExport::exportLayerBottomSolderMask() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::BottomStopMask);
    gen.generate();
    gen.saveToFile(getOutputFilePath("SOLDERMASK-BOTTOM.gbr"));
}

void BoardGerberExport::exportLayerBottomOverlay() const throw (Exception)
{
    GerberGenerator gen(mProject.getName() % " - " % mBoard.getName(),
                        mBoard.getUuid(), mProject.getVersion());
    drawLayer(gen, BoardLayer::BottomOverlay);
    gen.setLayerPolarity(GerberGenerator::LayerPolarity::Negative);
    drawLayer(gen, BoardLayer::BottomStopMask);
    gen.generate();
    gen.saveToFile(getOutputFilePath("SILKSCREEN-BOTTOM.gbr"));
}

void BoardGerberExport::drawLayer(GerberGenerator& gen, int layerId) const throw (Exception)
{
    // draw footprints incl. pads
    foreach (const BI_Device* device, mBoard.getDeviceInstances()) {
        Q_ASSERT(device);
        drawFootprint(gen, device->getFootprint(), layerId);
    }

    // draw vias
    foreach (const BI_Via* via, mBoard.getVias()) {
        Q_ASSERT(via);
        drawVia(gen, *via, layerId);
    }

    // draw traces
    foreach (const BI_NetLine* netline, mBoard.getNetLines()) {
        Q_ASSERT(netline);
        if (netline->getLayer().getId() == layerId) {
            gen.drawLine(netline->getStartPoint().getPosition(),
                         netline->getEndPoint().getPosition(),
                         netline->getWidth());
        }
    }

    // draw polygons
    foreach (const BI_Polygon* polygon, mBoard.getPolygons()) {
        Q_ASSERT(polygon);
        if (layerId == polygon->getPolygon().getLayerId()) {
            Polygon p(polygon->getPolygon());
            p.setLineWidth(calcWidthOfLayer(polygon->getPolygon().getLineWidth(), layerId));
            gen.drawPolygonOutline(p);
        }
    }
}

void BoardGerberExport::drawVia(GerberGenerator& gen, const BI_Via& via, int layerId) const throw (Exception)
{
    bool drawCopper = via.isOnLayer(layerId);
    bool drawStopMask = (layerId == BoardLayer::TopStopMask || layerId == BoardLayer::BottomStopMask)
                        && mBoard.getDesignRules().doesViaRequireStopMask(via.getDrillDiameter());
    if (drawCopper || drawStopMask) {
        Length outerDiameter = via.getSize();
        if (drawStopMask) {
            outerDiameter += mBoard.getDesignRules().calcStopMaskClearance(via.getSize()) * 2;
        }
        switch (via.getShape())
        {
            case BI_Via::Shape::Round: {
                gen.flashCircle(via.getPosition(), outerDiameter, Length(0));
                break;
            }
            case BI_Via::Shape::Square: {
                gen.flashRect(via.getPosition(), outerDiameter, outerDiameter,
                              Angle::deg0(), Length(0));
                break;
            }
            case BI_Via::Shape::Octagon: {
                gen.flashRegularPolygon(via.getPosition(), outerDiameter, 8,
                                        Angle::deg0(), Length(0));
                break;
            }
            default: {
                throw LogicError(__FILE__, __LINE__);
            }
        }
    }
}

void BoardGerberExport::drawFootprint(GerberGenerator& gen, const BI_Footprint& footprint, int layerId) const throw (Exception)
{
    // draw pads
    foreach (const BI_FootprintPad* pad, footprint.getPads()) {
        drawFootprintPad(gen, *pad, layerId);
    }

    // draw polygons
    for (int i = 0; i < footprint.getLibFootprint().getPolygonCount(); ++i) {
        const Polygon* polygon = footprint.getLibFootprint().getPolygon(i); Q_ASSERT(polygon);
        int layer = footprint.getIsMirrored() ? BoardLayer::getMirroredLayerId(layerId) : layerId;
        if (layer == polygon->getLayerId()) {
            Angle rot = footprint.getIsMirrored() ? -footprint.getRotation() : footprint.getRotation();
            Polygon p = polygon->rotated(rot).translate(footprint.getPosition());
            p.setLineWidth(calcWidthOfLayer(p.getLineWidth(), layer));
            gen.drawPolygonOutline(p);
            if (p.isFilled()) {
                gen.drawPolygonArea(p);
            }
        }
    }

    // draw ellipses
    for (int i = 0; i < footprint.getLibFootprint().getEllipseCount(); ++i) {
        const Ellipse* ellipse = footprint.getLibFootprint().getEllipse(i); Q_ASSERT(ellipse);
        int layer = footprint.getIsMirrored() ? BoardLayer::getMirroredLayerId(layerId) : layerId;
        if (layer == ellipse->getLayerId()) {
            Angle rot = footprint.getIsMirrored() ? -footprint.getRotation() : footprint.getRotation();
            Ellipse e = ellipse->rotated(rot).translate(footprint.getPosition());
            e.setLineWidth(calcWidthOfLayer(e.getLineWidth(), layer));
            gen.drawEllipseOutline(e);
            if (e.isFilled()) {
                gen.drawEllipseArea(e);
            }
        }
    }

    // TODO: draw texts

    // draw holes
    for (int i = 0; i < footprint.getLibFootprint().getHoleCount(); ++i) {
        const Hole* hole = footprint.getLibFootprint().getHole(i); Q_ASSERT(hole);
        gen.flashCircle(footprint.mapToScene(hole->getPosition()), hole->getDiameter(), Length(0));
    }
}

void BoardGerberExport::drawFootprintPad(GerberGenerator& gen, const BI_FootprintPad& pad, int layerId) const throw (Exception)
{
    bool isOnCopperLayer = pad.isOnLayer(layerId);
    bool isOnSolderMaskTop = pad.isOnLayer(BoardLayer::LayerID::TopCopper) && (layerId == BoardLayer::LayerID::TopStopMask);
    bool isOnSolderMaskBottom = pad.isOnLayer(BoardLayer::LayerID::BottomCopper) && (layerId == BoardLayer::LayerID::BottomStopMask);
    if (!isOnCopperLayer && !isOnSolderMaskTop && !isOnSolderMaskBottom) {
        return;
    }

    Angle rot = pad.getIsMirrored() ? -pad.getRotation() : pad.getRotation();
    const library::FootprintPad& libPad = pad.getLibPad();
    Length width = libPad.getWidth();
    Length height = libPad.getHeight();
    if (isOnSolderMaskTop || isOnSolderMaskBottom) {
        Length size = qMin(width, height);
        Length clearance = mBoard.getDesignRules().calcStopMaskClearance(size);
        width += clearance*2;
        height += clearance*2;
    }

    switch (libPad.getTechnology())
    {
        case library::FootprintPad::Technology_t::SMT: {
            //const library::FootprintPadSmt* smt = dynamic_cast<const library::FootprintPadSmt*>(&libPad); Q_ASSERT(smt);
            gen.flashRect(pad.getPosition(), width, height, rot, Length(0));
            break;
        }
        case library::FootprintPad::Technology_t::THT: {
            const library::FootprintPadTht* tht = dynamic_cast<const library::FootprintPadTht*>(&libPad); Q_ASSERT(tht);
            switch (tht->getShape())
            {
                case library::FootprintPadTht::Shape_t::ROUND: {
                    if (width == height) {
                        gen.flashCircle(pad.getPosition(), width, Length(0));
                    } else {
                        gen.flashObround(pad.getPosition(), width, height, rot, Length(0));
                    }
                    break;
                }
                case library::FootprintPadTht::Shape_t::RECT: {
                    gen.flashRect(pad.getPosition(), width, height, rot, Length(0));
                    break;
                }
                case library::FootprintPadTht::Shape_t::OCTAGON: {
                    if (width != height) {
                        throw LogicError(__FILE__, __LINE__, QString(),
                            tr("Sorry, non-square octagons are not yet supported."));
                    }
                    gen.flashRegularPolygon(pad.getPosition(), width, 8, rot, Length(0));
                    break;
                }
                default: {
                    throw LogicError(__FILE__, __LINE__);
                }
            }
            break;
        }
        default: {
            throw LogicError(__FILE__, __LINE__);
        }
    }
}

FilePath BoardGerberExport::getOutputFilePath(const QString& suffix) const noexcept
{
    QString projectName = FilePath::cleanFileName(mProject.getName(),
                          FilePath::ReplaceSpaces | FilePath::KeepCase);
    return mOutputDirectory.getPathTo(projectName % "_" % suffix);
}

/*****************************************************************************************
 *  Static Methods
 ****************************************************************************************/

Length BoardGerberExport::calcWidthOfLayer(const Length& width, int layerId) noexcept
{
    if ((layerId == BoardLayer::LayerID::BoardOutlines) && (width < Length(1000))) {
        return Length(1000); // outlines should have a minimum width of 1um
    } else {
        return width;
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
} // namespace librepcb
