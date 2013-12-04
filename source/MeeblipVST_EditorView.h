// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST_EditorView.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstqui4
//    19.08.2013  AWe   distinguish gui and non-gui parameters and controls
//    01.08.2013  AWe   add changes from meeblip VST3 projoct( v0.4)
//    30.07.2013  AWe   code formating
//    27.07.2013  AWe   move enums for VST_element to file MeeblipVST_Layout.h
//    22.07.2013  AWe   use AGain Example for VST SDK 3.0 as base for MeeblipVST plugin
//
// --------------------------------------------------------------------------

#ifndef __MeeblipVST_EditorView__
#define __MeeblipVST_EditorView__

#include "vstgui/plugin-bindings/aeffguieditor.h"
#include "aweVSTtypes.h"

#include "MeeblipVST_Layout.h"

// --------------------------------------------------------------------------
// MeeblipVST_EditorView Declaration
// --------------------------------------------------------------------------

class MeeblipVST_EditorView : public AEffGUIEditor, public CControlListener
{
public:
   MeeblipVST_EditorView( void*);

   // from AEffGUIEditor
   bool open( void* ptr);
   void close();
   void setParameter( VstInt32 index, float value);

   // from CControlListener
   void valueChanged( CControl* pControl);

protected:
   CBitmap* Background;

   CBitmap* imgBitmapList[ numGuiItems];

   CControl* guiControls[ kNumGuiParameters];

   void placeElement( ParamID paramId, CFrame* frame, GuiItemId guiItemId, int32 x, int32 y);
};

#endif // __MeeblipVST_EditorView__

