Viva
====

Viva is an open-source tool used to analyze traces (in the pajé
format) registered during the execution of parallel applications. The
tool serves also as a sandbox to the development of new visualization
techniques. Some features include:

* Temporal integration using dynamic time-intervals
* Spatial aggregation through hierarchical traces
* Interactive Graph Visualization with a force-directed algorithm

Viva is released under the [GNU Lesser General Public
Licence](http://www.gnu.org/licenses/lgpl.html) and received funding
from the [CAPES/Cofecub](http://www.capes.gov.br),
[CNPq](http://www.cnpq.br), and
[ANR](http://www.agence-nationale-recherche.fr/) agencies. Currently,
the tool is developed as part of the french [INFRA-SONGS ANR
project](http://infra-songs.gforge.inria.fr/).

Dependencies
------------

Viva needs wxWidgets, OpenGL, LibConfig, GTK2, GLUT.

Installation
------------

Just follow these steps:

     $ git clone git://scm.gforge.inria.fr/triva/viva.git
     $ mkdir build
     $ cd build
     $ cmake ..
     $ make

How to run
----------

If you are in the build directory, just type:

     $ ./viva -g <graph_configuration_file.txt> <trace_file.trace>

You might want to look into the examples directory.

Contact
-------

For more information, contact the author at: schnorr@gmail.com
