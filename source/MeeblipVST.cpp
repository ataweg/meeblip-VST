// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST.cpp
//
// --------------------------------------------------------------------------
// Changelog
//
//    29.01.2014  AWe   set initial values for gui elements from layout structure
//    23.01.2014  AWe   in getParameterDisplay() correct calculation of stepcount interval
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstgui4
//    22.08.2013  AWe   extend method canDo()
//    21.08.2013  AWe   add support for midi in/out
//    19.08.2013  AWe   distinguish gui and non-gui parameters and controls
//    14.08.2013  AWe   restructure process method, add checkSilence(9 method
//    01.08.2013  AWe   add program support
//    08.08.2013  AWe   implement setState(), getState()
//    30.07.2013  AWe   code formating
//    27.07.2013  AWe   remove parameter processing
//    22.07.2013  AWe   use tutorial example from the VST SDK 3.0 as base for
//                      the MeeblipVST plugin
//
// References
//    ...\SDK_3.0\vstgui4\vstgui\tutorial\source\Tutorial2Editor.cpp
//    ...\vstsdk2.4\public.sdk\samples\vst2.x\vstxsynth\source\vstxsnth.cpp
// --------------------------------------------------------------------------

#include "MeeblipVST.h"
#include "MeeblipVST_Layout.h"
#include "aweVSTtypes.h"

#include "vstgui/plugin-bindings/aeffguieditor.h"

#define MIDI_CONTROLCHANGE   0xB0

// --------------------------------------------------------------------------
// Debug support
// --------------------------------------------------------------------------

#define VERBOSITY       99
#define VERBOSITY_MIN   1

#include "aweDBG.h"

#if defined(WIN32) && defined( _DEBUG) && !defined(  VST3_PLUGIN)
#include <windows.h>
#include <stdio.h>

long stupid = AllocConsole();
long rc = printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" );
long rd = printf( "------------------------------------------------\n" );
long re = printf( "Meeblip v0.1 VST2.x( DLL) %s %s\n\n", __DATE__, __TIME__  );
#endif

// --------------------------------------------------------------------------
// MeeblipVSTProgram
// --------------------------------------------------------------------------

