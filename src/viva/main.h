/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
#include <libconfig.h>
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
  wxString configuration;

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
  { wxCMD_LINE_SWITCH, _T("h"), _T("help"), _T("Display help"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
  { wxCMD_LINE_OPTION, _("g"), _("graph"), _("Provide a graph configuration file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY},
  { wxCMD_LINE_PARAM, NULL, NULL, _("<paje_trace_file>"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
  { wxCMD_LINE_NONE }
};

DECLARE_APP(TrivaWXApp)
