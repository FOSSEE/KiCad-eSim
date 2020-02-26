#ifndef KIWAY_EXPRESS_H_
#define KIWAY_EXPRESS_H_
/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2014 KiCad Developers, see CHANGELOG.TXT for contributors.
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

// @see http://wiki.wxwidgets.org/Custom_Events_Tutorial

#include <wx/wx.h>
#include <frame_type.h>
#include <mail_type.h>


/**
 * Class KIWAY_EXPRESS
 * carries a payload from one KIWAY_PLAYER to another within a PROJECT.
 */
class KIWAY_EXPRESS : public wxEvent
{
public:
    /**
     * Function Dest
     * returns the destination player id of the message.
     */
    FRAME_T  Dest()                         { return m_destination; }

    /**
     * Function Command
     * returns the MAIL_T associated with this mail.
     */
    MAIL_T Command()
    {
        return (MAIL_T) GetId();    // re-purposed control id.
    }

    /**
     * Function Payload
     * returns the payload, which can be any text but it typicall self
     * identifying s-expression.
     */
    const std::string&  GetPayload()                    { return m_payload; }
    void SetPayload( const std::string& aPayload )      { m_payload = aPayload; }

    KIWAY_EXPRESS* Clone() const            { return new KIWAY_EXPRESS( *this ); }

    //KIWAY_EXPRESS() {}

    KIWAY_EXPRESS( FRAME_T aDestination,
            MAIL_T aCommand,
            const std::string& aPayload,
            wxWindow* aSource = NULL );

    KIWAY_EXPRESS( const KIWAY_EXPRESS& anOther );

    /// The wxEventType argument to wxEvent() and identifies an event class
    /// in a hurry.  These wxEventTypes also allow a common class to be used
    /// multiple ways.  Should be allocated at startup by wxNewEventType();
    static const wxEventType wxEVENT_ID;

    //DECLARE_DYNAMIC_CLASS( KIWAY_EXPRESS )

private:
    FRAME_T         m_destination;      ///< could have been a bitmap indicating multiple recipients
    std::string     m_payload;          ///< very often s-expression text, but not always

    // possible new ideas here.
};


typedef void ( wxEvtHandler::*kiwayExpressFunction )( KIWAY_EXPRESS& );

#define wxKiwayExressHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(kiwayExpressFunction, &func)


#define EVT_KIWAY_EXPRESS( func ) \
    DECLARE_EVENT_TABLE_ENTRY( \
        KIWAY_EXPRESS::wxEVENT_ID, -1, -1, \
        (wxObjectEventFunction) \
        (kiwayExpressFunction) & func, \
        (wxObject*) NULL ),


#endif  // KIWAY_EXPRESS_H_

