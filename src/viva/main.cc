#include "main.h"
#include <wx/event.h>
#include <wx/progdlg.h>
#include "loadingbar.h"

IMPLEMENT_APP(TrivaWXApp)

bool TrivaWXApp::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  try {
    reader = new PajeFileReader (std::string(filename.utf8_str()), NULL);
    decoder = new PajeEventDecoder ();
    simulator = new PajeSimulator ();

    reader->setOutputComponent (decoder);
    decoder->setInputComponent (reader);
    decoder->setOutputComponent (simulator);
    simulator->setInputComponent (decoder);

    LoadingBar loading (wxT("Loading file..."), reader);
    loading.ShowModal ();
  }catch (std::string error){
    wxString error2(error.c_str(), wxConvUTF8);
    wxMessageDialog dial(NULL, error2, wxT("Error"), wxOK|wxICON_ERROR);
    dial.ShowModal();
  }

  triview = new Triview(wxT("Triva"));
  triview->Show(true);
  return true;
}

int TrivaWXApp::OnExit()
{
  delete reader;
  delete decoder;
  delete simulator;
  delete triview;
  return 0;
}

int TrivaWXApp::OnRun()
{
  //run the program
  int exitcode = wxApp::OnRun();
  if (exitcode!=0){
    
    return exitcode;
  }
  return 0;
}
 
void TrivaWXApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  parser.SetDesc (g_cmdLineDesc);
  // must refuse '/' as parameter starter or cannot use "/path" style paths
  parser.SetSwitchChars (wxT("-"));
}
 
bool TrivaWXApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  filename = parser.GetParam(0);
  return true;
}