MeeblipVSTProgram::MeeblipVSTProgram()
{
   // Default Program Values

   vst_strncpy( name, "-init-", kVstMaxProgNameLen);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

AudioEffect* createEffectInstance( audioMasterCallback audioMaster)
{
   DBG( 1, "\nAudioEffect createEffectInstance" );

   return new MeeblipVST( audioMaster);
}

// --------------------------------------------------------------------------
//  MeeblipVST Implementation
// --------------------------------------------------------------------------

MeeblipVST::MeeblipVST( audioMasterCallback audioMaster)
   : AudioEffectX( audioMaster, kNumPrograms, kNumGuiParameters + kNumExtraParameters)
{
   DBG( 1, "\nMeeblipVST::MeeblipVST" );

   // initialize programs
   programs = new MeeblipVSTProgram[kNumPrograms];
   if( programs)
      setProgram( 0);

   // initialize parameters
   MeeblipVSTProgram *ap = &programs[ curProgram];
   DBG( 2, "      curProgram %d", curProgram);

   for( VstInt32 i = 0; i < kNumGuiParameters; i++)
   {
      float value = KnobValue2float( getLayoutItem( i)->defaultValue, i);

      parameters[i] = value;
      ap->parameters[i] = value;
      DBG( 2, "      set parameter %d %g %d", i, value, getLayoutItem( i)->defaultValue );
   }

   fMidiInChannel  = 0.0f;
   fMidiOutChannel = 0.0f;

   if( audioMaster)
   {
      setNumInputs( 2);
      setNumOutputs( 2);

      canProcessReplacing();  // supports replacing output
      canDoubleReplacing ();  // supports double precision processing

      isSynth();
      setUniqueID( CCONST('a', 'w', 'M', 'b'));// Axel's Meeblip
   }

   extern AEffGUIEditor* createEditor( AudioEffectX*);
   setEditor( createEditor( this));

   //   initProcess();  // initialize the synthesizer
   suspend();
   // init midi buffers
   init();

}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

MeeblipVST::~MeeblipVST()
{
   DBG( 1, "\nMeeblipVST::~MeeblipVST" );

   if( programs)
      delete[] programs;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::setProgram( VstInt32 program)
{
   DBG( 1, "\nMeeblipVST::setProgram %d", program );

   if( program < 0 || program >= kNumPrograms)
      return;

   MeeblipVSTProgram *ap = &programs[program];
   curProgram = program;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::setProgramName( char* name)
{
   DBG( 1, "\nMeeblipVST::setProgramName %s", name );

   vst_strncpy( programs[ curProgram].name, name, kVstMaxProgNameLen);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::getProgramName( char* name)
{
   DBG( 1, "\nMeeblipVST::getProgramName" );

   vst_strncpy( name, programs[curProgram].name, kVstMaxProgNameLen);

   DBG( 2, "      %s", name);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::getParameterDisplay( VstInt32 index, char* text)
{
   DBG( 1, "\nMeeblipVST::getParameterDisplay %d", index );

   if( index < kNumGuiParameters)
   {
      VstInt32 stepCount = getLayoutItem( index)->stepCount;
      float value = parameters[ index];

      if( stepCount == 1)
      {
         if( value < 0.5f )
            vst_strncpy( text, "Off", kVstMaxParamStrLen);
         else
            vst_strncpy( text, "On", kVstMaxParamStrLen);
      }
      else
      {
         VstInt32 intVal = float2KnobValue( value, index);

         // there is a bug in the int2string function:
         //    if intVal is zero, the returning string is empty
         if( intVal)
            int2string( intVal, text, kVstMaxParamStrLen);
         else
            vst_strncpy( text, "0", kVstMaxParamStrLen);
      }
      DBG( 2, "     %g --> %s step %d", value, text, stepCount );
   }
   else if( index < kNumGuiParameters + kNumExtraParameters)
   {
      float value;
      switch( index)
      {
         case kMidiInChannel:  value = fMidiInChannel;  break;
         case kMidiOutChannel: value = fMidiOutChannel; break;
      }
      int32 intVal = (int32)( value * 15 + 0.5f);
      int2string( intVal+1, text, kVstMaxParamStrLen);
      DBG( 2, "     %g --> %s step %d", value, text, 15 );
   }
   else
   {
      DBG( 2, "     not a valid index" );
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::getParameterName( VstInt32 index, char* label)
{
   DBG( 1, "\nMeeblipVST::getParameterName %d", index );

   if( index < kNumGuiParameters)
   {
      const char* parameterName = getLayoutItem( index)->parameterName;
      VstInt32 parameterNameLen = strlen( parameterName);
      VstInt32 len = parameterNameLen > kVstMaxParamStrLen ? kVstMaxParamStrLen : parameterNameLen;

      vst_strncpy( label, parameterName, parameterNameLen);
   }
   else if( index < kNumGuiParameters + kNumExtraParameters)
   {
      switch( index)
      {
         case kMidiInChannel:   vst_strncpy( label, "Midi In",  kVstMaxParamStrLen);   break;
         case kMidiOutChannel:  vst_strncpy( label, "Midi Out", kVstMaxParamStrLen);   break;
      }
   }

   DBG( 2, "      label: %s", label);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::setParameter( VstInt32 index, float value)
{
   DBG( 1, "\nMeeblipVST::setParameter %d %g", index, value );

   if( index < kNumGuiParameters)
   {
      MeeblipVSTProgram *ap = &programs[ curProgram];
      DBG( 0, "     %08x %d %d %d", (int)ap, curProgram, index, ap->parameters[index]);

      parameters[index] = value;
      ap->parameters[index] = value;

      if( midiEnable )
      {
         sendMidiCC( index, FLOAT_TO_MIDI( value) );
      }

      ( (AEffGUIEditor*)editor)->setParameter( index, value);
   }
   else if( index < kNumGuiParameters + kNumExtraParameters)
   {
      switch( index)
      {
         case kMidiInChannel:   fMidiInChannel  = value; break;
         case kMidiOutChannel:  fMidiOutChannel = value; break;
      }
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

float MeeblipVST::getParameter( VstInt32 index)
{
   DBG( 1, "\nMeeblipVST::getParameter %d", index );

   if( index < kNumGuiParameters)
   {
      DBG( 1, " %g", parameters[index] );
      return parameters[index];
   }
   else if( index < kNumGuiParameters + kNumExtraParameters)
   {
      float value;
      switch( index)
      {
         case kMidiInChannel:   value = fMidiInChannel;  break;
         case kMidiOutChannel:  value = fMidiOutChannel; break;
      }
      DBG( 1, " %g", value );
      return value;
   }

   DBG( 1, " invalid" );
   return 0.f;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

bool MeeblipVST::getProgramNameIndexed( VstInt32 category, VstInt32 index, char* text)
{
   DBG( 1, "\nMeeblipVST::getProgramNameIndexed %d %d", category, index );

   if( index < kNumPrograms)
   {
      vst_strncpy( text, programs[index].name, kVstMaxProgNameLen);
      DBG( 2, "      %s", text );
      return true;
   }
   return false;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

bool MeeblipVST::getEffectName( char* name)
{
   DBG( 1, "\nMeeblipVST::getEffectName" );

   vst_strncpy( name, "MeeblipVST", kVstMaxEffectNameLen);

   DBG( 2, "      %s:", name);
   return true;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

bool MeeblipVST::getVendorString( char* text)
{
   DBG( 1, "\nMeeblipVST::getVendorString" );

   vst_strncpy( text, "Axel Werner electronics( AWe)", kVstMaxVendorStrLen);

   DBG( 2, "      %s:", text);
   return true;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

bool MeeblipVST::getProductString( char* text)
{
   DBG( 1, "\nMeeblipVST::getProductString" );

   vst_strncpy( text, "Meeblip VST 2.x", kVstMaxProductStrLen);

   DBG( 2, "      %s:", text);
   return true;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

VstInt32 MeeblipVST::getVendorVersion()
{
   DBG( 1, "\nMeeblipVST::getVendorVersion" );

   return 1000;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

VstInt32 MeeblipVST::canDo( char* text)
{
   DBG( 1, "\nMeeblipVST::canDo" );

   int result = -1; // explicitly can't do; 0 => don't know

   if( !strcmp( text, "receiveVstEvents"))    result = 1;
   if( !strcmp( text, "sendVstEvents"))       result = 1;

   if(PLUG_MIDI_OUTPUTS)
   {
      if( !strcmp( text, "sendVstMidiEvent")) result = 1;
   }

   if(PLUG_MIDI_INPUTS)
   {
      if( !strcmp( text, "receiveVstMidiEvent")) result = 1;
   }

   // if( !strcmp( text, "receiveVstTimeInfo")) result = 1; // VstTimeInfo
   // if( !strcmp( text, "midiProgramNames"))   result = 1;

   DBG( 2, "      %s => %ssupported", text, result == 1 ? "" : "not ");

   return result;
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

bool MeeblipVST::init()
{
   DBG( 1, "\nMeeblipVST::init" );

   try
   {
      _vstEventsToHost            = new MyVstEvents;    // queue that holds the midi data for the host
      _vstEventsToHost->numEvents = 0;
      _vstMidiEventsToHost        = new VstMidiEvent[MAX_EVENTS_PER_TIMESLICE];        // array with midi data for the host
      _vstSysexEventsToHost       = new VstMidiSysexEvent[MAX_EVENTS_PER_TIMESLICE];   // dto. for sysex

      _midiEventsIn = new VstMidiEventVec[PLUG_MIDI_INPUTS];
      _midiSysexEventsIn = new VstSysexEventVec[PLUG_MIDI_INPUTS];
      _cleanMidiInBuffers();

      _midiEventsOut = new VstMidiEventVec[PLUG_MIDI_OUTPUTS];         // stores the midi cc message data
      _midiSysexEventsOut = new VstSysexEventVec[PLUG_MIDI_OUTPUTS];   // dto. for sysex
      _cleanMidiOutBuffers();
   }
   catch( ...)
   {
      return false;
   }

   return true;
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

void MeeblipVST::_cleanMidiInBuffers()
{
   DBG( 0, "\nMeeblipVST::_cleanMidiInBuffers" );

   for( int i = 0; i < PLUG_MIDI_INPUTS; i++ )
   {
      _midiEventsIn[i].clear();
      _midiSysexEventsIn[i].clear();
   }
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

void MeeblipVST::_cleanMidiOutBuffers()
{
   DBG( 0, "\nMeeblipVST::_cleanMidiOutBuffers" );

   for( int i = 0; i < PLUG_MIDI_OUTPUTS; i++ )
   {
      _midiEventsOut[i].clear();
      _midiSysexEventsOut[i].clear();
   }
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

// copy sysex messages from midi input to midi output (pass thru)

void MeeblipVST::copySysex()
{
   DBG( 0, "\nMeeblipVST::copySysex" );

   VstSysexEventVec::iterator it;
   for( it=_midiSysexEventsIn->begin(); it<_midiSysexEventsIn->end(); it++)
   {
      _midiSysexEventsOut->push_back(*it);
   }
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

bool MeeblipVST::getInputProperties( VstInt32 index, VstPinProperties* properties)
{
   DBG( 1, " MeeblipVST::getInputProperties %d", index );

   if( index < numinputs)
   {
      strcpy(properties->label, PLUG_NAME);
      properties->flags |= kVstPinIsActive;
      if( index%2==0) properties->flags |= kVstPinIsStereo;
      return true;
   }
   return false;
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

bool MeeblipVST::getOutputProperties( VstInt32 index, VstPinProperties* properties)
{
   DBG( 1, " MeeblipVST::getOutputProperties %d", index );

   if( index < numoutputs)
   {
      strcpy(properties->label, PLUG_NAME);
      properties->flags |= kVstPinIsActive;
      if( index%2==0) properties->flags |= kVstPinIsStereo;
      return true;
   }
   return false;
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

void MeeblipVST::preProcess(void)
{
   DBG( 0, "\nMeeblipVST::preProcess" );

   ;  // currently nothing to do
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

// Can be called inside processReplacing.
//    bool sendVstEventsToHost( VstEvents* events);            // Send MIDI events back to Host application

void MeeblipVST::postProcess(void)
{
   DBG( 0, "\nMeeblipVST::postProcess" );

   if( PLUG_MIDI_OUTPUTS)
   {
      VstInt32 left = (VstInt32)_midiEventsOut[0].size();
      VstInt32 count = 0;
      while( left > 0)
      {
         if( left<MAX_EVENTS_PER_TIMESLICE)
            _vstEventsToHost->numEvents = left;
         else
            _vstEventsToHost->numEvents = MAX_EVENTS_PER_TIMESLICE;

         for( int i = 0; i < _vstEventsToHost->numEvents; i++)
         {
            VstInt32 j = i+count;
            DBG( 2, "      Send midi out %02x %d %d", (uint8)_midiEventsOut[0][j].midiData[0], (uint8)_midiEventsOut[0][j].midiData[1], (uint8)_midiEventsOut[0][j].midiData[2] );

            _vstMidiEventsToHost[i].type            = kVstMidiType;
            _vstMidiEventsToHost[i].byteSize        = 24;
            _vstMidiEventsToHost[i].deltaFrames     = _midiEventsOut[0][j].deltaFrames;
            _vstMidiEventsToHost[i].flags           = 0;
            _vstMidiEventsToHost[i].noteLength      = 0;
            _vstMidiEventsToHost[i].noteOffset      = 0;
            _vstMidiEventsToHost[i].midiData[0]     = _midiEventsOut[0][j].midiData[0];
            _vstMidiEventsToHost[i].midiData[1]     = _midiEventsOut[0][j].midiData[1];
            _vstMidiEventsToHost[i].midiData[2]     = _midiEventsOut[0][j].midiData[2];
            _vstMidiEventsToHost[i].midiData[3]     = 0;
            _vstMidiEventsToHost[i].detune          = _midiEventsOut[0][j].detune;

            _vstEventsToHost->events[i] = (VstEvent*) &_vstMidiEventsToHost[i];
         }

         _vstEventsToHost->reserved  = 0;
         if( _vstEventsToHost->numEvents > 0)
            sendVstEventsToHost((VstEvents*)_vstEventsToHost);

         left -= _vstEventsToHost->numEvents;
         count += _vstEventsToHost->numEvents;
      }

      left =( VstInt32)_midiSysexEventsOut[0].size();
      count = 0;
      while( left > 0)
      {
         if( left<MAX_EVENTS_PER_TIMESLICE)
            _vstEventsToHost->numEvents = left;
         else
            _vstEventsToHost->numEvents = MAX_EVENTS_PER_TIMESLICE;

         for( int i = 0; i < _vstEventsToHost->numEvents; i++)
         {
            VstInt32 j = i+count;
            _vstSysexEventsToHost[i].type            = kVstSysExType;
            _vstSysexEventsToHost[i].byteSize        = _midiSysexEventsOut[0][j].byteSize;
            _vstSysexEventsToHost[i].deltaFrames     = _midiSysexEventsOut[0][j].deltaFrames;
            _vstSysexEventsToHost[i].flags           = 0;
            _vstSysexEventsToHost[i].dumpBytes       = _midiSysexEventsOut[0][j].dumpBytes;
            _vstSysexEventsToHost[i].resvd1          = 0;
            _vstSysexEventsToHost[i].sysexDump       = _midiSysexEventsOut[0][j].sysexDump;
            _vstSysexEventsToHost[i].resvd2          = 0;

            _vstEventsToHost->events[i] = (VstEvent*) &_vstSysexEventsToHost[i];
         }

         _vstEventsToHost->reserved  = 0;
         if( _midiSysexEventsOut[0].size() > 0)
            sendVstEventsToHost((VstEvents*)_vstEventsToHost);

         left -= _vstEventsToHost->numEvents;
         count += _vstEventsToHost->numEvents;
      }
      //flushing Midi Output Buffers before they are filled
      _cleanMidiOutBuffers();
}
   _cleanMidiInBuffers();
}

// --------------------------------------------------------------------------
// * process incoming midi or sysex events
// --------------------------------------------------------------------------
// copy incoming midi events from the hosts event queue to the plugins midi event list

VstInt32 MeeblipVST::processEvents( VstEvents* ev)
{
   DBG( 0, "\nMeeblipVST::processEvents" );

   if( PLUG_MIDI_INPUTS)
   {
      for( int i = 0; i < ev->numEvents; i++)
      {
         if( (ev->events[i])->type == kVstMidiType)
         {
            DBG( 1, "\n\nMeeblipVST::processEvents (midi)" );

            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            _midiEventsIn[0].push_back(*event);
         }
         else if( (ev->events[i])->type == kVstSysExType)
         {
            DBG( 1, "\n\nMeeblipVST::processEvents (sysex)" );

            VstMidiSysexEvent * event = (VstMidiSysexEvent*)ev->events[i];
            _midiSysexEventsIn[0].push_back(*event);
         }
      }
   }
   return 1;
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

void MeeblipVST::processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames)
{
   DBG( 0, "\nMeeblipVST::processReplacing" );

   //takes care of VstTimeInfo and such
   preProcess();

   //host should have called processEvents before process
   processMidiEvents( _midiEventsIn, _midiEventsOut, sampleFrames);
   processMidiSysexEvents( _midiSysexEventsIn, _midiSysexEventsOut, sampleFrames);

   float* in1  =  inputs[0];
   float* in2  =  inputs[1];

   float* out1 = outputs[0];
   float* out2 = outputs[1];

   while( --sampleFrames >= 0)
   {
      (*out1++) = (*in1++);
      (*out2++) = (*in2++);
   }

   //sending out MIDI events to Host to conclude wrapper
   postProcess();
}

// --------------------------------------------------------------------------
// *
// --------------------------------------------------------------------------

void MeeblipVST::processDoubleReplacing( double** inputs, double** outputs, VstInt32 sampleFrames)
{
   DBG( 0, "\nMeeblipVST::processDoubleReplacing" );

   //takes care of VstTimeInfo and such
   preProcess();

   //host should have called processEvents before process
   processMidiEvents(_midiEventsIn, _midiEventsOut, sampleFrames);
   processMidiSysexEvents( _midiSysexEventsIn, _midiSysexEventsOut, sampleFrames);


   double* in1  = inputs[0];
   double* in2  = inputs[1];
   double* out1 = outputs[0];
   double* out2 = outputs[1];

   while( --sampleFrames >= 0)
   {
      (*out1++) = (*in1++);
      (*out2++) = (*in2++);
   }

   //sending out MIDI events to Host to conclude wrapper
   postProcess();
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

tresult MeeblipVST::sendMidiCC( ParamID paramId, int32 midiValue)
{
   int midiChannel= FLOAT_TO_CHANNEL015( fMidiOutChannel); //outgoing midi channel

   CtrlNumber midiControllerNumber = getLayoutItem( paramId)->CCindex;
   DBG( 2, "      Midi out %d - %d", midiControllerNumber, midiValue );

   VstMidiEvent event = { 0 };

   event.deltaFrames = 0;
   event.midiData[0] = MIDI_CONTROLCHANGE + midiChannel;
   event.midiData[1] = (char)midiControllerNumber;
   event.midiData[2] = (char)midiValue;

   _midiEventsOut[0].push_back( event);

   return kResultOk;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::processMidiEvents( VstMidiEventVec *inputs, VstMidiEventVec *outputs, VstInt32 sampleFrames)
{
#if defined( _DEBUG)
   if( inputs[0].size() )
      DBG( 1, "\nMeeblipVST::processMidiEvents" );
#endif

   // process incoming events
   for( unsigned int i = 0; i < inputs[0].size(); i++)
   {
      VstMidiEvent event = inputs[0][i];

      short midiStatus  =  event.midiData[0] & 0xf0;         // scraping  channel
      short midiChannel = (event.midiData[0] & 0x0f) + 1;    // isolating channel (1-16)
      short midiData1   =  event.midiData[1] & 0x7f;
      short midiData2   =  event.midiData[2] & 0x7f;

      if( midiStatus == 0x80)
      {
         DBG( 2, "      Note off %d %d", midiData1, midiData2 );
      }
      else if( midiStatus == 0x90)
      {
         DBG( 2, "      Note on  %d %d", midiData1, midiData2 );
      }
      else if( midiStatus == 0xb0)
      {
         unsigned char cc = (unsigned char)midiData1;
         unsigned char intValue = (unsigned char)midiData2;

         // process midi control commands
         DBG( 2, "      control command %d %d", cc, intValue );

         ParamID paramId;
         tresult rc = mapCCToTag( cc, paramId);
         DBG( 2, "      set param %s  %d %g", rc == kResultTrue? "ok" : "fail", paramId, MIDI_TO_FLOAT( intValue) );

         if( rc == kResultTrue)
         {
            bool midiEnableStatus = midiEnable;
            midiEnable = false;
            DBG( 2, "      Midi %sable", midiEnable ? "en" : "dis" );
            setParameter( paramId, MIDI_TO_FLOAT( intValue)); // setup parameter, gui element
            midiEnable = midiEnableStatus;
            DBG( 2, "      Midi %sable", midiEnable ? "en" : "dis" );
         }
      }
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST::processMidiSysexEvents( VstSysexEventVec *inputs, VstSysexEventVec *outputs, VstInt32 sampleFrames)
{
   DBG( 0, "\nMeeblipVST::processMidiSysexEvents" );

   // process incoming events

   // pass thru sysex events
   copySysex();

   // process outgoing sysex events
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

VstInt32 MeeblipVST::float2KnobValue( float value, VstInt32 index)
{
   VstInt32 intVal;
   VstInt32 minValue  = getLayoutItem( index)->minValue;
   VstInt32 maxValue  = getLayoutItem( index)->maxValue;
   VstInt32 stepCount = getLayoutItem( index)->stepCount;

   if( value < 1.0f/(2*stepCount))
      intVal = minValue;
   else if( value >= 1.0f - 1.0f/(2*stepCount))
      intVal = maxValue;
   else
   {
      VstInt32 n = VstInt32( (value - 1.0f/(2*stepCount)) * stepCount) + 1;
      VstInt32 interval = (maxValue - minValue)/stepCount;
      intVal = minValue + n * interval;
   }

   return intVal;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

float MeeblipVST::KnobValue2float( VstInt32 intVal, VstInt32 index)
{
   float value;

   VstInt32 minValue  = getLayoutItem( index)->minValue;
   VstInt32 maxValue  = getLayoutItem( index)->maxValue;
   VstInt32 stepCount = getLayoutItem( index)->stepCount;

   value = float(intVal - minValue)/float( stepCount);

   return value;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
