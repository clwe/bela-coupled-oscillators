#include <Bela.h>
#include "coupled_oscillators.hpp"
float stiffn_str = 0.01;
float stiffn_plt = 0.005;
CoupledOscillators string = CoupledOscillators(CoupledOscillators::STRING, stiffn_str, 16);
CoupledOscillators plate = CoupledOscillators(CoupledOscillators::PLATE, stiffn_plt, 4,4);

bool setup(BelaContext *context, void *userData)
{
	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		float in = audioRead(context, n, 1);
		int inputNode = 0;
		int outputNode = 4;
		float out_s = string.verletStep(in, inputNode, outputNode);
		float out_p = plate.verletStep(out_s, 1, outputNode);
		audioWrite(context, n, 0, out_p);
		audioWrite(context, n, 1, out_p);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}