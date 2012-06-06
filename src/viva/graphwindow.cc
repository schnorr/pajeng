#include "graphwindow.h"
#include <limits>

#define ID_QUALITY_0 0
#define ID_QUALITY_1 1
#define ID_QUALITY_2 2
#define ID_QUALITY_3 3
#define ID_QUALITY_4 4
#define ID_PLAY 5
#define ID_PAUSE 6
#define ID_VIEW_TIMEINTERVAL 7
#define ID_VIEW_GRAPHCONFIGURATION 8
#define ID_GOTO_BOTTOM 9
#define ID_GO_DOWN 10
#define ID_GO_UP 11
#define ID_GOTO_TOP 12
#define ID_REFRESH 13

DEFINE_EVENT_TYPE (CreateScaleSliders)

GraphWindow::GraphWindow (wxWindow *parent, VivaGraph *vivagraph)
  : wxFrame(parent, wxID_ANY, wxT("Viva Graph Window"), wxDefaultPosition, wxSize(700,400))
{
  this->vivagraph = vivagraph;

  menubar = new wxMenuBar;
  file = new wxMenu;
  file->Append(wxID_OPEN);
  file->AppendSeparator();
  file->Append(wxID_EXIT);
  menubar->Append(file, wxT("&File"));

  edit = new wxMenu;
  wxMenu *quality = new wxMenu;
  quality->AppendRadioItem(ID_QUALITY_0, wxT("&0 (lower quality)\tCtrl+0"));
  quality->AppendRadioItem(ID_QUALITY_1, wxT("&1\tCtrl+1"));
  quality->AppendRadioItem(ID_QUALITY_2, wxT("&2\tCtrl+2"));
  quality->AppendRadioItem(ID_QUALITY_3, wxT("&3\tCtrl+3"));
  quality->AppendRadioItem(ID_QUALITY_4, wxT("&4 (higher quality)\tCtrl+4"));
  edit->AppendSubMenu(quality, wxT("Quality"));

  menubar->Append(edit, wxT("&Edit"));

  view = new wxMenu;
  menubar->Append(view, wxT("&View"));
  view->AppendCheckItem (ID_VIEW_TIMEINTERVAL, wxT("&Time Interval\tCtrl+T"));
  view->AppendCheckItem (ID_VIEW_GRAPHCONFIGURATION, wxT("&Graph Configuration\tCtrl+G"));

  help = new wxMenu;
  help->Append(wxID_ABOUT, wxT("&About\tCtrl+A"));
  menubar->Append(help, wxT("&Help"));

  SetMenuBar(menubar);

  this->Connect(ID_VIEW_TIMEINTERVAL, wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler (GraphWindow::OnTimeIntervalMenu));

  this->Connect(ID_QUALITY_0,
                ID_QUALITY_4,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphWindow::OnUpdateQuality));

  this->Connect(wxID_EXIT,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphWindow::OnQuit));

  this->Connect(wxID_OPEN,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphWindow::OnOpen));

  this->Connect(wxID_ABOUT,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphWindow::OnAbout));
 
  //status bar
  CreateStatusBar(3);

  //tool bar
  wxToolBar *toolbar = CreateToolBar(wxTB_TEXT);
  toolbar->AddTool(ID_PLAY,
                   wxT("Play"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-play"), wxART_TOOLBAR),
                   wxT("Play the particle system algorithm"));
  toolbar->AddTool(ID_PAUSE,
                   wxT("Stop"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-stop"), wxART_TOOLBAR),
                   wxT("Stop the particle system algorithm"));
  toolbar->AddTool(ID_GOTO_BOTTOM,
                   wxT("Bottom"),
                   wxArtProvider::GetBitmap(wxT("gtk-goto-bottom"), wxART_TOOLBAR),
                   wxT("Expand as much as possible the current nodes of the graph"));
  toolbar->AddTool(ID_GO_DOWN,
                   wxT("Down"),
                   wxArtProvider::GetBitmap(wxT("gtk-go-down"), wxART_TOOLBAR),
                   wxT("Expand by one level all current nodes of the graph"));
  toolbar->AddTool(ID_GO_UP,
                   wxT("Up"),
                   wxArtProvider::GetBitmap(wxT("gtk-go-up"), wxART_TOOLBAR),
                   wxT("Collapse by one level all current nodes of the graph"));
  toolbar->AddTool(ID_GOTO_TOP,
                   wxT("Top"),
                   wxArtProvider::GetBitmap(wxT("gtk-goto-top"), wxART_TOOLBAR),
                   wxT("Collapse as much as possible the current nodes of the graph"));
  toolbar->AddTool(ID_REFRESH,
                   wxT("Shake"),
                   wxArtProvider::GetBitmap(wxT("gtk-refresh"), wxART_TOOLBAR),
                   wxT("Shake the graph layout"));

  toolbar->Realize();
  this->Connect(ID_PLAY,
                ID_PAUSE,
                wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(GraphWindow::OnFDThreadManagement));
  this->Connect(ID_GOTO_BOTTOM,
                ID_GOTO_TOP,
                wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(GraphWindow::OnGoButtonsPressed));
  this->Connect(ID_REFRESH,
                wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(GraphWindow::OnRefreshButtonPressed));

  main_panel = new wxPanel(this, -1);
  main_vbox = new wxBoxSizer (wxVERTICAL);

  //creating the graphframe where the graph is drawn
  GraphFrame *graphframe = new GraphFrame (main_panel, -1);
  main_vbox->Add (graphframe, 1, wxEXPAND);

  this->Connect(CreateScaleSliders,
                wxCommandEventHandler(GraphWindow::OnCreateScaleSliders));
  this->Connect (wxEVT_SCROLL_THUMBTRACK,
                 wxScrollEventHandler(GraphWindow::OnScaleSliderChanged));

  main_panel->SetSizer(main_vbox);
  Centre();

  //try to create sliders now
  wxCommandEvent event (CreateScaleSliders);
  wxPostEvent (this, event);

  //let everything work smoothly with filters
  vivagraph->setWindow (this);
  graphframe->setVivaGraph (vivagraph);
}

GraphWindow::~GraphWindow ()
{
}

double GraphWindow::scaleSliderValue (std::string name)
{
  if (!vivagraph){
    return COMPOSITION_DEFAULT_USER_SCALE;
  }
  if (scaleSliders.size () == 0){
    wxCommandEvent event (CreateScaleSliders);
    wxPostEvent (this, event);
    return COMPOSITION_DEFAULT_USER_SCALE;
  }

  if (scaleSliders.count(name) == 0){
    return COMPOSITION_DEFAULT_USER_SCALE;
  }else{
    int max = std::numeric_limits<int>::max();
    return (double)scaleSliders[name]->GetValue()/max;
  }
}

void GraphWindow::OnTimeIntervalMenu (wxCommandEvent& event)
{
}

void GraphWindow::OnGraphConfigurationMenu (wxCommandEvent& event)
{
}

void GraphWindow::OnUpdateQuality(wxCommandEvent& event)
{
  int event_id = event.GetId();
  if (event_id >= ID_QUALITY_0 &&
      event_id <= ID_QUALITY_4){
    wxString str = wxT("Quality: ");
    str << event_id;
    SetStatusText(str, 2);
    vivagraph->qualityChanged (event_id);
  }
}


void GraphWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void GraphWindow::OnOpen(wxCommandEvent& WXUNUSED(event))
{
}

void GraphWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void GraphWindow::OnScaleSliderChanged (wxScrollEvent& WXUNUSED(event))
{
  vivagraph->scaleSliderChanged ();
}

void GraphWindow::OnFDThreadManagement(wxCommandEvent& event)
{
  if (!vivagraph) return;
  switch (event.GetId()){
  case ID_PLAY: vivagraph->start_runner(); break;
  case ID_PAUSE: vivagraph->stop_runner(); break;
  default: break;
  }
}

void GraphWindow::OnGoButtonsPressed(wxCommandEvent& event)
{
  if (!vivagraph) return;
  switch (event.GetId()){
  case ID_GOTO_BOTTOM: vivagraph->go_bottom(); break;
  case ID_GO_DOWN: vivagraph->go_down(); break;
  case ID_GO_UP: vivagraph->go_up(); break;
  case ID_GOTO_TOP: vivagraph->go_top(); break;
  default: break;
  }
}

void GraphWindow::OnRefreshButtonPressed(wxCommandEvent& event)
{
  vivagraph->refresh ();
}

void GraphWindow::OnCreateScaleSliders (wxCommandEvent& event)
{
  if (!vivagraph){
    throw "vivagraph is not defined on "+std::string(__FUNCTION__);
  }

  if (!vivagraph->compositionsScale.size()){
    return;
  }

  static int created = 0;
  if (created) return;

  //creating the sliders to let user control the scale of compositions
  wxBoxSizer *scale_sliders = new wxBoxSizer (wxHORIZONTAL);
  std::map<std::string,double> comp = vivagraph->compositionsScale;
  std::map<std::string,double>::iterator it;
  for (it = comp.begin(); it != comp.end(); it++){
    std::string confname = (*it).first;
    double value = (*it).second;
    int max = std::numeric_limits<int>::max();
    wxSlider *slider = new wxSlider (main_panel, wxID_ANY, max * COMPOSITION_DEFAULT_USER_SCALE, 0, max);
    wxStaticText *start_text = new wxStaticText (main_panel, -1, wxString(confname.c_str(), wxConvUTF8));
    wxSizerFlags flags;
    scale_sliders->Add (start_text);
    scale_sliders->Add (slider, 1, wxEXPAND);

    //saving scale slider so we can get its value after
    scaleSliders[confname] = slider;
  }
  main_vbox->Add(scale_sliders, 0, wxEXPAND);
  main_panel->Fit();
  Fit();

  created = 1;
}
