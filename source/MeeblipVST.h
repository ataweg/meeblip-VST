// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstqui4
//    21.08.2013  AWe   add support for midi in/out
//    19.08.2013  AWe   distinguish gui and non-gui parameters and controls
//    01.08.2013  AWe   add program support
//    30.07.2013  AWe   code formating
//    22.07.2013  AWe   use tutorial example from VST SDK 3.0 as base for
//                      the MeeblipVST plugin
//
// --------------------------------------------------------------------------

#ifndef __MeeblipVST__
#define __MeeblipVST__

#include "MeeblipVST_Layout.h"

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "aweVSTtypes.h"

#include <algorithm>
#include <vector>

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

template <typename FloatType>
inline int roundToInt(const FloatType x)
{
   return x>=0 ? static_cast<int>(x + 0.5) : static_cast<int>(x - 0.5);
}

#define MIDI_TO_FLOAT(i)            ( (float)(i)     * (1.0f/127))
#define FLOAT_TO_MIDI(i)            ( roundToInt((i) * 127.0f))
#define FLOAT_TO_CHANNEL015(i)      ( roundToInt((i) * 15.0f))

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

enum
{
   // Global
   kNumPrograms = 128,
   kNumOutputs = 2
};

// --------------------------------------------------------------------------
// MeeblipVSTProgram
// --------------------------------------------------------------------------

class MeeblipVSTProgram
{
friend class MeeblipVST;
public:
   MeeblipVSTProgram();
   ~MeeblipVSTProgram() {}

private:
   float parameters[ kNumGuiParameters];
   float fMidiInChannel;
   float fMidiOutChannel;

   char name[kVstMaxProgNameLen+1];
};

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define PLUG_NAME          "midiMeeblipVST"
#define PLUG_MIDI_INPUTS   1
#define PLUG_MIDI_OUTPUTS  1

typedef std::vector<VstMidiEvent> VstMidiEventVec;
typedef std::vector<VstMidiSysexEvent> VstSysexEventVec;

#define MAX_EVENTS_PER_TIMESLICE 256

struct MyVstEvents
{
    VstInt32 numEvents;
    VstIntPtr reserved;
    VstEvent* events[MAX_EVENTS_PER_TIMESLICE];
};

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

class MeeblipVST : public AudioEffectX
{
public:
   MeeblipVST( audioMasterCallback audioMaster);
   ~MeeblipVST();

   // Processing
   virtual void processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames);
   virtual void processDoubleReplacing( double** inputs, double** outputs, VstInt32 sampleFrames);
   virtual VstInt32 processEvents( VstEvents* events);      // Called when new MIDI events come in

   virtual void preProcess();
   virtual void postProcess();

   // Program
   virtual void setProgram( VstInt32 program);
   virtual void setProgramName( char* name);
   virtual void getProgramName( char* name);
   virtual bool getProgramNameIndexed( VstInt32 category, VstInt32 index, char* text);

   // Parameters
   virtual void setParameter( VstInt32 index, float value);
   virtual float getParameter( VstInt32 index);
   virtual void getParameterDisplay( VstInt32 index, char* text);
   virtual void getParameterName( VstInt32 index, char* text);

   virtual bool getInputProperties (VstInt32 index, VstPinProperties* properties);
   virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);

   virtual bool getEffectName( char* name);
   virtual bool getVendorString( char* text);
   virtual bool getProductString( char* text);
   virtual VstInt32 getVendorVersion();
   virtual VstInt32 canDo( char* text);

private:
   MeeblipVSTProgram* programs;

protected:
   float parameters[ kNumGuiParameters];
   char programName[ kVstMaxProgNameLen + 1];

// --------------------------------------------------------------------------
// midi support
// --------------------------------------------------------------------------

public:
   tresult sendMidiCC( ParamID paramId, int32 midiValue);

protected:
   bool midiEnable;
   float fMidiInChannel;
   float fMidiOutChannel;

// ------------------------------------
//
// ------------------------------------

public:
   virtual VstInt32 getNumMidiInputChannels ()
   {
#if PLUG_MIDI_INPUTS
      return 16;
#else
      return 0;
#endif
   }

   virtual VstInt32 getNumMidiOutputChannels ()
   {
#if PLUG_MIDI_OUTPUTS
      return 16;
#else
      return 0;
#endif
   }


protected:
   bool init();
   virtual void processMidiEvents(VstMidiEventVec *inputs, VstMidiEventVec *outputs, VstInt32 sampleFrames);
   virtual void processMidiSysexEvents( VstSysexEventVec *inputs, VstSysexEventVec *outputs, VstInt32 sampleFrames);

   void copySysex();

   VstMidiEventVec *_midiEventsIn;
   VstSysexEventVec *_midiSysexEventsIn;
   void _cleanMidiInBuffers();

   VstMidiEventVec *_midiEventsOut;
   VstSysexEventVec *_midiSysexEventsOut;
   void _cleanMidiOutBuffers();

   MyVstEvents  *_vstEventsToHost;
   VstMidiEvent *_vstMidiEventsToHost;
   VstMidiSysexEvent *_vstSysexEventsToHost;

   int numinputs, numoutputs, bottomOctave;

   static bool midiSort(VstMidiEvent first, VstMidiEvent second)
   {
      return first.deltaFrames < second.deltaFrames;
   }

   static void sortMidiEvents(VstMidiEventVec _vec)
   {
      std::stable_sort( _vec.begin(), _vec.end(), midiSort );
   }

};

#endif // __MeeblipVST__