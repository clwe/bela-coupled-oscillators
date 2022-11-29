#include "coupled_oscillators.hpp"
#include "stiffness_matrix.hpp"
#include "defines.hpp"
using namespace std;


CoupledOscillators::CoupledOscillators():_stiffM(N_MASSES) {

	for (int i=0; i<N_MASSES; i++){
		_x_pos[i]= 0;
		_x_pos_old[i]= 0;
		_force[i] = 0;
		_x[i]=0;
		_v[i]=0;
		_a[i]=0;
	}
	_stiffness =  0.01;
	_friction  = 0.0001;
	_mass = 0.1;
	_old_audio_input = 0;
};
float CoupledOscillators::spring(float x1, float x2, float stiffness) {
	float spring_force = -stiffness*(x2-x1);
	return spring_force;
}
	
float CoupledOscillators::damper(float x1, float x1_old, float x2, float x2_old, float friction) {
	float friction_force = -friction*((x2-x2_old)-(x1-x1_old));
	return friction_force;
}
	
float CoupledOscillators::massPosUpdate(float x, float x_old, float force_on_mass, float mass) {
	float x_update =  2*x-x_old+force_on_mass/mass; 
	return x_update;
}
float CoupledOscillators::calculateInputVelocity(float new_audio_input){
	float new_input_velocity = new_audio_input - _old_audio_input;
	_old_audio_input = new_audio_input;
	return new_input_velocity;
}
float CoupledOscillators::getAccel(float stiffness, float x_new) {
	return 0;
}

	
float CoupledOscillators::step(float in, int inputNode) {
	float input_force = calculateInputVelocity(in);
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

float CoupledOscillators::verletStep(float in, int input_node, int output_node) {
	
	float input_velocity = calculateInputVelocity(in);
	
	//    dx = v_old * dt + 0.5 * a_old * dt^2;% - damping * a_old * dt^2;
    //    x_new = x_old + dx;
    //    a_new = getAccel(stiffness, x_new);
    //    v_new = v_old + 0.5 * (a_old + a_new) * dt;% - damping * (a_old + a_new) * dt;
    //    v_new = v_new - 2*(damping * v_new) ;
   	
   	for (int i=0; i<N_MASSES; i++) {
   		float dx = _v[i] + 0.5 * _a[i];
   		_x[i] = _x[i] + dx;
   	}
   	
   	float a_new[N_MASSES];
   	//_stiffM.mat_multiply(_x, a_new);
   	getStringAcceleration(_x, a_new);
   	//a_new[0] = -_stiffness*_x[0];
   	for (int i=0; i<N_MASSES; i++) {
   		float dv = 0.5 * (_a[i] + a_new[i]);
   		if(i==input_node) {
   			dv += input_velocity;
   		}
   		_v[i] += dv;
   		_a[i] = a_new[i];
   		_v[i] += -2*(_friction * _v[i]);
   	}
   	
   	//*s->v_data() += 0.5 * (*s->a_data() + _aNew) * _dt;
   	//*s->a_data() = _aNew;
   	//*s->v_data() += - 2 * (*co.get_damping_matrix().data() * *s->v_data());
   
   	return _x[output_node];
}

void CoupledOscillators::getStringAcceleration(float x[N_MASSES], float (&a)[N_MASSES]) {
	float stiffness = 0.05;
	if(N_MASSES==1) {
		a[0] = -stiffness * x[0];
		return;
	}
	a[0] = -stiffness * (x[0]-x[1]);
	for (int i=1; i<N_MASSES-1; i++){
		a[i] = -stiffness * (-x[i-1]+2*x[i]-x[i+1]);
	}
	a[N_MASSES-1] = -stiffness * (x[N_MASSES-1]-x[N_MASSES-2]);
}