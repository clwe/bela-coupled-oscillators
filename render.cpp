#include <Bela.h>
#include "coupled_oscillators.hpp"
#include <cmath>

COConfig plate_conf = {.type=PLATE, .x=4, .y=4, .stiffness=0.0005, .damping=0.0001};
COConfig string_conf = {.type=STRING, .len=32, .stiffness=0.01, .damping=0.0001};


CoupledOscillators string = CoupledOscillators(string_conf);
CoupledOscillators plate = CoupledOscillators(plate_conf);

float gFrequency = 7.0;
float gPhase = 0.0;
float gInverseSampleRate;

bool setup(BelaContext *context, void *userData)
{
	gInverseSampleRate = context->audioFrames / context->audioSampleRate;
	gPhase = 0.0;
	return true;
}

void render(BelaContext *context, void *userData)
{
	float lfo = 0.05 * sinf(gPhase);
	plate.modulateGlobalStiffness(lfo);
	string.modulateGlobalStiffness(lfo);
	gPhase += 2.0 * M_PI * gFrequency * gInverseSampleRate;
	if(gPhase > 2.0 * M_PI)
		gPhase -= 2.0 * M_PI;
			
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		float in = audioRead(context, n, 1);
		int inputNode = 0;
		int outputNode = 4;
		float out_s = string.step(in, inputNode, outputNode);
		float out_p = plate.step(out_s, 1, outputNode);
		audioWrite(context, n, 0, out_p);
		audioWrite(context, n, 1, out_p);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}