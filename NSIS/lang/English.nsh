;Additional text definitions for English

;File name of license file
LicenseLangString MUILicense ${LANG_ENGLISH} "..\COPYRIGHT.txt"

;Welcome page
LangString WELCOME_PAGE_TEXT ${LANG_ENGLISH} "This installer will guide you through the installation of KiCad ${PACKAGE_VERSION}.$\r$\n$\r$\n\
It is not required to close any other applications before starting the installer, neither is it necessary to reboot your computer.$\r$\n$\r$\n\
This is free open source software licensed under the GPL.$\r$\n$\r$\n\
Click Next to continue."

;Error messages
LangString ERROR_ADMIN_REQ  ${LANG_ENGLISH} "Admin rights are required to install KiCad!"
LangString ERROR_WIN9X      ${LANG_ENGLISH} "Error! This can't run under Windows 9x!"
LangString ERROR_WIN_MIN    ${LANG_ENGLISH} "A minimum of Windows 8.1 is required to install KiCad using this installer"
LangString ERROR_UNINSTALL_FIRST    ${LANG_ENGLISH} "You must run the uninstaller on your existing KiCad ${KICAD_VERSION} installation to proceed with installation"

;Other languages
LangString LANGUAGE_NAME_EN ${LANG_ENGLISH} "English"
LangString LANGUAGE_NAME_DE ${LANG_ENGLISH} "German"
LangString LANGUAGE_NAME_ES ${LANG_ENGLISH} "Spanish"
LangString LANGUAGE_NAME_FR ${LANG_ENGLISH} "French"
LangString LANGUAGE_NAME_IT ${LANG_ENGLISH} "Italian"
LangString LANGUAGE_NAME_JA ${LANG_ENGLISH} "Japanese"
LangString LANGUAGE_NAME_NL ${LANG_ENGLISH} "Dutch"
LangString LANGUAGE_NAME_PL ${LANG_ENGLISH} "Polish"
LangString LANGUAGE_NAME_ZH ${LANG_ENGLISH} "Chinese"

;Component option
LangString TITLE_SEC_MAIN ${LANG_ENGLISH} "Main application"
LangString TITLE_SEC_LIBRARIES ${LANG_ENGLISH} "Libraries"
LangString TITLE_SEC_SCHLIB ${LANG_ENGLISH} "Schematic libraries"
LangString TITLE_SEC_FPLIB ${LANG_ENGLISH} "Footprint libraries"
LangString TITLE_SEC_PACKAGES3D ${LANG_ENGLISH} "Footprint 3d models"
LangString TITLE_SEC_DEMOS ${LANG_ENGLISH} "Demonstration projects"
LangString TITLE_SEC_DOCS ${LANG_ENGLISH} "Help files"
LangString TITLE_SEC_FILE_ASSOC ${LANG_ENGLISH} "File associations"

;Component option descriptions
LangString DESC_SEC_MAIN ${LANG_ENGLISH} "Main application files."
LangString DESC_SEC_SCHLIB_DOWNLOAD ${LANG_ENGLISH} "Schematic libraries are recommended to install. They will be downloaded separately unless installer detects a previously downloaded archive."
LangString DESC_SEC_SCHLIB ${LANG_ENGLISH} "Schematic libraries are required unless they have been previously installed."
LangString DESC_SEC_FPLIB_DOWNLOAD ${LANG_ENGLISH} "Footprint libraries are recommended to install. They will be downloaded separately unless installer detects a previously downloaded archive."
LangString DESC_SEC_FPLIB ${LANG_ENGLISH} "Footprint libraries are required unless they have been previously installed."
LangString DESC_SEC_PACKAGES3D_DOWNLOAD ${LANG_ENGLISH} "Footprint 3d models are recommended to install. They will be downloaded separately unless installer detects a previously downloaded archive."
LangString DESC_SEC_PACKAGES3D ${LANG_ENGLISH} "Footprint 3d models are recommended to install."
LangString DESC_SEC_DEMOS ${LANG_ENGLISH} "Some demonstration projects and tutorials."
LangString DESC_SEC_DOCS ${LANG_ENGLISH} "Help files in PDF format."
LangString DESC_SEC_DOCS_EN ${LANG_ENGLISH} "$(LANGUAGE_NAME_EN) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_DE ${LANG_ENGLISH} "$(LANGUAGE_NAME_DE) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_ES ${LANG_ENGLISH} "$(LANGUAGE_NAME_ES) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_FR ${LANG_ENGLISH} "$(LANGUAGE_NAME_FR) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_IT ${LANG_ENGLISH} "$(LANGUAGE_NAME_IT) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_JA ${LANG_ENGLISH} "$(LANGUAGE_NAME_JA) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_NL ${LANG_ENGLISH} "$(LANGUAGE_NAME_NL) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_PL ${LANG_ENGLISH} "$(LANGUAGE_NAME_PL) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_DOCS_ZH ${LANG_ENGLISH} "$(LANGUAGE_NAME_ZH) $(TITLE_SEC_DOCS)"
LangString DESC_SEC_FILE_ASSOC ${LANG_ENGLISH} "Creates file associations for KiCad related files"

