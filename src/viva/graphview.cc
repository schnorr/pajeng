#include "graphview.h"

#define ID_QUALITY_0 0
#define ID_QUALITY_1 1
#define ID_QUALITY_2 2
#define ID_QUALITY_3 3
#define ID_QUALITY_4 4
#define ID_PLAY 5
#define ID_PAUSE 6

GraphView::GraphView (VivaGraph *vivagraph)
  : BasicFrame (wxT("Viva Graph View"))
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

  help = new wxMenu;
  help->Append(wxID_ABOUT, wxT("&About\tCtrl+A"));
  menubar->Append(help, wxT("&Help"));

  SetMenuBar(menubar);

  this->Connect(ID_QUALITY_0,
                ID_QUALITY_4,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphView::OnUpdateQuality));

  this->Connect(wxID_EXIT,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphView::OnQuit));

  this->Connect(wxID_OPEN,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphView::OnOpen));

  this->Connect(wxID_ABOUT,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(GraphView::OnAbout));

  this->Connect(wxEVT_PAINT,
                wxPaintEventHandler(GraphView::OnPaint));
  // this->Connect(wxEVT_MOTION,
  //               wxMouseEventHandler(GraphView::OnSearchNode));
  // this->Connect(wxEVT_LEFT_DOWN,
  //               wxMouseEventHandler(GraphView::OnLeftDown));
  // this->Connect(wxEVT_LEFT_UP,
  //               wxMouseEventHandler(GraphView::OnLeftUp));
  // this->Connect(TupiLayoutUpdated,
  //               wxCommandEventHandler(GraphView::OnLayoutUpdated));
  // this->Connect (wxEVT_ACTIVATE,
  //                wxActivateEventHandler(GraphView::OnActivate));

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
  //               wxCommandEventHandler(GraphView::OnThreadManagement));
}

GraphView::~GraphView (void)
{
}

void GraphView::OnUpdateQuality(wxCommandEvent& event)
{
  // int event_id = event.GetId();
  // if (event_id >= ID_QUALITY_0 &&
  //     event_id <= ID_QUALITY_4){
  //   layout_set_quality (layout, event_id);
  //   wxString str = wxT("Quality: ");
  //   str << event_id;
  //   SetStatusText(str, 2);
  //   layout_reset_energies(layout);
  //   start_runner();
  // }
}


void GraphView::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void GraphView::OnOpen(wxCommandEvent& WXUNUSED(event))
{
}

void GraphView::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void GraphView::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxColour black, gray, white;
  black.Set(wxT("#000000"));
  gray.Set(wxT("#c0c0c0"));
  white.Set(wxT("#ffffff"));

  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  dc.DrawLine (10, 10, 20, 20);
}
