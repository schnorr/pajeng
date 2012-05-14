#include "PajeSimulator.h"

void PajeSimulator::setSelectionStartEndTime (double start, double end)
{
  selectionStart = start;
  selectionEnd = end;

  timeSelectionChanged ();
}
