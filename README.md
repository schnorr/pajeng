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
__libpaje__ library,
and a set of auxiliary tools to manage Paje trace files (such as
__pj_dump__ and __pj_validate__). The space-time visualization tool
called __pajeng__ is deprecated (removed from the sources) since modern
tools to a better job (see __pj_gantt__, for instance, or take a more general approach
using R+ggplot2 to visualize the output of __pj_dump__). This effort
was started as part of the french
[INFRA-SONGS ANR
project](http://infra-songs.gforge.inria.fr/). Development has
continued through a collaboration between [INF/UFRGS](http://www.inf.ufrgs.br/en/) and [INRIA](https://www.inria.fr/).

### Two-line Installation

     sudo apt-get install git cmake build-essential libqt4-dev libqt4-opengl-dev libboost-dev freeglut3-dev asciidoc flex bison;
     git clone git://github.com/schnorr/pajeng.git ; mkdir -p pajeng/b ; cd pajeng/b ; cmake .. ; make install

### Detailed Installation and everything else

[Check the PajeNG's wiki](https://github.com/schnorr/pajeng/wiki/).