#include "VivaGraph.h"

VivaGraph::VivaGraph (void)
{
}

void VivaGraph::inputEntity (PajeObject *data)
{
  throw "VivaGraph don't take inputEntity method";
}

void VivaGraph::hierarchyChanged (void)
{
  std::cout << "VivaGraph: " << __FUNCTION__ << std::endl;
  PajeContainer *root = rootInstance ();
  std::cout << root->identifier() << std::endl;

}
