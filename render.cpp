#include <Bela.h>
#include "coupled_oscillators.hpp"


CoupledOscillators co = CoupledOscillators();

bool setup(BelaContext *context, void *userData)
{
	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		float in = audioRead(context, n, 1);
		float out = co.step(in);
		
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}

	}
}

void cleanup(BelaContext *context, void *userData)
{

}