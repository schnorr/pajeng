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

GraphWindow::GraphWindow (wxWindow *parent, VivaGraph *vivagraph)
  : wxFrame(parent, wxID_ANY, wxT("Viva Graph Window"), wxDefaultPosition, wxSize(700,400))
{
  this->vivagraph = vivagraph;
  vivagraph->setWindow (this);

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
  wxToolBar *toolbar = CreateToolBar();
  toolbar->AddTool(ID_PLAY,
                   wxT("Play"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-play"),
                                            wxART_TOOLBAR));
  toolbar->AddTool(ID_PAUSE,
                   wxT("Pause"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-pause"),
                                            wxART_TOOLBAR));
  toolbar->Realize();
  // this->Connect(ID_PLAY,
  //               ID_PAUSE,
  //               wxEVT_COMMAND_TOOL_CLICKED,
  //               wxCommandEventHandler(GraphFrame::OnThreadManagement));

  wxPanel *panel = new wxPanel(this, -1);
  GraphFrame *graphframe = new GraphFrame (panel, -1);
  graphframe->setVivaGraph (vivagraph);
  wxBoxSizer *vbox = new wxBoxSizer (wxVERTICAL);
  vbox->Add (graphframe, 1, wxEXPAND);

  wxBoxSizer *scale_sliders = new wxBoxSizer (wxHORIZONTAL);

  std::map<std::string,double> comp = vivagraph->compositionsScale;
  std::map<std::string,double>::iterator it;
  for (it = comp.begin(); it != comp.end(); it++){
    std::string confname = (*it).first;
    double value = (*it).second;
    int max = std::numeric_limits<int>::max();
    wxSlider *slider = new wxSlider (panel, wxID_ANY, max * COMPOSITION_DEFAULT_USER_SCALE, 0, max);
    wxStaticText *start_text = new wxStaticText (panel, -1, wxString(confname.c_str(), wxConvUTF8));
    wxSizerFlags flags;
    scale_sliders->Add (start_text);
    scale_sliders->Add (slider, 1, wxEXPAND);

    //saving scale slider so we can get its value after
    scaleSliders[confname] = slider;
  }
  vbox->Add(scale_sliders, 0, wxEXPAND);

  this->Connect (wxEVT_SCROLL_THUMBTRACK,
                 wxScrollEventHandler(GraphWindow::OnScaleSliderChanged));

  panel->SetSizer(vbox);
  Centre();
}

GraphWindow::~GraphWindow ()
{
}

double GraphWindow::scaleSliderValue (std::string name)
{
  int max = std::numeric_limits<int>::max();
  if (scaleSliders.count(name) == 0){
    std::cout << "Warning, no scale slider for " << name << std::endl;
    return COMPOSITION_DEFAULT_USER_SCALE;
  }else{
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
