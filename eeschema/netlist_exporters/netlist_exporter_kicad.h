/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2013 jp.charras at wanadoo.fr
 * Copyright (C) 2013 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 1992-2015 KiCad Developers
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

#ifndef NETLIST_EXPORTER_KICAD_H
#define NETLIST_EXPORTER_KICAD_H

#include <netlist_exporter_generic.h>

class OUTPUTFORMATTER;

/**
 * Class NETLIST_EXPORTER_KICAD
 * generates the kicad netlist format supported by pcbnew. It is basically
 * the generic netlist format just formatted slightly different.
 */
class NETLIST_EXPORTER_KICAD : public NETLIST_EXPORTER_GENERIC
{
public:
    NETLIST_EXPORTER_KICAD( NETLIST_OBJECT_LIST* aMasterList, PART_LIBS* aLibs ) :
        NETLIST_EXPORTER_GENERIC( aMasterList, aLibs )
    {
    }

    /**
     * Function WriteNetlist
     * writes to specified output file
     */
    bool WriteNetlist( const wxString& aOutFileName, unsigned aNetlistOptions );   // OVERRIDE

    /**
     * Function Format
     * outputs this s-expression netlist into @a aOutputFormatter.
     * @param aOutputFormatter is the destination of the serialization to text.
     * @param aCtl is bit set composed by OR-ing together enum GNL bits, it allows ouputting
     *  a subset of the full document model.
     * @throw IO_ERROR if any problems.
     */
    void Format( OUTPUTFORMATTER* aOutputFormatter, int aCtl );
};

#endif
