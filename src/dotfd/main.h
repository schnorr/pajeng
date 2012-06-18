#include <wx/wxprec.h>
#include <wx/cmdline.h>
#include <graphviz/types.h>
#include <graphviz/graph.h>
#include "tupi.h"
#include "tupiew.h"
#include "runner.h"

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

void *layout;

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
  virtual int OnRun();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

private:
  wxString filename;
  Agraph_t *g;
protected:
  Tupiew *tupiew;
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_PARAM,
       NULL,
       NULL,
       wxT("<graphviz_dot_file>"),
       wxCMD_LINE_VAL_STRING,
       wxCMD_LINE_OPTION_MANDATORY
     },
     { wxCMD_LINE_NONE }
};

DECLARE_APP(MyApp)
