PajeNG - Trace Visualization Tool
==================================

PajeNG (Paje Next Generation) is a re-implementation (in C++) of the
well-known [Paje visualization tool](http://paje.sf.net) for the
analysis of execution traces (in the [Paje File
Format](http://paje.sourceforge.net/download/publication/lang-paje.pdf)).
The tool is released under the [GNU General Public Licence
3](http://www.gnu.org/licenses/gpl.html). PajeNG comprises a library
(__libpaje__), the visualization tool (__pajeng__) and a set of
auxilirary tools to manage Paje trace files (such as __pj_dump__ and
__pj_validate__).

### Installation

We recommend an out-of-source compilation:

     $ git clone git://github.com/schnorr/pajeng.git
     $ mkdir -p pajeng/build
     $ cd pajeng/build
     $ cmake ..
     $ make
     $ make install