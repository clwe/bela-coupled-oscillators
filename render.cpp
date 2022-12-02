#include <Bela.h>
#include <cmath>
#include <libraries/Trill/Trill.h>
#include <libraries/OnePole/OnePole.h>
#include "coupled_oscillators.hpp"

COConfig plate_conf = {.type=PLATE, .x=5, .y=5, .stiffness=0.0005, .damping=0.0001};
COConfig string_conf = {.type=STRING, .size=32, .stiffness=0.01, .damping=0.0001};
CoupledOscillators string = CoupledOscillators(string_conf);
CoupledOscillators plate = CoupledOscillators(plate_conf);

#define NUM_TOUCH 2 // Number of touches on Trill sensor

// Trill object declaration
Trill touchSensor;

// Location of touches on Trill Bar
float gTouchLocation[NUM_TOUCH] = { 0.0, 0.0 };
// Size of touches on Trill Bar
float gTouchSize[NUM_TOUCH] = { 0.0, 0.0 };
// Number of active touches
int gNumActiveTouches = 0;

// Sleep time for auxiliary task in microseconds
unsigned int gTaskSleepTime = 12000; // microseconds

// One Pole filters objects declaration
OnePole freqFilt[NUM_TOUCH], ampFilt[NUM_TOUCH];
// Frequency of one pole filters
float gCutOffFreq = 5, gCutOffAmp = 15;
// Range for oscillator frequency mapping
float gFreqRange[2] = { -0.5, 0.5 };
// Range for oscillator amplitude mapping
float gAmplitudeRange[2] = { 0.0, 1.0 } ;

/*
* Function to be run on an auxiliary task that reads data from the Trill sensor.
* Here, a loop is defined so that the task runs recurrently for as long as the
* audio thread is running.
*/
void loop(void*)
{
	while(!Bela_stopRequested())
	{
		// Read locations from Trill sensor
		touchSensor.readI2C();
		gNumActiveTouches = touchSensor.getNumTouches();
		for(unsigned int i = 0; i <  gNumActiveTouches; i++) {
			gTouchLocation[i] = touchSensor.touchLocation(i);
			gTouchSize[i] = touchSensor.touchSize(i);
		}
		// For all inactive touches, set location and size to 0
		for(unsigned int i = gNumActiveTouches; i < NUM_TOUCH; i++) {
			gTouchLocation[i] = 0.0;
			gTouchSize[i] = 0.0;
		}
		usleep(gTaskSleepTime);
	}
}

bool setup(BelaContext *context, void *userData)
{
	// Setup a Trill Bar sensor on i2c bus 1, using the default mode and address
	if(touchSensor.setup(1, Trill::BAR) != 0) {
		fprintf(stderr, "Unable to initialise Trill Bar\n");
		return false;
	}
	touchSensor.printDetails();

	// Set and schedule auxiliary task for reading sensor data from the I2C bus
	Bela_runAuxiliaryTask(loop);

	// For each possible touch...
	for(unsigned int i = 0; i < NUM_TOUCH; i++) {
		// Setup low pass filters for smoothing frequency and amplitude
		freqFilt[i].setup(gCutOffFreq, context->audioSampleRate);
		ampFilt[i].setup(gCutOffAmp, context->audioSampleRate);
	}

	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		/* For each touch:
		*
		* 	- Map touch location to sitffness of the resonator
		* 	and smooth value changes using a single pole LP filter
		* 	- Map touch size toa amplitude of the resonator and
		* 	smooth value changes using a single pole LP filter
		*/

		float frq_string, amp_string;
		frq_string = map(gTouchLocation[0], 0, 1, gFreqRange[0], gFreqRange[1]);
		amp_string = map(gTouchSize[0], 0, 1, gAmplitudeRange[0], gAmplitudeRange[1]);
		amp_string = ampFilt[0].process(amp_string);
		string.modulateGlobalStiffness(frq_string);
		
		float frq_plate, amp_plate;
		frq_plate = map(gTouchLocation[1], 0, 1, gFreqRange[0], gFreqRange[1]);
		amp_plate = map(gTouchSize[1], 0, 1, gAmplitudeRange[0], gAmplitudeRange[1]);
		amp_plate = ampFilt[1].process(amp_plate);
		plate.modulateGlobalStiffness(frq_plate);

		float in = audioRead(context, n, 0);
		int inputNode = 0;
		int outputNode = 4;
		float out_s = amp_string * string.step(in, inputNode, outputNode);
		float out_p = amp_plate * plate.step(out_s, 1, outputNode);
		audioWrite(context, n, 0, out_s+out_p);
		audioWrite(context, n, 1, out_s+out_p);
	}
}

void cleanup(BelaContext *context, void *userData)
{}
