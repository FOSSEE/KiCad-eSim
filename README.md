# KiCad-eSim
This repository is maintained by FOSSEE team for KiCad v4.0.7 

## Building KiCad installer for eSim

1. Download the KiCad installer from - https://kicad-downloads.s3.cern.ch/windows/stable/kicad-4.0.7-i686.exe

2. Install KiCad from the executable [kicad-4.0.7-i686.exe] downloaded in step 1 at preferred location. Henceforth this location is to be referred to as `installdirectory`.

3. Clone the folder `NSIS` present in this repository and place it at the KiCad install location, i.e. installdirectory/.

4. Delete the folder `packages3d` present at the location installdirectory/share/kicad/modules/.

5. Compress the folders `bin`, `lib`, `share` and `ssl` into file named **KiCad.7z**. This file should be placed at `installdirectory/`.\
(7z compresssion tool can be obtained at - https://www.7-zip.org/download.html )

6. Goto the `NSIS` folder and compile the script `install.nsi`.\
(NSIS compiler can be obtained at - https://prdownloads.sourceforge.net/nsis/nsis-3.05-setup.exe?download)

7. Use the generated `kicad-4.0.7-i686.exe` in the `NSIS` folder as the default installer for KiCad when building installer for eSim.

8. Uninstall the KiCad used to build this installer.
