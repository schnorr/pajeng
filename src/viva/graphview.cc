#include "graphview.h"

#define ID_QUALITY_0 0
#define ID_QUALITY_1 1
#define ID_QUALITY_2 2
#define ID_QUALITY_3 3
#define ID_QUALITY_4 4
#define ID_PLAY 5
#define ID_PAUSE 6
#define ID_VIEW_TIMEINTERVAL 7
#define ID_VIEW_GRAPHCONFIGURATION 8

#define NODE_SIZE 30

DEFINE_EVENT_TYPE (VivaGraphChanged)
DEFINE_EVENT_TYPE (VivaGraphLayoutUpdated)

GraphFrame::GraphFrame (wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  Create (parent, id, pos, size, style, name);
}

bool GraphFrame::Create(wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  if (!BasicFrame::Create(parent, id, pos, size, style, name) )
    return false;


  std::cout << "FRAM " << size.GetWidth() << " " << size.GetHeight() << std::endl;

  vivagraph = NULL;

  this->Connect(wxEVT_PAINT,
                wxPaintEventHandler(GraphFrame::OnPaint));
  this->Connect(VivaGraphChanged,
                wxCommandEventHandler(GraphFrame::OnVivaGraphChanged));
  this->Connect(VivaGraphLayoutUpdated,
                wxCommandEventHandler(GraphFrame::OnVivaGraphLayoutUpdated));

  // this->Connect(wxEVT_MOTION,
  //               wxMouseEventHandler(GraphFrame::OnSearchNode));
  this->Connect(wxEVT_LEFT_DOWN,
                wxMouseEventHandler(GraphFrame::leftMouseClicked));
  this->Connect(wxEVT_RIGHT_DOWN,
                wxMouseEventHandler(GraphFrame::rightMouseClicked));
  // this->Connect(wxEVT_LEFT_UP,
  //               wxMouseEventHandler(GraphFrame::OnLeftUp));

  translate = wxRealPoint(400.0,300.0);

  return true;
}

void GraphFrame::setVivaGraph (VivaGraph *vivagraph)
{
  this->vivagraph = vivagraph;
  vivagraph->setView (this);
}

void GraphFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxColour black, gray, white;
  black.Set(wxT("#000000"));
  gray.Set(wxT("#c0c0c0"));
  white.Set(wxT("#ffffff"));

  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  if (!vivagraph) return;

  std::vector<VivaNode*>::iterator it;
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    wxPoint position = (*it)->position();

    dc.SetPen(wxPen(black));
    dc.DrawRectangle(position.x-NODE_SIZE/2,
                     position.y-NODE_SIZE/2,
                     NODE_SIZE,
                     NODE_SIZE);
    dc.DrawText (wxString((*it)->node->name, wxConvUTF8), position.x+15, position.y+15);
  }
}

void GraphFrame::leftMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  //transform the mouse position in the screen space to the logical space
  wxPoint mouseLogical;
  mouseLogical.x = dc.DeviceToLogicalX (screen.x);
  mouseLogical.y = dc.DeviceToLogicalY (screen.y);

  vivagraph->leftMouseClicked (mouseLogical);
}

void GraphFrame::rightMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  //transform the mouse position in the screen space to the logical space
  wxPoint mouseLogical;
  mouseLogical.x = dc.DeviceToLogicalX (screen.x);
  mouseLogical.y = dc.DeviceToLogicalY (screen.y);

  vivagraph->rightMouseClicked (mouseLogical);
}

void GraphFrame::OnVivaGraphChanged (wxCommandEvent& event)
{
  Refresh();
}

void GraphFrame::OnVivaGraphLayoutUpdated (wxCommandEvent& event)
{
  Refresh();
}

wxSize GraphFrame::DoGetBestSize ()
{
  std::cout << __FUNCTION__ << std::endl;
  wxSize x;
  return x;
}

GraphWindow::GraphWindow (wxWindow *parent, VivaGraph *vivagraph)
  : wxFrame(parent, wxID_ANY, wxT("Viva Graph Window"))
{
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

  viewmenu = new wxMenu;
  menubar->Append(viewmenu, wxT("&View"));
  viewmenu->AppendCheckItem (ID_VIEW_TIMEINTERVAL, wxT("&Time Interval\tCtrl+T"));
  viewmenu->AppendCheckItem (ID_VIEW_GRAPHCONFIGURATION, wxT("&Graph Configuration\tCtrl+G"));

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
  view = new GraphFrame (panel, -1);
  view->setVivaGraph (vivagraph);
  wxBoxSizer *vbox = new wxBoxSizer (wxVERTICAL);
  vbox->Add (view, 1, wxEXPAND);
  panel->SetSizer(vbox);
  Centre();
}

GraphWindow::~GraphWindow ()
{
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
