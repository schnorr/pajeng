#include "tupiew.h"
#include <wx/artprov.h>

DEFINE_EVENT_TYPE (TupiLayoutUpdated)

#define NODE_SIZE 30

extern tp_dynar *nodes;

#define ID_QUALITY_0 0
#define ID_QUALITY_1 1
#define ID_QUALITY_2 2
#define ID_QUALITY_3 3
#define ID_QUALITY_4 4
#define ID_PLAY 5
#define ID_PAUSE 6

Tupiew::Tupiew(const wxString& title)
  : BasicFrame(title)
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

  view = new wxMenu;
  menubar->Append(view, wxT("&View"));

  help = new wxMenu;
  help->Append(wxID_ABOUT, wxT("&About\tCtrl+A"));
  menubar->Append(help, wxT("&Help"));

  SetMenuBar(menubar);

  Connect(ID_QUALITY_0, ID_QUALITY_4, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(Tupiew::OnQuality));

  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
      wxCommandEventHandler(Tupiew::OnQuit));

  Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
      wxCommandEventHandler(Tupiew::OnOpen));

  Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(Tupiew::OnAbout));

  this->Connect(wxEVT_PAINT, wxPaintEventHandler(Tupiew::OnPaint));
  this->Connect(wxEVT_MOTION, wxMouseEventHandler(Tupiew::OnSearchNode));
  this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Tupiew::OnLeftDown));
  this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Tupiew::OnLeftUp));
  this->Connect(TupiLayoutUpdated, wxCommandEventHandler(Tupiew::OnLayoutUpdated));

  Connect (wxEVT_ACTIVATE, wxActivateEventHandler(Tupiew::OnActivate));

  //status bar
  CreateStatusBar(3);


  wxIcon play(wxT("play.png"));

  //tool bar
  wxToolBar *toolbar = CreateToolBar();
  toolbar->AddTool(ID_PLAY, wxT("Play"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-play"), wxART_TOOLBAR));
  toolbar->AddTool(ID_PAUSE, wxT("Pause"),
                   wxArtProvider::GetBitmap(wxT("gtk-media-pause"), wxART_TOOLBAR));
  toolbar->Realize();

  Connect(ID_PLAY, ID_PAUSE, wxEVT_COMMAND_TOOL_CLICKED,
      wxCommandEventHandler(Tupiew::OnThreadManagement));

  Centre();
}

void Tupiew::start_runner (void)
{
  if (runner && !runner->IsAlive()){
    runner = NULL;
  }
  if (runner != NULL){
    return;
  }
  runner = new LayoutRunner (this, layout);
  runner->Create();
  runner->Run();
}

void Tupiew::stop_runner (void)
{
  runner->keepRunning = false;
  runner->Wait();
  runner = NULL;  
}

void Tupiew::OnActivate(wxActivateEvent& event)
{
  static int flag = 1;
  if (!runner && flag){
    start_runner();
    flag = 0;
  }
}

void Tupiew::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxColour black, gray, white;
  black.Set(wxT("#000000"));
  gray.Set(wxT("#c0c0c0"));
  white.Set(wxT("#ffffff"));

  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  int i;
  for (i = 0; i < dynar_count (nodes); i++){
    tp_node *node = *(tp_node**)dynar_get_ptr (nodes, i);
    tp_point pos = node->particle->position;

    int j;
    for (j = 0; j < dynar_count (node->connected); j++){
      tp_node *c = *(tp_node**)dynar_get_ptr(node->connected, j);
      tp_point pc = c->particle->position;
      dc.SetPen(wxPen(gray));
      dc.DrawLine (pos.x*100, pos.y*100, pc.x*100, pc.y*100);
    }
  }
  for (i = 0; i < dynar_count (nodes); i++){
    tp_node *node = *(tp_node**)dynar_get_ptr (nodes, i);
    tp_point pos = node->particle->position;

    //dc.FloodFill (pos.x*100, pos.y*100, white);
    dc.SetPen(wxPen(black));
    dc.DrawRectangle(pos.x*100-NODE_SIZE/2,
                     pos.y*100-NODE_SIZE/2,
                     NODE_SIZE,
                     NODE_SIZE);
    // dc.DrawText (wxString(node->name, wxConvUTF8), pos.x*100+15, pos.y*100+15);
  }
}

void Tupiew::OnLeftUp (wxMouseEvent& event)
{
  if (selected){
    selected->particle->frozen = 0;
  }
  selected = NULL;
}

void Tupiew::OnLeftDown (wxMouseEvent& event)
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

  tp_rect mask;
  mask.size.width = (double)NODE_SIZE/100;
  mask.size.height = (double)NODE_SIZE/100;

  tp_point point;
  point.x = (double)mouseLogical.x/100;
  point.y = (double)mouseLogical.y/100;

  selected = layout_find_node_by_position (layout, point);
}

void Tupiew::OnSearchNode (wxMouseEvent& event)
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

  if (selected){
    tp_point newpos;
    newpos.x = (double)mouseLogical.x/100;
    newpos.y = (double)mouseLogical.y/100;
    layout_move_node (layout, selected, newpos);
    start_runner();
    Refresh();
  }else{
    event.Skip();
  }
}

void Tupiew::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void Tupiew::OnOpen(wxCommandEvent& WXUNUSED(event))
{
  std::cout << __FUNCTION__ << std::endl;
}

void Tupiew::OnQuality(wxCommandEvent& event)
{
  int event_id = event.GetId();
  if (event_id >= ID_QUALITY_0 &&
      event_id <= ID_QUALITY_4){
    layout_set_quality (layout, event_id);
    wxString str = wxT("Quality: ");
    str << event_id;
    SetStatusText(str, 2);
    layout_reset_energies(layout);
    start_runner();
  }
}

void Tupiew::OnThreadManagement(wxCommandEvent& event)
{
  switch (event.GetId()){
  case ID_PLAY: 
  {
    if (!runner){
      start_runner();
    }
  }
  break;
  case ID_PAUSE:
  {
    if (runner){
      stop_runner();
    }
  }
  break;
  default: break;
  }
}

void Tupiew::OnClose (wxCloseEvent& WXUNUSED(event))
{
  std::cout << __FUNCTION__ << std::endl;
  if (runner){
    runner->keepRunning = false;
    runner->Wait();
  }
  std::cout << " terminando finalmente " << std::endl;
}


void Tupiew::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  std::cout << __FUNCTION__ << std::endl;
  wxMessageBox( _("This is Tupi, a force-directed graph viewer.\nReleased under the GPLv3 license."),
                _("About Tupi (wxwidgets)"),
                wxOK | wxICON_INFORMATION, this );
}

void Tupiew::OnLayoutUpdated (wxCommandEvent& event)
{
  static double last = 0;
  double current = gettime();
  double dif = current - last;
  if (dif > 0.01){
    Refresh();
    last = current;
  }
}
