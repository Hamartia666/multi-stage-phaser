#pragma once
#include <pluginterfaces/vst/vsttypes.h>

class LowFrequencyOscillator
{
public:
	LowFrequencyOscillator();
	~LowFrequencyOscillator();

	void setOscillationFrequency(float normalisedNewValue)
	{
		double diff = maxFrequency - minFrequency;
		oscillationFrequency = minFrequency + (diff * normalisedNewValue);
	};
	double getOscillationFrequency() { return oscillationFrequency; };

	void setOscillationAmplitude(float normalissedAmplitude) {
		oscillationAmplitude = normalissedAmplitude;
	};
	float getOscillationAmplitude() { return oscillationAmplitude; };

	void setSampleRate(Steinberg::Vst::SampleRate newSampleRate) { sampleRate = newSampleRate; };

	void setInversion(float newValue)
	{
		if (newValue == 0.0f)
			invertMovement = false;
		if (newValue == 1.0f)
			invertMovement = true;
	};

	float next(int channelNumber);
	void resetCounter();

private:
	void updateCounter(int channelNumber);

	float counterPerChannel[2] = { 0.0f, 0.0f };
	Steinberg::Vst::SampleRate sampleRate = 44100;
	double oscillationFrequency = 0.0;
	float oscillationAmplitude = 1.0f;
	bool invertMovement = false;

	const float maxAmplitude = 0.35f;
	const float minAmplitude = 0.0f;
	const double maxFrequency = 6.0f;
	const double minFrequency = 0.0f;
};