// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        MeeblipVST_EditorView.cpp
//
// --------------------------------------------------------------------------
// Changelog
//
//    29.01.2014  AWe   set initial values for gui elements from layout structure
//    23.01.2014  AWe   in setParameter() correct calculation of stepcount interval
//    11.09.2013  AWe   adapted to use vstsdk2.4 from VST3 SDK and vstqui4
//    19.08.2013  AWe   distinguish gui and non-gui parameters and controls
//    01.08.2013  AWe   add changes from meeblip VST3 projoct( v0.4)
//    30.07.2013  AWe   code formating
//    27.07.2013  AWe   remove class CTextButton
//                      move GUI layout to file MeeblipVST_Layout.cpp
//    22.07.2013  AWe   use AGain Example from the VST SDK 3.0 as base for the MeeblipVST plugin
//                      remove function checkSizeConstraint()
//
// --------------------------------------------------------------------------


#include "MeeblipVST_EditorView.h"
#include "MeeblipVST_Layout.h"

// --------------------------------------------------------------------------
// Debug support
// --------------------------------------------------------------------------

#define VERBOSITY       99
#define VERBOSITY_MIN   1

#include "aweDBG.h"

// --------------------------------------------------------------------------
//  MeeblipVST_EditorView Implementation
// --------------------------------------------------------------------------

AEffGUIEditor* createEditor( AudioEffectX* effect)
{
   DBG( 1, "\nEffGUIEditor createEditor" );

   return new MeeblipVST_EditorView( effect);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

MeeblipVST_EditorView::MeeblipVST_EditorView( void* ptr)
   : AEffGUIEditor( ptr)
{
   DBG( 1, "\nMeeblipVST_EditorView::MeeblipVST_EditorView" );
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

bool MeeblipVST_EditorView::open( void* parent)
{
   DBG( 1, "\nMeeblipVST_EditorView::open" );

   for( int32 id = 0; id < numGuiItems; id++)
   {
      if( MeeblipVST_Bitmaps[ id].desc != NULL)
         imgBitmapList[id] = new CBitmap( MeeblipVST_Bitmaps[ id].desc);
   }

   // get the background image
   Background = imgBitmapList[ 0];

   CRect frameSize( 0, 0, Background->getWidth()-1, Background->getHeight()-1);
   CFrame* newFrame = new CFrame( frameSize, parent, this);

   // set Background image
   newFrame->setBackground( Background );
   newFrame->setSize( Background->getWidth(), Background->getHeight());           // window size

   // -- set the member frame to our frame
   frame = newFrame;

   DBG( 2, "      numLayoutItems %d", kNumGuiParameters );

   for( ParamID paramId = 0; paramId < kNumGuiParameters; paramId++)
   {
      MeeblipVST_LayoutItem *layoutItem = &MeeblipVST_Layout[ paramId];
      DBG2( 3, " ParamId %d", paramId );

      GuiItemId guiItem = layoutItem->guiItem;
      int32 posX = layoutItem->posX;
      int32 posY = layoutItem->posY;

      placeElement( paramId, newFrame, guiItem,  posX, posY);
   }

   // -- forget the bitmaps
   for( int32 id = 1; id < numGuiItems; id++)
   {
      imgBitmapList[id]->forget();
   }

   return true;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST_EditorView::close()
{
   DBG( 1, "\nMeeblipVST_EditorViewView::close" );

   // -- on close we need to delete the frame object.
   // -- once again we make sure that the member frame variable is set to zero before we delete it
   // -- so that calls to setParameter won't crash.

   if( frame)
   {
      frame->close();
      frame = 0;
   }

   if( Background)
   {
      Background->forget();
      Background = 0;
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST_EditorView::valueChanged( CControl* pControl)
{
   DBG( 1, "\nMeeblipVST_EditorView::valueChanged" );

   ParamID paramId = pControl->getTag();

   if( paramId < kNumGuiParameters)
   {
      //-- valueChanged is called whenever the user changes one of the guiControls in the User Interface( UI)
      effect->setParameterAutomated( paramId, pControl->getValue());

      DBG( 2, "      valueChanged %d : %g ", paramId, pControl->getValue() );
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void MeeblipVST_EditorView::setParameter( VstInt32 index, float value)
{
   DBG( 1, "\nMeeblipVST_EditorView::setParameter %d %g", index, value );

   //-- setParameter is called when the host automates one of the effects parameter.
   //-- The UI should reflect this state.
   if( frame && index < kNumGuiParameters)
   {
      VstInt32 stepCount = getLayoutItem( index)->stepCount;
      float valueScaled;

      if( stepCount == 1)
      {
         if( value < 0.5f )
            valueScaled = 0.0f;
         else
            valueScaled = 1.0f;
      }
      else
      {
         if( value < 1.0f/((stepCount+1)*2))
            valueScaled = 0.0f;
         else if( value >= 1.0f - 1.0f/((stepCount+1)*2))
            valueScaled = 1.0f;
         else
         {
            float n = floor( (value - 1.0f/(2*stepCount)) * stepCount) + 1.0f;
            valueScaled = (1.0f/stepCount) * n;
         }
      }
      DBG( 2, "     %g --> %g step %d", value, valueScaled, stepCount );

      guiControls[ index]->setValue( valueScaled);
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void  MeeblipVST_EditorView::placeElement( ParamID paramId, CFrame* frame, GuiItemId guiItemId, int32 x, int32 y)
{
   DBG( 1, "\nMeeblipVST_EditorView::placeElement %d", paramId );

   CBitmap* guiItemBitmap = imgBitmapList[ guiItemId];
   int32 subBitmapCount =  MeeblipVST_Bitmaps[ guiItemId].subBitmapCount;

   CCoord width  = guiItemBitmap->getWidth();
   CCoord height = guiItemBitmap->getHeight()/subBitmapCount;

   CRect r( 0, 0, width, height );
   r.offset( x-width/2, y-height/2);

   switch( guiItemId)
   {
      case KnobAnimated:
      case Knob2Animated:
         {
            CAnimKnob* knob = new CAnimKnob( r, this, paramId, guiItemBitmap);
            frame->addView( knob);
            guiControls[ paramId] = knob;
         }
         break;

      case ButtonAnimated:
         {
            CMovieButton* button = new CMovieButton(r, this, paramId, guiItemBitmap);
            frame->addView( button);

            // -- remember our guiControls so that we can sync them with the state of the effect
            guiControls[ paramId] = button;
         }
         break;
   }

   //-- sync parameter
   setParameter( paramId, effect->getParameter( paramId));
}

