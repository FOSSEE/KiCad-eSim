/*
 * KiRouter - a push-and-(sometimes-)shove PCB router
 *
 * Copyright (C) 2014  CERN
 * Author: Maciej Suminski <maciej.suminski@cern.ch>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.or/licenses/>.
 */

/**
 * Push and Shove router settings dialog.
 */

#ifndef __dialog_pns_settings__
#define __dialog_pns_settings__

#include "dialog_pns_settings_base.h"

class PNS_ROUTING_SETTINGS;

class DIALOG_PNS_SETTINGS : public DIALOG_PNS_SETTINGS_BASE
{
	public:
		DIALOG_PNS_SETTINGS( wxWindow* aParent, PNS_ROUTING_SETTINGS& aSettings );

	private:
        virtual void OnOkClick( wxCommandEvent& aEvent );

		PNS_ROUTING_SETTINGS& m_settings;
};

#endif // __dialog_pns_settings__
