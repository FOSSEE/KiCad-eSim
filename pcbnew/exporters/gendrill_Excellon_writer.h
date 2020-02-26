/**
 * @file gendrill_Excellon_writer.h
 * @brief Classes used in drill files, map files and report files generation.
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2015 Jean_Pierre Charras <jp.charras at wanadoo.fr>
 * Copyright (C) 1992-2015 KiCad Developers, see change_log.txt for contributors.
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

#ifndef _GENDRILL_EXCELLON_WRITER_
#define _GENDRILL_EXCELLON_WRITER_

#include <vector>


class BOARD;
class PLOTTER;


// the DRILL_TOOL class  handles tools used in the excellon drill file:
class DRILL_TOOL
{
public:
    int m_Diameter;         // the diameter of the used tool (for oblong, the smaller size)
    int m_TotalCount;       // how many times it is used (round and oblong)
    int m_OvalCount;        // oblong count
    bool m_Hole_NotPlated;  // Is the hole plated or not plated

public:
    DRILL_TOOL( int aDiameter, bool a_NotPlated )
    {
        m_TotalCount     = 0;
        m_OvalCount      = 0;
        m_Diameter       = aDiameter;
        m_Hole_NotPlated = a_NotPlated;
    }
};


/* the HOLE_INFO class handle hole which must be drilled (diameter, position and layers)
 * For buried or micro vias, the hole is not on all layers.
 * So we must generate a drill file for each layer pair (adjacent layers)
 * Not plated holes are always through holes, and must be output on a specific drill file
 * because they are drilled after the Pcb process is finished.
 */
class HOLE_INFO
{
public:
    int         m_Hole_Diameter;        // hole value, and for oblong: min(hole size x, hole size y)
    int         m_Tool_Reference;       // Tool reference for this hole = 1 ... n (values <=0 must not be used)
    wxSize      m_Hole_Size;            // hole size for oblong holes
    double      m_Hole_Orient;          // Hole rotation (= pad rotation) for oblong holes
    int         m_Hole_Shape;           // hole shape: round (0) or oval (1)
    wxPoint     m_Hole_Pos;             // hole position
    LAYER_ID    m_Hole_Bottom_Layer;    // hole ending layer (usually back layer)
    LAYER_ID    m_Hole_Top_Layer;       // hole starting layer (usually front layer):
                                        // m_Hole_Top_Layer < m_Hole_Bottom_Layer
    bool        m_Hole_NotPlated;       // hole not plated. Must be in a specific drill file or section

public:
    HOLE_INFO()
    {
        m_Hole_NotPlated = false;
        m_Hole_Diameter = 0;
        m_Tool_Reference = 0;
        m_Hole_Orient = 0.0;
        m_Hole_Shape = 0;
        m_Hole_Bottom_Layer = B_Cu;
        m_Hole_Top_Layer = F_Cu;
    }
};


/* the DRILL_PRECISION helper class to handle drill precision format in excellon files
 */
class DRILL_PRECISION
{
public:
    int m_lhs;      // Left digit number (integer value of coordinates)
    int m_rhs;      // Right digit number (decimal value of coordinates)

public: DRILL_PRECISION( int l = 2, int r = 4 )
    {
        m_lhs = l; m_rhs = r;
    }


    wxString GetPrecisionString()
    {
        wxString text;

        text << m_lhs << wxT( ":" ) << m_rhs;
        return text;
    }
};


typedef std::pair<LAYER_ID, LAYER_ID>   LAYER_PAIR;
class OUTPUTFORMATTER;

/**
 * EXCELLON_WRITER is a class mainly used to create Excellon drill files
 * However, this class is also used to create drill maps and drill report
 */
class EXCELLON_WRITER
{
public:
    enum ZEROS_FMT {            // Zero format in coordinates
        DECIMAL_FORMAT,         // Floating point coordinates
        SUPPRESS_LEADING,       // Suppress leading zeros
        SUPPRESS_TRAILING,      // Suppress trainling zeros
        KEEP_ZEROS              // keep zeros
    };

    wxPoint                  m_Offset;                  // offset coordinates
    bool                     m_ShortHeader;             // true to generate the smallest header (strip comments)

private:
    FILE*                    m_file;                    // The output file
    BOARD*                   m_pcb;
    bool                     m_minimalHeader;           // True to use minimal header
                                                        // in excellon file (strip comments)
    bool                     m_unitsDecimal;            // true = decimal, false = inches
    ZEROS_FMT                m_zeroFormat;              // the zero format option for output file
    DRILL_PRECISION          m_precision;               // The current coordinate precision (not used in decimal format)
    double                   m_conversionUnits;         // scaling factor to convert the board unites to Excellon units
                                                        // (i.e inches or mm)
    bool                     m_mirror;
    wxPoint                  m_offset;                  // Drill offset coordinates
    bool                     m_merge_PTH_NPTH;          // True to generate only one drill file
    std::vector<HOLE_INFO>   m_holeListBuffer;          // Buffer containing holes
    std::vector<DRILL_TOOL>  m_toolListBuffer;          // Buffer containing tools

