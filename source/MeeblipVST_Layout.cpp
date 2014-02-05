// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST_Layout.cpp
//
// --------------------------------------------------------------------------
// Changelog
//
//    29.01.2014  AWe   set initial values for gui elements from layout structure
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstqui4
//    01.08.2013  AWe   add mappinng methods mapCCToTag
//    27.07.2013  AWe   use a structure for the GUI layout,
//                      change this file for a new GUI layout
//
// --------------------------------------------------------------------------

#include "MeeblipVST_Layout.h"

// --------------------------------------------------------------------------
// Debug support
// --------------------------------------------------------------------------

#define VERBOSITY       99
#define VERBOSITY_MIN   1

#include "aweDBG.h"

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

MeeblipVST_LayoutItemBitmap MeeblipVST_Bitmaps[] =
{ // guiItemId             desc                        subBitmapCount
   { Background,          "MeeblipVST_Background.png", 0     },   // Background always first bitmap

   { KnobAnimated,        "KnobAnimated.png"         , 129   },   // imgKnobAnimated
   { Knob2Animated,       "Knob2Animated.png"        , 129   },   // imgKnob2Animated
   { ButtonAnimated,      "ButtonAnimated.png"       , 2     }    // imgSwitchAnimated
};

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#if 1
MeeblipVST_LayoutItem MeeblipVST_Layout[] =
{
  // Name           gui             X    Y     min  max dflt step  CC
   { "OSCB_WAVE"  , ButtonAnimated,  45, 244,   0,   1,   0,   1,  75 },
   { "OSCB_ENABL" , ButtonAnimated, 105, 263,   0,   1,   0,   1,  74 },
   { "OSCB_OCT"   , ButtonAnimated, 165, 263,   0,   1,   0,   1,  73 },
   { "ANTI_ALIAS" , ButtonAnimated, 225, 263,   0,   1,   0,   1,  72 },
   { "LFO_WAVE"   , ButtonAnimated, 286, 263,   0,   1,   0,   1,  67 },
   { "LFO_RANDOM" , ButtonAnimated, 346, 263,   0,   1,   0,   1,  66 },
   { "OSC_FM"     , ButtonAnimated, 406, 263,   0,   1,   0,   1,  65 },
   { "KNOB_SHIFT" , ButtonAnimated, 466, 244,   0,   1,   0,   1,  64 },

   { "OSCA_WAVE"  , ButtonAnimated,  45, 168,   0,   1,   0,   1,  79 },
   { "PWM_SWEEP"  , ButtonAnimated, 105, 187,   0,   1,   0,   1,  78 },
   { "OSCA_NOISE" , ButtonAnimated, 165, 187,   0,   1,   0,   1,  77 },
   { "SUSTAIN"    , ButtonAnimated, 225, 187,   0,   1,   0,   1,  76 },
   { "LFO_DEST"   , ButtonAnimated, 286, 187,   0,   1,   0,   1,  71 },
   { "LFO_ENABLE" , ButtonAnimated, 346, 187,   0,   1,   0,   1,  70 },
   { "DISTORTION" , ButtonAnimated, 406, 187,   0,   1,   0,   1,  69 },
   { "FILTER_MODE", ButtonAnimated, 466, 168,   0,   1,   0,   1,  68 },

   { "OSC_DETUNE" , Knob2Animated,   45,  93, -64,  63,   0, 127,  55 },
   { "PULSE_KNOB" , KnobAnimated,   105, 112,   0, 127,  64, 127,  54 },
   { "PORTAMENTO" , KnobAnimated,   165, 112,   0, 127,   0, 127,  53 },
   { "VCFENVMOD"  , Knob2Animated,  225, 112, -64,  63,   0, 127,  52 },
   { "LFOLEVEL"   , KnobAnimated,   286, 112,   0, 127,   0, 127,  51 },
   { "LFOFREQ"    , KnobAnimated,   346, 112,   0, 127,   0, 127,  50 },
   { "CUTOFF"     , KnobAnimated,   406, 112,   0, 127, 127, 127,  49 },
   { "RESONANCE"  , KnobAnimated,   466,  93,   0, 127,   0, 127,  48 },

// { "spare"      , KnobAnimated,    45,  17,   0, 127,   0, 127,  63 },
// { "spare"      , KnobAnimated,   105,  36,   0, 127,   0, 127,  62 },
   { "DCF_ATTACK" , KnobAnimated,   165,  36,   0, 127,   0, 127,  61 },
   { "DCF_DECAY"  , KnobAnimated,   225,  36,   0, 127,  96, 127,  60 },
   { "AMP_ATTACK" , KnobAnimated,   286,  36,   0, 127,   0, 127,  59 },
   { "AMP_DECAY"  , KnobAnimated,   346,  36,   0, 127,  96, 127,  58 },
// { "spare"      , KnobAnimated,   406,  36,   0, 127,   0, 127,  57 },
// { "spare"      , KnobAnimated,   466,  17,   0, 127,   0, 127,  56 }
};
#else

