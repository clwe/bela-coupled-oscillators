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
		int inputNode = 16;
		float out = co.step(in, inputNode);
		audioWrite(context, n, 0, out);
		audioWrite(context, n, 1, out);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}