    PlotFormat              m_mapFileFmt;               // the format of the map drill file,
                                                        // if this map is needed
    const PAGE_INFO*        m_pageInfo;                 // the page info used to plot drill maps
                                                        // If NULL, use a A4 page format

public:
    EXCELLON_WRITER( BOARD* aPcb );

    ~EXCELLON_WRITER()
    {
    }

    /**
     * Return the plot offset (usually the position
     * of the auxiliary axis
     */
    const wxPoint GetOffset() { return m_offset; }

    /**
     * Function SetFormat
     * Initialize internal parameters to match the given format
     * @param aMetric = true for metric coordinates, false for imperial units
     * @param aZerosFmt =  DECIMAL_FORMAT, SUPPRESS_LEADING, SUPPRESS_TRAILING, KEEP_ZEROS
     * @param aLeftDigits = number of digits for integer part of coordinates
     *          if <= 0 (default), a suitable value will be used, depending on units
     * @param aRightDigits = number of digits for mantissa part of coordinates
     *          if <= 0 (default), a suitable value will be used, depending on units
     */
    void SetFormat( bool aMetric, ZEROS_FMT aZerosFmt = DECIMAL_FORMAT,
                    int aLeftDigits = 0, int aRightDigits = 0 );

    /**
     * Sets the page info used to plot drill maps
     * If NULL, a A4 page format will be used
     * @param aPageInfo = a reference to the page info, usually used to plot/display the board
     */
    void SetPageInfo( const PAGE_INFO* aPageInfo ) { m_pageInfo = aPageInfo; }

    /**
     * Function SetMapFileFormat
     * Initialize the format for the drill map file
     * @param SetMapFileFormat = a PlotFormat value (one of
     * PLOT_FORMAT_HPGL, PLOT_FORMAT_POST, PLOT_FORMAT_GERBER,
     * PLOT_FORMAT_DXF, PLOT_FORMAT_SVG, PLOT_FORMAT_PDF
     * the most useful are PLOT_FORMAT_PDF and PLOT_FORMAT_POST
     */
    void SetMapFileFormat( PlotFormat aMapFmt ) { m_mapFileFmt = aMapFmt; }


    /**
     * Function SetOptions
     * Initialize internal parameters to match drill options
     * @param aMirror = true to create mirrored coordinates (Y coordinates negated)
     * @param aMinimalHeader = true to use a minimal header (no comments, no info)
     * @param aOffset = drill coordinates offset
     */
    void SetOptions( bool aMirror, bool aMinimalHeader, wxPoint aOffset, bool aMerge_PTH_NPTH )
    {
        m_mirror = aMirror;
        m_offset = aOffset;
        m_minimalHeader = aMinimalHeader;
        m_merge_PTH_NPTH = aMerge_PTH_NPTH;
    }

    /**
     * Function BuildHolesList
     * Create the list of holes and tools for a given board
     * The list is sorted by increasing drill size.
     * Only holes included within aLayerPair are listed.
     * If aLayerPair identifies with [F_Cu, B_Cu], then
     * pad holes are always included also.
     *
     * @param aLayerPair is an inclusive range of layers.
     * @param aGenerateNPTH_list :
     *       true to create NPTH only list (with no plated holes)
     *       false to created plated holes list (with no NPTH )
     */
    void BuildHolesList( LAYER_PAIR aLayerPair,
                         bool aGenerateNPTH_list );

    int  GetHolesCount() const { return m_holeListBuffer.size(); }

    /**
     * Function CreateDrillandMapFilesSet
     * Creates the full set of Excellon drill file for the board
     * filenames are computed from the board name, and layers id
     * @param aPlotDirectory = the output folder
     * @param aGenDrill = true to generate the EXCELLON drill file
     * @param aGenMap = true to generate a drill map file
     * @param aReporter = a REPORTER to return activity or any message (can be NULL)
     */
    void CreateDrillandMapFilesSet( const wxString& aPlotDirectory,
                                    bool aGenDrill, bool aGenMap,
                                    REPORTER * aReporter = NULL );