MeeblipVST_LayoutItem MeeblipVST_Layout[] =
{
  // Name           gui             X    Y     min  max dflt step  CC
   { "OSCB_WAVE"  , ButtonAnimated,  54, 244,    0,   1,   0,   1, 75  },
   { "OSCB_ENABL" , ButtonAnimated, 130, 263,    0,   1,   0,   1, 74  },
   { "OSCB_OCT"   , ButtonAnimated, 206, 263,    0,   1,   0,   1, 73  },
   { "ANTI_ALIAS" , ButtonAnimated, 281, 263,    0,   1,   0,   1, 72  },
   { "LFO_WAVE"   , ButtonAnimated, 357, 263,    0,   1,   0,   1, 67  },
   { "LFO_RANDOM" , ButtonAnimated, 433, 263,    0,   1,   0,   1, 66  },
   { "OSC_FM"     , ButtonAnimated, 509, 263,    0,   1,   0,   1, 65  },
   { "KNOB_SHIFT" , ButtonAnimated, 585, 244,    0,   1,   0,   1, 64  },

   { "OSCA_WAVE"  , ButtonAnimated,  54, 168,    0,   1,   0,   1, 79  },
   { "PWM_SWEEP"  , ButtonAnimated, 130, 187,    0,   1,   0,   1, 78  },
   { "OSCA_NOISE" , ButtonAnimated, 206, 187,    0,   1,   0,   1, 77  },
   { "SUSTAIN"    , ButtonAnimated, 281, 187,    0,   1,   0,   1, 76  },
   { "LFO_DEST"   , ButtonAnimated, 357, 187,    0,   1,   0,   1, 71  },
   { "LFO_ENABLE" , ButtonAnimated, 433, 187,    0,   1,   0,   1, 70  },
   { "DISTORTION" , ButtonAnimated, 509, 187,    0,   1,   0,   1, 69  },
   { "FILTER_MODE", ButtonAnimated, 585, 168,    0,   1,   0,   1, 68  },

   { "OSC_DETUNE" , Knob2Animated,   54,  92,  -64,  63,   0, 127, 55 },
   { "PULSE_KNOB" , KnobAnimated,   130, 111,    0, 127,  64, 127, 54 },
   { "PORTAMENTO" , KnobAnimated,   206, 111,    0, 127,   0, 127, 53 },
   { "VCFENVMOD"  , Knob2Animated,  281, 111,  -64,  63,   0, 127, 52 },
   { "LFOLEVEL"   , KnobAnimated,   357, 111,    0, 127,   0, 127, 51 },
   { "LFOFREQ"    , KnobAnimated,   433, 111,    0, 127,   0, 127, 50 },
   { "CUTOFF"     , KnobAnimated,   509, 111,    0, 127, 127, 127, 49 },
   { "RESONANCE"  , KnobAnimated,   585,  92,    0, 127,   0, 127, 48 },

// { "spare"      , KnobAnimated,    54,  26,    0, 127,   0, 127, 63 },
// { "spare"      , KnobAnimated,   130,  35,    0, 127,   0, 127, 62 },
   { "AMP_ATTACK" , KnobAnimated,   206,  35,    0, 127,   0, 127, 61 },
   { "AMP_DECAY"  , KnobAnimated,   281,  35,    0, 127,  96, 127, 60 },
   { "DCF_ATTACK" , KnobAnimated,   357,  35,    0, 127,   0, 127, 59 },
   { "DCF_DECAY"  , KnobAnimated,   433,  35,    0, 127,  96, 127, 58 },
// { "spare"      , KnobAnimated,   509,  35,    0, 127,   0, 127, 57 },
// { "spare"      , KnobAnimated,   585,  26,    0, 127,   0, 127, 56 }
};

