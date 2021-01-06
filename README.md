[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub issues](https://img.shields.io/github/issues/fossee/KiCad-eSim)](https://github.com/fossee/KiCad-eSim/issues)
[![GitHub forks](https://img.shields.io/github/forks/fossee/KiCad-eSim)](https://github.com/fossee/KiCad-eSim/network)
[![GitHub stars](https://img.shields.io/github/stars/fossee/KiCad-eSim)](https://github.com/fossee/KiCad-eSim/stargazers)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat)](https://github.com/fossee/KiCad-eSim)
![GitHub contributors](https://img.shields.io/github/contributors/fossee/kicad-esim)

KiCad Installer for eSim
====

This repository is maintained by FOSSEE team for KiCad v4.0.7. It contains all the documentation for making KiCad executable for Windows 7 and above and has been customized for the needs of eSim.

> Note: Most of the contents of `master` branch of this repository has been made available by extracting the KiCad installer from - https://kicad-downloads.s3.cern.ch/windows/stable/kicad-4.0.7-i686.exe.
> The difference with the original KiCad installer is the removal of 3D packages, help doc in all languages (except English) and addition of eSim compatible KiCad libraries.


## Building KiCad installer for eSim

1. Take the `master` branch of of this repository. Rename the folder to `kicad`.

2. Remove following files/directories from the `kicad` folder:
	- `.git` folder
	- kicad-4.0.7.tar.xz
	- README.md
    - share/doc/kicad/help/de
    - share/doc/kicad/help/es
    - share/doc/kicad/help/fr
    - share/doc/kicad/help/it
    - share/doc/kicad/help/ja
    - share/doc/kicad/help/nl
    - share/doc/kicad/help/pl
    - previous kicad installer (if any)

3. Compress the folders `bin`, `lib`, `share` and `ssl` into file named **KiCad.7z**.

    > Note: 7z compresssion tool can be obtained at - https://www.7-zip.org/download.html

3. Goto the `NSIS` folder and compile the script `install.nsi`.

    > Note: NSIS compiler can be obtained at - https://prdownloads.sourceforge.net/nsis/nsis-3.05-setup.exe?download

4. Use the generated `kicad-4.0.7-i686.exe` in the `NSIS` folder as the default installer for KiCad when building installer for eSim.

    > Note: This new generated installer should be pushed to the repository for any future requirements