;File association descriptions (show in Windows Explorer)
LangString FILE_DESC_KICAD_PCB ${LANG_ENGLISH} "KiCad Board"
LangString FILE_DESC_SCH ${LANG_ENGLISH} "KiCad Schematic"
LangString FILE_DESC_PRO ${LANG_ENGLISH} "KiCad Project"
LangString FILE_DESC_KICAD_WKS ${LANG_ENGLISH} "KiCad Page Layout"

;Application Friendly Names (for windows explorer hook)
LangString APP_FRIENDLY_KICAD ${LANG_ENGLISH} "KiCad"
LangString APP_FRIENDLY_PCBNEW ${LANG_ENGLISH} "KiCad - Pcbnew"
LangString APP_FRIENDLY_EESCHEMA ${LANG_ENGLISH} "KiCad - Eeschema"
LangString APP_FRIENDLY_PLEDITOR ${LANG_ENGLISH} "KiCad - Page Layout Editor"

;Application names
LangString APP_NAME_KICAD ${LANG_ENGLISH} "KiCad"
LangString APP_NAME_PCBNEW ${LANG_ENGLISH} "Pcbnew"
LangString APP_NAME_EESCHEMA ${LANG_ENGLISH} "Eeschema"
LangString APP_NAME_PLEDITOR ${LANG_ENGLISH} "Page Layout Editor"
LangString APP_NAME_PCBCALCULATOR ${LANG_ENGLISH} "PCB Calculator"
LangString APP_NAME_BITMAP2COMPONENT ${LANG_ENGLISH} "Bitmap to Component"
LangString APP_NAME_GERBVIEW ${LANG_ENGLISH} "Gerbview"

;General messages
LangString PROGRAM_IS_OPEN_ERROR ${LANG_ENGLISH} "$R1 is currently running! You must close the program before you are allowed continue."

LangString FREECAD_PROMPT ${LANG_ENGLISH} "To edit or create 3D object models you may install FreeCAD. \
FreeCAD and user manual can be download free from the FreeCAD web page. Check this box to open the FreeCAD web page."

LangString CLEANUP_PROMPT ${LANG_ENGLISH} "Remove downloaded files after installation?"
LangString UNINST_PROMPT ${LANG_ENGLISH} "Are you sure you want to completely remove $(^Name) and all of its components? $\n\
This will also remove all modified and new files, symbols and footprints in the program directory \
(including python modules installed by user)!"

LangString UNINST_SUCCESS ${LANG_ENGLISH} "$(^Name) was successfully removed from your computer."
LangString INSTALLER_RUNNING ${LANG_ENGLISH} "The installer is already running."
LangString UNINSTALLER_RUNNING ${LANG_ENGLISH} "The uninstaller is already running."
LangString ALREADY_INSTALLED ${LANG_ENGLISH} "${PRODUCT_NAME} is already installed. Installing this package will overwrite existing files. Do you want to continue?"


;Install details
LangString INSTALLING_APPS ${LANG_ENGLISH} "Installing application..."
LangString INSTALLING_PIP ${LANG_ENGLISH} "Installing pip..."
LangString INSTALLING_SCH_LIBS ${LANG_ENGLISH} "Installing schematic libraries..."
LangString INSTALLING_PCB_LIBS ${LANG_ENGLISH} "Installing PCB libraries..."
LangString INSTALLING_3D_MODELS ${LANG_ENGLISH} "Installing 3D models..."
LangString INSTALLING_DEMOS ${LANG_ENGLISH} "Installing demos..."
LangString INSTALLING_DOCUMENTATION ${LANG_ENGLISH} "Installing documentation..."
LangString SETTING_FILE_ASSOCS ${LANG_ENGLISH} "Setting file associations..."
LangString CREATING_SHORTCUTS ${LANG_ENGLISH} "Creating shortcuts..."
LangString CREATING_PROGRAM_ENTRY ${LANG_ENGLISH} "Creating Program entry..."
LangString REMOVING_SHORTCUTS ${LANG_ENGLISH} "Removing shortcuts..."
LangString REMOVING_APP ${LANG_ENGLISH} "Removing application..."
LangString REMOVING_LIBRARIES ${LANG_ENGLISH} "Removing libraries..."
LangString REMOVING_DOCS ${LANG_ENGLISH} "Removing documentation..."
LangString REMOVING_ENV_VARS ${LANG_ENGLISH} "Removing environment variables..."
LangString REMOVING_FILE_ASSOC ${LANG_ENGLISH} "Removing file associations..."