MeeblipVST_LayoutItem MeeblipVST_Layout[] =
{
  // Name           gui             X    Y     min  max dflt step  CC
   { "OSCB_WAVE"  , ButtonAnimated,  42, 383,    0,   1,   0,   1, 75  },
   { "OSCB_ENABL" , ButtonAnimated, 123, 399,    0,   1,   0,   1, 74  },
   { "OSCB_OCT"   , ButtonAnimated, 203, 399,    0,   1,   0,   1, 73  },
   { "ANTI_ALIAS" , ButtonAnimated, 279, 399,    0,   1,   0,   1, 72  },
   { "LFO_WAVE"   , ButtonAnimated, 359, 399,    0,   1,   0,   1, 67  },
   { "LFO_RANDOM" , ButtonAnimated, 436, 399,    0,   1,   0,   1, 66  },
   { "OSC_FM"     , ButtonAnimated, 517, 399,    0,   1,   0,   1, 65  },
   { "KNOB_SHIFT" , ButtonAnimated, 596, 383,    0,   1,   0,   1, 64  },

   { "OSCA_WAVE"  , ButtonAnimated,  42, 312,    0,   1,   0,   1, 79  },
   { "PWM_SWEEP"  , ButtonAnimated, 123, 328,    0,   1,   0,   1, 78  },
   { "OSCA_NOISE" , ButtonAnimated, 203, 328,    0,   1,   0,   1, 77  },
   { "SUSTAIN"    , ButtonAnimated, 279, 328,    0,   1,   0,   1, 76  },
   { "LFO_DEST"   , ButtonAnimated, 359, 328,    0,   1,   0,   1, 71  },
   { "LFO_ENABLE" , ButtonAnimated, 436, 328,    0,   1,   0,   1, 70  },
   { "DISTORTION" , ButtonAnimated, 517, 328,    0,   1,   0,   1, 69  },
   { "FILTER_MODE", ButtonAnimated, 596, 312,    0,   1,   0,   1, 68  },

   { "OSC_DETUNE" , Knob2Animated,   42, 229,  -64,  63,   0, 127, 55 },
   { "PULSE_KNOB" , KnobAnimated,   123, 244,    0, 127,  64, 127, 54 },
   { "PORTAMENTO" , KnobAnimated,   203, 244,    0, 127,   0, 127, 53 },
   { "VCFENVMOD"  , Knob2Animated,  279, 244,  -64,  63,   0, 127, 52 },
   { "LFOLEVEL"   , KnobAnimated,   359, 244,    0, 127,   0, 127, 51 },
   { "LFOFREQ"    , KnobAnimated,   436, 244,    0, 127,   0, 127, 50 },
   { "CUTOFF"     , KnobAnimated,   517, 244,    0, 127, 127, 127, 49 },
   { "RESONANCE"  , KnobAnimated,   596, 229,    0, 127,   0, 127, 48 },

// { "spare"      , KnobAnimated,    42, 151,    0, 127,   0, 127, 63 },
// { "spare"      , KnobAnimated,   123, 166,    0, 127,   0, 127, 62 },
   { "AMP_ATTACK" , KnobAnimated,   203, 166,    0, 127,   0, 127, 61 },
   { "AMP_DECAY"  , KnobAnimated,   279, 166,    0, 127,  96, 127, 60 },
   { "DCF_ATTACK" , KnobAnimated,   359, 166,    0, 127,   0, 127, 59 },
   { "DCF_DECAY"  , KnobAnimated,   436, 166,    0, 127,  96, 127, 58 },
// { "spare"      , KnobAnimated,   517, 166,    0, 127,   0, 127, 57 },
// { "spare"      , KnobAnimated,   596, 151,    0, 127,   0, 127, 56 }
};
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

tresult mapCCToTag( CtrlNumber midiControllerNumber, ParamID &tag)
{
   // search the tag for the given midi cc number
   for( tag = 0; tag < kNumGuiParameters; tag++)
   {
      if( midiControllerNumber == MeeblipVST_Layout[ tag].CCindex)
      {
         DBG( 2, "      midiCC %d --> paramId %d", midiControllerNumber, tag);
         return kResultTrue;
      }
   }

   return kResultFalse;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

MeeblipVST_LayoutItem *getLayoutItem( ParamID tag )
{
   return &MeeblipVST_Layout[ tag];
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