    /**
     * Function CreateDrillFile
     * Creates an Excellon drill file
     * @param aFile = an opened file to write to will be closed by CreateDrillFile
     * @return hole count
     */
    int  CreateDrillFile( FILE * aFile );

    /**
     * Function GenDrillReportFile
     *  Create a plain text report file giving a list of drill values and drill count
     *  for through holes, oblong holes, and for buried vias,
     *  drill values and drill count per layer pair
     *  there is only one report for all drill files even when buried or blinds vias exist
     *
     * Here is a sample created by this function:
     *  Drill report for F:/tmp/interf_u/interf_u.brd
     *  Created on 04/10/2012 20:48:38
     *  Selected Drill Unit: Imperial (inches)
     *
     *  Drill report for plated through holes :
     *  T1  0,025"  0,64mm  (88 holes)
     *  T2  0,031"  0,79mm  (120 holes)
     *  T3  0,032"  0,81mm  (151 holes)  (with 1 slot)
     *  T4  0,040"  1,02mm  (43 holes)
     *  T5  0,079"  2,00mm  (1 hole)  (with 1 slot)
     *  T6  0,120"  3,05mm  (1 hole)  (with 1 slot)
     *
     *  Total plated holes count 404
     *
     *
     *  Drill report for buried and blind vias :
     *
     *  Drill report for holes from layer Soudure to layer Interne1 :
     *
     *  Total plated holes count 0
     *
     *
     *  Drill report for holes from layer Interne1 to layer Interne2 :
     *  T1  0,025"  0,64mm  (3 holes)
     *
     *  Total plated holes count 3
     *
     *
     *  Drill report for holes from layer Interne2 to layer Composant :
     *  T1  0,025"  0,64mm  (1 hole)
     *
     *  Total plated holes count 1
     *
     *
     *  Drill report for unplated through holes :
     *  T1  0,120"  3,05mm  (1 hole)  (with 1 slot)
     *
     *  Total unplated holes count 1
     *
     * @param aFullFileName : the name of the file to create
     *  m_unitsDecimal = false to use inches, true to use mm in report file
     *
     * @return success if the file is created
     */
    bool GenDrillReportFile( const wxString& aFullFileName );

    /**
     * Function GenDrillMapFile
     * Plot a map of drill marks for holes.
     * the paper sheet to use to plot the map is set in m_pageInfo
     * ( calls SetPageInfo() to set it )
     * if NULL, A4 format will be used
     * @param aFullFileName : the full filename of the map file to create,
     * @param aFormat : one of the supported plot formats (see enum PlotFormat )
     */
    bool GenDrillMapFile( const wxString& aFullFileName, PlotFormat aFormat );

private:
    /* Print the DRILL file header. The full header is:
     * M48
     * ;DRILL file {PCBNEW (2007-11-29-b)} date 17/1/2008-21:02:35
     * ;FORMAT={ <precision> / absolute / <units> / <numbers format>}
     * FMAT,2
     * INCH,TZ
     */
    void WriteEXCELLONHeader();

    void WriteEXCELLONEndOfFile();

    /* Created a line like:
     * X48000Y19500
     * According to the selected format
     */
    void WriteCoordinates( char* aLine, double aCoordX, double aCoordY );

    /** Helper function.
     * Writes the drill marks in HPGL, POSTSCRIPT or other supported formats
     * Each hole size has a symbol (circle, cross X, cross + ...) up to
     * PLOTTER::MARKER_COUNT different values.
     * If more than PLOTTER::MARKER_COUNT different values,
     * these other values share the same mark shape
     * @param aPlotter = a PLOTTER instance (HPGL, POSTSCRIPT ... plotter).
     */
    bool PlotDrillMarks( PLOTTER* aPlotter );

    /// Get unique layer pairs by examining the micro and blind_buried vias.
    std::vector<LAYER_PAIR> getUniqueLayerPairs() const;

    /**
     * Function printToolSummary
     * prints m_toolListBuffer[] tools to aOut and returns total hole count.
     * @param aOut = the current OUTPUTFORMATTER to print summary
     * @param aSummaryNPTH = true to print summary for NPTH, false for PTH
     */
    unsigned printToolSummary( OUTPUTFORMATTER& aOut, bool aSummaryNPTH ) const;

    const std::string layerPairName( LAYER_PAIR aPair ) const;

    const std::string layerName( LAYER_ID aLayer ) const;

    const wxString drillFileName( LAYER_PAIR aPair, bool aNPTH ) const;
};

#endif  //  #ifndef _GENDRILL_EXCELLON_WRITER_
