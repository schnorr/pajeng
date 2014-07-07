PajeNG - Trace Visualization Tool
==================================

PajeNG (Paje Next Generation) is a re-implementation (in C++) and
direct heir of the well-known [Paje visualization
tool](http://paje.sf.net) for the analysis of execution traces (in the
[Paje File
Format](http://paje.sourceforge.net/download/publication/lang-paje.pdf))
through trace visualization (space/time view).  The tool is released
under the [GNU General Public License
3](http://www.gnu.org/licenses/gpl.html). PajeNG comprises the
__libpaje__ library, the space-time visualization tool in __pajeng__
and a set of auxiliary tools to manage Paje trace files (such as
__pj_dump__ and __pj_validate__). It is developed as part of the
french [INFRA-SONGS ANR project](http://infra-songs.gforge.inria.fr/).

### Two-line Installation

     sudo apt-get install git cmake build-essential libqt4-dev libboost-dev freeglut3-dev asciidoc flex bison;
     git clone git://github.com/schnorr/pajeng.git ; mkdir -p pajeng/b ; cd pajeng/b ; cmake .. ; make install

### Detailed Installation and everything else

[Check the PajeNG's wiki](https://github.com/schnorr/pajeng/wiki/).