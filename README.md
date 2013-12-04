meeblip-VST
===========

meeblipSE-V2 VST plugin. Requires Steinberg VST3 SDK (3.5.2) and Visual C++ 2010 Express.

Setup the build enviroment for Meeblip VST plugin
--------------------------------------------------

* get the Microsoft Visual C++ COmpiler from their web site
  Visual Studio 10 Express

* get the VST SDK 3.0 from the Steinberg Web site
  get an account (it's free) and login
  goto the 3rd party page
  download and install the SDK

* define an enviroment variable to the installed VST3 SDK
  set VSTSDK_ROOT=F:\_myprojects\VST\VST3 SDK

* open and build %VSTSDK_ROOT%\base\win\base_vc10.sln

* open with VC and convert to a vc10 project
   %VSTSDK_ROOT%\vstgui4\vstgui\ide\visualstudio\vstgui.sln
   %VSTSDK_ROOT%\vstgui4\vstgui\ide\visualstudio\vstgui.vcproj

* in the ...\win\MeeblipVST_vc10.sln file change the relative path for both packages
  "base_vc10", "..\..\..\..\..\VST\VST3 SDK\base\win\base_vc10.vcxproj"
  "vstgui", "..\..\..\..\..\VST\VST3 SDK\vstgui4\vstgui\ide\visualstudio\vstgui.vcxproj"


