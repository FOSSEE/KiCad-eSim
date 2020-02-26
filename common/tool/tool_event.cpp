/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
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

#include <cstring>
#include <string>

#include <tool/tool_event.h>
#include <tool/tool_action.h>
#include <tool/tool_manager.h>

#include <boost/foreach.hpp>

struct FlagString
{
    int flag;
    std::string str;
};


static const std::string flag2string( int aFlag, const FlagString* aExps )
{
    std::string rv;

    for( int i = 0; aExps[i].str.length(); i++ )
    {
        if( aExps[i].flag & aFlag )
            rv += aExps[i].str + " ";
    }

    return rv;
}


bool TOOL_EVENT::IsAction( const TOOL_ACTION* aAction ) const
{
    return Matches( aAction->MakeEvent() );
}


const std::string TOOL_EVENT::Format() const
{
    std::string ev;

    const FlagString categories[] =
    {
        { TC_MOUSE,    "mouse"    },
        { TC_KEYBOARD, "keyboard" },
        { TC_COMMAND,  "command"  },
        { TC_MESSAGE,  "message"  },
        { TC_VIEW,     "view"     },
        { 0,           ""         }
    };

    const FlagString actions[] =
    {
        { TA_MOUSE_CLICK,           "click"               },
        { TA_MOUSE_DBLCLICK,        "double click"        },
        { TA_MOUSE_UP,              "button-up"           },
        { TA_MOUSE_DOWN,            "button-down"         },
        { TA_MOUSE_DRAG,            "drag"                },
        { TA_MOUSE_MOTION,          "motion"              },
        { TA_MOUSE_WHEEL,           "wheel"               },
        { TA_KEY_PRESSED,           "key-pressed"         },
        { TA_VIEW_REFRESH,          "view-refresh"        },
        { TA_VIEW_ZOOM,             "view-zoom"           },
        { TA_VIEW_PAN,              "view-pan"            },
        { TA_VIEW_DIRTY,            "view-dirty"          },
        { TA_CHANGE_LAYER,          "change-layer"        },
        { TA_CANCEL_TOOL,           "cancel-tool"         },
        { TA_CONTEXT_MENU_UPDATE,   "context-menu-update" },
        { TA_CONTEXT_MENU_CHOICE,   "context-menu-choice" },
        { TA_UNDO_REDO,             "undo-redo"           },
        { TA_ACTION,                "action"              },
        { TA_ACTIVATE,              "activate"            },
        { 0,                        ""                    }
    };

    const FlagString buttons[] =
    {
        { BUT_NONE,   "none"   },
        { BUT_LEFT,   "left"   },
        { BUT_RIGHT,  "right"  },
        { BUT_MIDDLE, "middle" },
        { 0,          ""       }
    };

    const FlagString modifiers[] =
    {
        { MD_SHIFT, "shift" },
        { MD_CTRL,  "ctrl"  },
        { MD_ALT,   "alt"   },
        { 0,        ""      }
    };

    ev = "category: ";
    ev += flag2string( m_category, categories );
    ev += " action: ";
    ev += flag2string( m_actions, actions );

    if( m_actions & TA_MOUSE )
    {
        ev += " btns: ";
        ev += flag2string( m_mouseButtons, buttons );
    }

    if( m_actions & TA_KEYBOARD )
    {
        char tmp[128];
        sprintf( tmp, "key: %d", m_keyCode );
        ev += tmp;
    }

    if( m_actions & ( TA_MOUSE | TA_KEYBOARD ) )
    {
        ev += " mods: ";
        ev += flag2string( m_modifiers, modifiers );
    }

    if( m_commandId )
    {
        char tmp[128];
        sprintf( tmp, "cmd-id: %d", *m_commandId );
        ev += tmp;
    }

    if( m_commandStr )
        ev += "cmd-str: " + ( *m_commandStr );

    return ev;
}


const std::string TOOL_EVENT_LIST::Format() const
{
    std::string s;

    BOOST_FOREACH( TOOL_EVENT e, m_events )
        s += e.Format() + " ";

    return s;
}
