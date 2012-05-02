#include <wx/wxprec.h>
#include <wx/cmdline.h>
//#include <graphviz/types.h>
//#include <graphviz/graph.h>
//#include "tupi_private.h"
#include "triview.h"
#include "runner.h"

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

/* tp_layout *layout; */

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
  /* Agraph_t *g; */
protected:
  Triview *triview;
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
