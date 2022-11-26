#include <vector>
using namespace std;

#define N_MASSES 32

class CoupledOscillators
{
public:
	CoupledOscillators(){
		for (int i=0; i<N_MASSES; i++){
			_x_pos[i]= 0;
			_x_pos_old[i]= 0;
			_force[i] = 0;
		}
		_stiffness =  0.01;
		_friction  = 0.008;
		_mass = 0.1;
		_old_audio_input = 0;
	};
	float spring(float x1, float x2, float stiffness) {
		float spring_force = -stiffness*(x2-x1);
		return spring_force;
	}
	
	float damper(float x1, float x1_old, float x2, float x2_old, float friction) {
		float friction_force = -friction*((x2-x2_old)-(x1-x1_old));
		return friction_force;
	}
	
	float massPosUpdate(float x, float x_old, float force_on_mass, float mass) {
		float x_update =  2*x-x_old+force_on_mass/mass; 
		return x_update;
	}
	float calculateInputForce(float new_audio_input){
		float new_input_force = new_audio_input - _old_audio_input;
		_old_audio_input = new_audio_input;
		return new_input_force;
	}
	
	float step(float in, int inputNode) {
		float input_force = calculateInputForce(in);
		//_force[0] = input_force;
		_force[0] = spring(0, _x_pos[0], _stiffness) + damper(0, 0, _x_pos[0], _x_pos_old[0], _friction);
		
		for (int i = 1; i<N_MASSES-1; i++){
			_force[i]  = spring(_x_pos[i-1], _x_pos[i], _stiffness) + damper(_x_pos[i-1], _x_pos_old[i-1], _x_pos[i], _x_pos_old[i], _friction);
			if (i==inputNode){
				_force[i]+=0.1*input_force;
			}
		}
		_force[N_MASSES-1] = spring(_x_pos[N_MASSES-1], 0, _stiffness) + damper(_x_pos[N_MASSES-1], _x_pos_old[N_MASSES-1], 0, 0, _friction);
		
		float x_update = massPosUpdate( _x_pos[0], _x_pos_old[0], _force[0], _mass);
		_x_pos_old[0] = _x_pos[0];
		_x_pos[0]= x_update;
		
		for (int i = 0; i<N_MASSES-1; i++){
			x_update = massPosUpdate( _x_pos[i], _x_pos_old[i], _force[i]-_force[i+1], _mass);
			_x_pos_old[i] = _x_pos[i];
			_x_pos[i]= x_update;
		}
		
		x_update = massPosUpdate( _x_pos[N_MASSES-1], _x_pos_old[N_MASSES-1], -_force[N_MASSES-1], _mass);
		_x_pos_old[N_MASSES-1] = _x_pos[N_MASSES-1];
		_x_pos[N_MASSES-1]= x_update;

		return _x_pos[(int)(N_MASSES/2)];
	}
private:
	float _x_pos[N_MASSES];
	float _x_pos_old[N_MASSES];
	float _force[N_MASSES];
	float _friction;
	float _stiffness;
	float _mass;
	float _old_audio_input;
};