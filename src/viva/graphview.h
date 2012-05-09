#ifndef __GRAPH_WINDOW_H
#define __GRAPH_WINDOW_H
#include <wx/artprov.h>
#include "basic.h"
#include "timeinterval.h"
#include "VivaGraph.h"

class VivaGraph;
class TimeIntervalView;

DECLARE_EVENT_TYPE (VivaGraphChanged, -1)
DECLARE_EVENT_TYPE (VivaGraphLayoutUpdated, -1)

class GraphFrame : public BasicFrame
{
public:
  GraphFrame () {;};
  GraphFrame (wxWindow *parent,
              wxWindowID id,
              const wxPoint & pos = wxDefaultPosition,
              const wxSize & size = wxDefaultSize,
              long style = wxDEFAULT_FRAME_STYLE,
              const wxString & name = _T("GraphFrame"));
  ~GraphFrame () {;};
  bool Create(wxWindow* parent,
              wxWindowID id,
              const wxPoint & pos = wxDefaultPosition,
              const wxSize & size = wxDefaultSize,
              long style = wxDEFAULT_FRAME_STYLE,
              const wxString & name = _T("GraphFrame"));
  void setVivaGraph (VivaGraph *vivagraph);

private:
  VivaGraph *vivagraph;

  void OnPaint (wxPaintEvent& event);

  //user interactions
  void leftMouseClicked (wxMouseEvent& event);
  void rightMouseClicked (wxMouseEvent& event);

  //from VivaGraph, and runner
  void OnVivaGraphChanged (wxCommandEvent& event);
  void OnVivaGraphLayoutUpdated (wxCommandEvent& event);

protected:
  wxSize DoGetBestSize ();

};

class GraphWindow : public wxFrame
{
private:
  VivaGraph *vivagraph;

  wxMenuBar *menubar;
  wxMenu *file;
  wxMenu *edit;
  wxMenu *viewmenu;
  wxMenu *help;

  GraphFrame *view;

protected:
  void OnTimeIntervalMenu (wxCommandEvent& event);
  void OnGraphConfigurationMenu (wxCommandEvent& event);
  void OnUpdateQuality (wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnAbout (wxCommandEvent& event);

public:
  GraphWindow (wxWindow *parent, VivaGraph *vivagraph);
  ~GraphWindow (void);
};

#endif
