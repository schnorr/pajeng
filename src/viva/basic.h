#ifndef __BASIC_FRAME_H
#define __BASIC_FRAME_H
#include <wx/wx.h>

class BasicFrame : public wxPanel
{
  DECLARE_DYNAMIC_CLASS(BasicFrame);

public:
  BasicFrame () {;};
  BasicFrame (wxWindow* parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style,
              const wxString& name);
  ~BasicFrame () {;};
  bool Create(wxWindow* parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style,
              const wxString& name);

protected:
  double ratio;
  wxRealPoint translate;
  wxPoint mousePosition;

private:
  void OnMouseMotion (wxMouseEvent& event);
  void OnMouseWheel (wxMouseEvent& event);
  void OnSize (wxSizeEvent& event);
};

#endif
