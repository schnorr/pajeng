#include "main.h"


IMPLEMENT_APP(TrivaWXApp)

bool TrivaWXApp::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  try {
    reader = new PajeFileReader (std::string(filename.utf8_str()), NULL);
    decoder = new PajeEventDecoder ();
    simulator = new PajeSimulator ();
    timeinterval = new TimeInterval ();
    vivagraph = new VivaGraph (std::string(configuration.mb_str()));

    connectComponents (reader, decoder);
    connectComponents (decoder, simulator);
    connectComponents (simulator, timeinterval);
    connectComponents (timeinterval, vivagraph);

    LoadingBar loading (wxT("Loading file..."), reader);
    loading.ShowModal ();
  }catch (std::string error){
    wxString error2(error.c_str(), wxConvUTF8);
    wxMessageDialog dial(NULL, error2, wxT("Error"), wxOK|wxICON_ERROR);
    dial.ShowModal();
  }

  simulator->report();

  try {
    window = new GraphWindow (NULL, vivagraph);
    window->Show(true);
    timeslicewindow = new TimeSliceWindow (window, timeinterval);
    timeslicewindow->Show (true);
  }catch (std::string error){
    std::cout << error << std::endl;
    exit(1);
    return false;
  }
  return true;
}

void TrivaWXApp::connectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->setOutputComponent (c2);
  c2->setInputComponent (c1);
}

int TrivaWXApp::OnExit()
{
  delete reader;
  delete decoder;
  delete simulator;
  delete vivagraph;
  return 0;
}

int TrivaWXApp::OnRun()
{
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
  parser.Found (_("g"), &configuration);
  //validate configuration
  config_t config;
  config_init (&config);
  if (config_read_file (&config, std::string(configuration.mb_str()).c_str()) == CONFIG_FALSE){
    wxString message;
    message << _("Parsing of configuration file '")
            << configuration
            << _("' failed with '")
            << wxString::FromUTF8(config_error_text(&config))
            << _("' at line ")
            << config_error_line (&config);
    wxMessageDialog *dial = new wxMessageDialog(NULL,
                                                message,
                                                wxT("Parsing Error"),
                                                wxOK | wxICON_ERROR);
    dial->ShowModal();
    config_destroy (&config);
    return false;
  }else{
    config_destroy (&config);
  }
  return true;
}
