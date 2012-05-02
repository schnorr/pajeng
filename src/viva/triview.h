#ifndef __TRIVIEW_H
#define __TRIVIEW_H
#include "basic.h"
#include "runner.h"
#include "tupi_private.h"

/* extern tp_layout *layout; */

DECLARE_EVENT_TYPE (TupiLayoutUpdated, -1)

class Triview : public BasicFrame
{
public:
  Triview(const wxString& title);

  void start_runner (void);
  void stop_runner (void);

  void OnActivate(wxActivateEvent& event);
  void OnPaint(wxPaintEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnClose (wxCloseEvent& event);
  void OnAbout (wxCommandEvent& event);
  void OnQuality (wxCommandEvent& event);
  void OnThreadManagement (wxCommandEvent& event);

  void OnLayoutUpdated (wxCommandEvent& event);
  void OnSearchNode (wxMouseEvent& event);
  void OnLeftDown (wxMouseEvent& event);
  void OnLeftUp (wxMouseEvent& event);


  wxMenuBar *menubar;
  wxMenu *file;
  wxMenu *edit;
  wxMenu *view;
  wxMenu *help;

  /* tp_node *selected; */

private:
  LayoutRunner *runner;
};

#endif
