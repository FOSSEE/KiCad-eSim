/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2007, 2008, 2012 Alexander Lunev <al.lunev@yahoo.com>
 * Copyright (C) 2012 KiCad Developers, see CHANGELOG.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file pcb_plane.h
 */

#ifndef PCB_PLANE_H_
#define PCB_PLANE_H_

#include <wx/wx.h>

#include <pcb_polygon.h>

namespace PCAD2KICAD {

class PCB_PLANE : public PCB_POLYGON
{
public:

    PCB_PLANE( PCB_CALLBACKS* aCallbacks, BOARD* aBoard, int aPCadLayer );
    ~PCB_PLANE();

    virtual bool Parse( XNODE*          aNode,
                        wxString        aDefaultMeasurementUnit,
                        wxString        aActualConversion,
                        wxStatusBar*    aStatusBar );
};

} // namespace PCAD2KICAD

#endif    // PCB_PLANE_H_
