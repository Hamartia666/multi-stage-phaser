//------------------------------------------------------------------------
// Copyright(c) 2024 HonoursProject.
//------------------------------------------------------------------------

#include "mspcontroller.h"
#include "mspcids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "params.h"

using namespace Steinberg;

namespace HonoursProject {

//------------------------------------------------------------------------
// MultiStagePhaserController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
	parameters.addParameter(STR16("Mix"), nullptr, 0, 1.0f, Vst::ParameterInfo::kCanAutomate, mixId);
	parameters.addParameter(STR16("Stage 1 LFO Frequency"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo1FreqId);
	parameters.addParameter(STR16("Stage 1 LFO Depth"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo1AmpId);
	parameters.addParameter(STR16("Stage 1 LFO Movement"), nullptr, 1, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo1InvertId);
	parameters.addParameter(STR16("Stage 1 APF Frequency"), nullptr, 0, 0.8f, Vst::ParameterInfo::kCanAutomate, apf1FreqId);
	parameters.addParameter(STR16("Stage 1 Toggle"), nullptr, 1, 1.0f, Vst::ParameterInfo::kCanAutomate, apf1ToggleId);

	parameters.addParameter(STR16("Stage 2 LFO Frequency"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo2FreqId);
	parameters.addParameter(STR16("Stage 2 LFO Depth"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo2AmpId);
	parameters.addParameter(STR16("Stage 2 LFO Movement"), nullptr, 1, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo2InvertId);
	parameters.addParameter(STR16("Stage 2 APF Frequency"), nullptr, 0, 0.5f, Vst::ParameterInfo::kCanAutomate, apf2FreqId);
	parameters.addParameter(STR16("Stage 2 Toggle"), nullptr, 1, 1.0f, Vst::ParameterInfo::kCanAutomate, apf2ToggleId);

	parameters.addParameter(STR16("Stage 3 LFO Frequency"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo3FreqId);
	parameters.addParameter(STR16("Stage 3 LFO Depth"), nullptr, 0, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo3AmpId);
	parameters.addParameter(STR16("Stage 3 LFO Movement"), nullptr, 1, 0.0f, Vst::ParameterInfo::kCanAutomate, lfo3InvertId);
	parameters.addParameter(STR16("Stage 3 APF Frequency"), nullptr, 0, 0.2f, Vst::ParameterInfo::kCanAutomate, apf3FreqId);
	parameters.addParameter(STR16("Stage 3 Toggle"), nullptr, 1, 1.0f, Vst::ParameterInfo::kCanAutomate, apf3ToggleId);


	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API MultiStagePhaserController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "mspeditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
} // namespace HonoursProject
