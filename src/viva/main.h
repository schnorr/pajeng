#include <wx/wxprec.h>
#include <wx/cmdline.h>
#include <wx/event.h>
#include <wx/progdlg.h>
#include "loadingbar.h"
#include "graphwindow.h"
#include "timeslicewindow.h"
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"
#include "TimeInterval.h"
#include "VivaGraph.h"

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

class TrivaWXApp : public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
  virtual int OnRun();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

private:
  wxString filename;
  PajeFileReader *reader;
  PajeEventDecoder *decoder;
  PajeSimulator *simulator;
  TimeInterval *timeinterval;
  VivaGraph *vivagraph;
  GraphWindow *window;
  TimeSliceWindow *timeslicewindow;

  void connectComponents (PajeComponent *c1, PajeComponent *c2);
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_PARAM,
       NULL,
       NULL,
       wxT("<paje_trace_file>"),
       wxCMD_LINE_VAL_STRING,
       wxCMD_LINE_OPTION_MANDATORY
     },
     { wxCMD_LINE_NONE }
};

DECLARE_APP(TrivaWXApp)
