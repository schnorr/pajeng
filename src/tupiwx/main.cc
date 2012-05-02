#include "main.h"
#include <wx/event.h>

IMPLEMENT_APP(MyApp)

int node_compare (const void *p1, const void *p2)
{
  return strcmp (((tp_node*)p1)->name, ((tp_node*)p2)->name);
}
tp_dynar *nodes = dynar_new (sizeof(tp_node*), &node_compare);

bool MyApp::OnInit()
{
  std::cout << __FUNCTION__ << std::endl;
  if (!wxApp::OnInit())
    return false;

  tupiew = new Tupiew(wxT("Tupi"));
  tupiew->Show(true);
  return true;
}

int MyApp::OnExit()
{
  std::cout << "#" << __FUNCTION__ << std::endl;


  //traverse nodes, remove all, free
  int i;
  for (i = 0; i < dynar_count (nodes); i++){
    tp_node *node = *(tp_node**)dynar_get_ptr (nodes, i);
    layout_remove_node (layout, node);
  }
  dynar_free (nodes);
  layout_free (layout);

  std::cout << "FinAL " << __FUNCTION__ << std::endl;
  return 0;
}

int MyApp::OnRun()
{
  std::cout << __FUNCTION__ << std::endl;

  FILE *file = fopen (filename.utf8_str(), "r");
  if (!file){
    fprintf (stderr, "Could not open file\n");
    return 1;
  }
  aginit();
  g = agread (file);
  if (!g){
    fprintf (stderr, "Could not read a graph from file (agread)\n");
    return 1;
  }
  fclose(file);

  layout = layout_new ();

  //reading nodes/edges
  Agnode_t *n1, *n2;
  for (n1 = agfstnode (g); n1; n1 = agnxtnode (g, n1)){
    tp_node *node = node_new (n1->name);
    dynar_add_as (nodes, tp_node*, node);
    layout_add_node (layout, node);
  }
  for (n1 = agfstnode (g); n1; n1 = agnxtnode (g, n1)){
    for (n2 = agfstnode (g); n2; n2 = agnxtnode (g, n2)){
      if (agfindedge (g, n1, n2)){
        tp_node *node_n1 = layout_find_node (layout, n1->name);
        tp_node *node_n2 = layout_find_node (layout, n2->name);
        node_connect (node_n1, node_n2);
      }
    }
  }
  agclose (g);

  //run the program
  int exitcode = wxApp::OnRun();
  if (exitcode!=0){
    
    return exitcode;
  }
  return 0;
}
 
void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  std::cout << __FUNCTION__ << std::endl;
  parser.SetDesc (g_cmdLineDesc);
  // must refuse '/' as parameter starter or cannot use "/path" style paths
  parser.SetSwitchChars (wxT("-"));
}
 
bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  std::cout << __FUNCTION__ << std::endl;
  filename = parser.GetParam(0);
  return true;
}
