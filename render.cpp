#include <Bela.h>
#include "coupled_oscillators.hpp"

COConfig plate_conf = {.type=PLATE, .x=4, .y=4, .stiffness=0.005, .damping=0.0001};
COConfig string_conf = {.type=STRING, .len=16, .stiffness=0.01, .damping=0.001};


CoupledOscillators string = CoupledOscillators(string_conf);
CoupledOscillators plate = CoupledOscillators(plate_conf);

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