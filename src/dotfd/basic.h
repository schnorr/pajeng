#ifndef __BASIC_FRAME_H
#define __BASIC_FRAME_H
#include <wx/wx.h>

class BasicFrame : public wxFrame
{
public:
  BasicFrame (const wxString& title);


  double ratio;
  wxRealPoint translate;
  wxPoint mousePosition;
private:
  void OnMouseMotion (wxMouseEvent& event);
  void OnMouseWheel (wxMouseEvent& event);
};

#endif
