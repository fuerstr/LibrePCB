# LibrePCB

[![Travis Build Status](https://travis-ci.org/LibrePCB/LibrePCB.svg?branch=master)](https://travis-ci.org/LibrePCB/LibrePCB)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/jq41j14jlgs6rxsn/branch/master?svg=true)](https://ci.appveyor.com/project/librepcb/librepcb/branch/master)
[![Flattr this project](https://img.shields.io/badge/flattr-donate-yellow.svg)](https://flattr.com/submit/auto?user_id=LibrePCB&url=https://github.com/LibrePCB/LibrePCB&title=LibrePCB&language=&tags=github&category=software)
[![Donate with Bitcoin](https://img.shields.io/badge/bitcoin-donate-yellow.svg)](https://blockchain.info/address/1FiXZxoXe3px1nNuNygRb1NwcYr6U8AvG8)


## About LibrePCB

LibrePCB is a free [EDA](https://en.wikipedia.org/wiki/Electronic_design_automation)
software to develop printed circuit boards. It runs on Linux, Windows and Mac.
The project is still in a quite early development stage (no stable release available).

![Screenshot](doc/screenshot.png)

### Features

- Cross-platform (Unix/Linux, Mac OS X, Windows)
- Multilingual (both application and library elements)
- All-In-One: project management + library/schematic/board editors
- Intuitive, modern and easy-to-use graphical user interface
- Very powerful library design with some innovative concepts
- Human-readable file formats for both libraries and projects
- Multi-PCB feature (different PCB variants of the same schematic)
- Automatic netlist synchronisation between schematic and board


## Installation

There are no stable releases available yet, so basically you need to compile
LibrePCB by yourself. Nevertheless there are easier ways to get LibrePCB running:

- For Windows there are nightly builds available to download: 
  [librepcb-nightly.zip](https://ci.appveyor.com/api/projects/librepcb/librepcb/artifacts/build/librepcb-nightly.zip?branch=master)
- On Arch Linux you can install the package 
  [librepcb-git](https://aur.archlinux.org/packages/librepcb-git/) from the AUR.
- Using a [Docker](https://www.docker.com/) container you can build and run LibrePCB
  on any Linux without cluttering up your system with all the requirements. Just follow
  [these instructions](https://github.com/LibrePCB/LibrePCB/tree/master/dev/docker).

*Warning: Because LibrePCB's file format is not yet considered as stable (i.e.
breaking changes can occur), you should not yet use LibrePCB productively!*


## Development

### Requirements

To compile LibrePCB, you need the following software components:

- g++ >= 4.8, MinGW >= 4.8, or Clang >= 3.3 (C++11 support is required)
- [Qt](http://www.qt.io/download-open-source/) >= 5.2
- [zlib](http://www.zlib.net/)
- [OpenSSL](https://www.openssl.org/)

#### Installation on Debian/Ubuntu/Mint

```bash
sudo apt-get install git build-essential qt5-default qttools5-dev-tools libglu1-mesa-dev openssl zlib1g zlib1g-dev
sudo apt-get install qt5-doc qtcreator # optional
```

#### Installation on Arch Linux

```bash
sudo pacman -S git base-devel qt5-base qt5-tools desktop-file-utils shared-mime-info openssl zlib
sudo pacman -S qt5-doc qtcreator # optional
```

#### Installation on Mac OS X

1. Install Xcode through the app store and start it at least once (for the license)
2. Install [homebrew](https://github.com/Homebrew/brew) (**the** package manager)
3. Install *qt5*: `brew update && brew install qt5`
4. Make the toolchain available: `brew unlink qt && brew link --force qt5`

#### Installation on Windows

Download and run the 
[Qt for Windows (MinGW) installer](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-windows-x86-mingw530-5.8.0.exe) 
from [here](https://www.qt.io/download-open-source/). LibrePCB does not compile 
with MSVC, so you must install following components with the Qt installer:

- The MinGW compiler itself
- The Qt libraries for MinGW

### Cloning

It's important to clone the repository recursively to get all submodules too:

```bash
git clone --recursive https://github.com/LibrePCB/LibrePCB.git && cd LibrePCB
```

### Building

#### Using Qt Creator

Building with [Qt Creator](http://doc.qt.io/qtcreator/) is probably the easiest
way. To keep build time as low as possible make sure to set the correct make
flags to use all available CPU cores to build. See this [stackoverflow
answer](https://stackoverflow.com/questions/8860712/setting-default-make-options-for-qt-creator).

#### Using qmake and make

Since Qt Creator is also using qmake and make to build, it's easy to do the same
on the command line:

```bash
mkdir build && cd build
qmake -r ../librepcb.pro
make -j 8
```

### Run LibrePCB

#### From Qt Creator

Select the run configuration `librepcb` and click on the `Run` button:

![qtcreator_run](doc/qtcreator_run.png)

#### From Command Line

```bash
./generated/unix/librepcb                  # Unix/Linux
open ./build/generated/mac/librepcb.app    # Mac OS X
generated\windows\librepcb.exe             # Windows
```

### Installation

On a Unix/Linux system, LibrePCB can be installed with `sudo make install`.

### Workspace

At the first startup, LibrePCB asks for a workspace directory where the library
elements and projects will be saved.  For developers there is a demo workspace
inclusive library and projects in the submodule "dev/demo-workspace/".

*Note: As LibrePCB is still under heavy developement, it's highly recommended to
use this demo workspace. Otherwise you won't be able to create/use any 
components and some parts of the application will not work properly.*


## Credits

- First of all, many thanks to all of our [contributors](AUTHORS.md)!
- Thanks also to [cloudscale.ch](https://www.cloudscale.ch/) for sponsoring our 
  API server!


## License

LibrePCB is published under the [GNU GPLv3](http://www.gnu.org/licenses/gpl-3.0.html) license.
