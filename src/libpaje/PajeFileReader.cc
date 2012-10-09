/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeFileReader.h"
#include <sys/time.h>

double gettime ()
{
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}

PajeFileReader::PajeFileReader (std::string f)
{
  chunkSize = PAJE_DEFAULT_CHUNK_SIZE;
  moreData = true;
  currentChunk = 0;
  filename = f;
  file.open (filename.c_str());
  if (file.fail()){
    throw "Error loading file: "+f;
  }
  file.seekg (0, std::ios::end);
  length = file.tellg ();
  file.seekg (0, std::ios::beg);

  input = &file;
}

PajeFileReader::PajeFileReader ()
{
  chunkSize = PAJE_DEFAULT_CHUNK_SIZE;
  moreData = true;
  currentChunk = 0;
  filename = std::string ("stdin");

  input = &std::cin;
}

PajeFileReader::~PajeFileReader (void)
{
  file.close();
}

void PajeFileReader::startChunk (int chunkNumber)
{
  if (chunkNumber != currentChunk){
    if (chunkNumber >= chunkInfo.size()){
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    std::streampos position = chunkInfo.find (chunkNumber)->second;
    input->seekg (position);
    currentChunk = chunkNumber;
    moreData = true;
  }else{
    if (chunkInfo.size() == 0){
      std::streampos position = input->tellg();
      chunkInfo.insert (std::pair<int,std::streampos>(chunkNumber, position));
    }
  }
  PajeComponent::startChunk (chunkNumber);
}

void PajeFileReader::endOfChunkLast (bool last)
{
  currentChunk++;
  if (!last){
    if (currentChunk == chunkInfo.size()){
      std::streampos position = input->tellg();
      chunkInfo.insert (std::pair<int,std::streampos>(currentChunk,position));
    }
  }
  PajeComponent::endOfChunkLast (last);
}

bool PajeFileReader::canEndChunk (void)
{
  if (!this->hasMoreData()){
    //file reached end, so we can end current chunk
    return true;
  }

  std::streamoff offsetInFile = input->tellg();
  PajeData *buffer = new PajeData(chunkSize);
  input->read (buffer->bytes, chunkSize);
  if (input->eof()){
    // file.close();
    // file.open (filename.c_str());
  }
  std::streamoff length = input->gcount ();
  buffer->length = length;
  std::streamoff line_size = std::streamoff(chunkSize);
  if (length < line_size){
    moreData = false;
  }

  //align data based on EOL
  char *bytes = buffer->bytes;
  char *eol;
  eol = (char*)memchr (bytes, '\n', length);
  if (eol != NULL){
    std::streamoff newlength = std::streamoff (eol - bytes + 1);
    if (newlength != length){
      length = newlength;
      moreData = true;
      input->seekg (offsetInFile + length);
      buffer->bytes[length] = '\0';
      buffer->length = length;
    }
  }

  bool ret = true;
  if (length > 0){
    if (PajeComponent::canEndChunkBefore(buffer)){
      input->seekg(offsetInFile);
      ret = true;
    }else{
      input->seekg(offsetInFile+length);
      ret = false;
    }
  }else{
    ret = true;
  }
  delete buffer;
  return ret;
}

void PajeFileReader::readNextChunk (void)
{
  if (!moreData) return;

  int nextChunk = currentChunk + 1;
  if (nextChunk < chunkInfo.size()){
    // this chunk has already been read, we should know its size
    std::streampos nextChunkPosition = chunkInfo.find (nextChunk)->second;
    std::streampos chunkSize = nextChunkPosition - input->tellg();
    PajeData *buffer = new PajeData (chunkSize);
    input->read (buffer->bytes, chunkSize);
    current = input->tellg();
    std::streamoff length = input->gcount();
    buffer->length = length;
    if (length != chunkSize){
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }
    PajeComponent::outputEntity (buffer);
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }else{
    // first time reading this chunk.
    // must determine its correct size (must end in a line boundary
    // and in a date-changing event).
    // need to create a NSMutableData from a NSData
    PajeData *buffer = new PajeData(chunkSize);
    input->read (buffer->bytes, chunkSize);
    current = input->tellg();
    std::streamoff length = input->gcount ();
    buffer->length = length;
    if (length < chunkSize){
      moreData = false;
    }else{
      char *bytes = buffer->bytes;
      int i;
      int offset = 0;
      for (i = length-1; i >= 0 && bytes[i] != '\n'; i--) {
        offset++;
      }
      if ((i >= 0) && (offset > 0)) {
        input->seekg (input->tellg() - std::streampos(offset));
        length = length - std::streampos(offset);
        buffer->bytes[length] = '\0';
        buffer->length = length;
      }
    }

    if (length > 0){
      PajeComponent::outputEntity (buffer);
    }
    delete buffer;
    while (!this->canEndChunk());
  }
}

bool PajeFileReader::hasMoreData (void)
{
  return moreData;
}

void PajeFileReader::setUserChunkSize (std::streamoff userChunkSize)
{
  chunkSize = userChunkSize;
}

unsigned long long PajeFileReader::traceSize (void)
{
  return length;
}

unsigned long long PajeFileReader::traceRead (void)
{
  if (!input->eof())
    return current;
  else
    return length;
}
