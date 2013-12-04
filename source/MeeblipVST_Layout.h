// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST_Layout.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstqui4
//    19.08.2013  AWe   add non gui parameters for midi channel selection
//    01.08.2013  AWe   add MeeblipVST_ParamIds (previously they have their own file)
//                      add mappinng methods mapCCToIndex
//    27.07.2013  AWe   use a structure for the GUI layout,
//
// --------------------------------------------------------------------------

#ifndef __MeeblipVST_Layout__
#define __MeeblipVST_Layout__

#include "vstgui/plugin-bindings/aeffguieditor.h"
#include "aweVSTtypes.h"

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

enum MeeblipVST_ParamIds
{
//   kNumGuiParameters = sizeof( MeeblipVST_Layout)/ sizeof( MeeblipVST_LayoutItem),
   kNumGuiParameters = 28,

   // extra parameters
   kMidiInChannel = kNumGuiParameters,
   kMidiOutChannel,

   kNumExtraParameters = 2,
};

enum GuiItemId
{
   Background = 0,

   KnobAnimated,
   Knob2Animated,
   ButtonAnimated,

   numGuiItems
};

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

struct tagMeeblipVST_LayoutItemBitmap
{
   const GuiItemId      guiItemId;
   const UTF8StringPtr  desc;
   const int32          subBitmapCount;
};

typedef struct tagMeeblipVST_LayoutItemBitmap  MeeblipVST_LayoutItemBitmap;

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

struct tagMeeblipVST_LayoutItem
{
   const char *parameterName;

   const GuiItemId guiItem;
   const int32 posX;
   const int32 posY;

   const int8 minValue;
   const int8 maxValue;
   const int8 defaultValue;
   const int8 stepCount;
   const int8 CCindex;           // midiControllerNumber

};

typedef struct tagMeeblipVST_LayoutItem  MeeblipVST_LayoutItem;

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

MeeblipVST_LayoutItemBitmap MeeblipVST_Bitmaps[];
MeeblipVST_LayoutItem MeeblipVST_Layout[];

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

tresult mapCCToTag( CtrlNumber midiControllerNumber, ParamID &tag);
MeeblipVST_LayoutItem *getLayoutItem( ParamID tag);

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#endif
