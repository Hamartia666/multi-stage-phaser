//------------------------------------------------------------------------
// Copyright(c) 2024 HonoursProject.
//------------------------------------------------------------------------

#include "mspprocessor.h"
#include "mspcids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "params.h"
#include "lfo.h"
#include "apf.h"

using namespace Steinberg;

namespace HonoursProject {
//------------------------------------------------------------------------
// MultiStagePhaserProcessor
//------------------------------------------------------------------------
MultiStagePhaserProcessor::MultiStagePhaserProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kMultiStagePhaserControllerUID);
}

//------------------------------------------------------------------------
MultiStagePhaserProcessor::~MultiStagePhaserProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData(index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount();
				if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
				{
					switch (paramQueue->getParameterId())
					{
					case mixId:
						mix = (float)value;
						break;
					case lfo1FreqId:
						lfo[0].setOscillationFrequency((float)value);
						break;
					case lfo1AmpId:
						lfo[0].setOscillationAmplitude((float)value);
						break;
					case lfo1InvertId:
						lfo[0].setInversion((float)value);
						break;
					case apf1FreqId:
						apf[0].setCornerFrequency((float)value);
						break;
					case apf1ToggleId:
						apf[0].setEnable((float)value);
						break;
					case lfo2FreqId:
						lfo[1].setOscillationFrequency((float)value);
						break;
					case lfo2AmpId:
						lfo[1].setOscillationAmplitude((float)value);
						break;
					case lfo2InvertId:
						lfo[1].setInversion((float)value);
						break;
					case apf2FreqId:
						apf[1].setCornerFrequency((float)value);
						break;
					case apf2ToggleId:
						apf[1].setEnable((float)value);
						break;
					case lfo3FreqId:
						lfo[2].setOscillationFrequency((float)value);
						break;
					case lfo3AmpId:
						lfo[2].setOscillationAmplitude((float)value);
						break;
					case lfo3InvertId:
						lfo[2].setInversion((float)value);
						break;
					case apf3FreqId:
						apf[2].setCornerFrequency((float)value);
						break;
					case apf3ToggleId:
						apf[2].setEnable((float)value);
						break;
					}
				}
			}
		}
	}

	//--- Here you have to implement your processing
	if (data.numSamples == 0 || data.numOutputs == 0)
		return kResultTrue;

	int32 numOfChannels = data.inputs[0].numChannels;
	Vst::Sample32** in = data.inputs[0].channelBuffers32;
	Vst::Sample32** out = data.outputs[0].channelBuffers32;

	float blend = mix * 0.5f;

	for (int32 ch = 0; ch < numOfChannels; ++ch)
	{
		Vst::Sample32* ptrIn = in[ch];
		Vst::Sample32* ptrOut = out[ch];
		Vst::Sample32 temp;
		for (int32 i = 0; i < data.numSamples; ++i)
		{
			temp = *ptrIn;
			float oscillatorValue = 0.0f;
			for (int stage = 0; stage < 3; ++stage)
			{
				if (apf[stage].isEnabled())
				{
					oscillatorValue = lfo[stage].next(ch);
					apf[stage].setNewOscillatorValue(oscillatorValue);
					temp = apf[stage].processSample(temp, ch);
				}		
			}
			temp = (temp * blend) + *ptrIn * (1 - blend);

			*ptrOut = temp;

			ptrIn++;
			ptrOut++;
		}
	}

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MultiStagePhaserProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace HonoursProject
