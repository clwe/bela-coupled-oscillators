#include "coupled_oscillators.hpp"
#include "stiffness_matrix.hpp"
#include "defines.hpp"
using namespace std;


CoupledOscillators::CoupledOscillators(Type t, float stiffness, int x, int y):_type(t),_stiffM(stiffness) {

	for (int i=0; i<N_MASSES; i++){
		_x[i]=0;
		_v[i]=0;
		_a[i]=0;
	}
	_friction  = 0.0001;
	
	switch (_type) {
		case PLATE:
			_stiffM.buildUpPlate(x,y, stiffness);
			break;
		case STRING:
			_stiffM.buildUpString(x, stiffness);
			_string_len = x;
			break;
	}
};

float CoupledOscillators::simpleHighPass(float in) {
	float hp = in-_in_old;
	_in_old = in;
	return hp;
}

float CoupledOscillators::verletStep(float in, int input_node, int output_node) {
	
	//  dx = v_old * dt + 0.5 * a_old * dt^2;% - damping * a_old * dt^2;
    //  x_new = x_old + dx;
    //  a_new = getAccel(stiffness, x_new);
    //  v_new = v_old + 0.5 * (a_old + a_new) * dt;% - damping * (a_old + a_new) * dt;
    //  v_new = v_new - 2*(damping * v_new) ;
   	
   	//	*s->v_data() += 0.5 * (*s->a_data() + _aNew) * _dt;
   	//	*s->a_data() = _aNew;
   	//	*s->v_data() += - 2 * (*co.get_damping_matrix().data() * *s->v_data());
   	
   	for (int i=0; i<N_MASSES; i++) {
   		float dx = _v[i] + 0.5 * _a[i];
   		_x[i] += dx;
   		if(i==input_node) {
   			_x[i] += simpleHighPass(in);
   		}
   	}
   	
   	float a_new[N_MASSES];
   	
   	//_stiffM.mat_multiply(_x, a_new);
   	if(_type == STRING) {
   		getStringAcceleration(_x, a_new, _stiffM.getStiffnessVal());
   	}
   	else {
   		_stiffM.mat_multiply(_x, a_new);
   	}
   	
   	for (int i=0; i<N_MASSES; i++) {
   		float dv = 0.5 * (_a[i] + a_new[i]);
   		_v[i] += dv;
   		_a[i] = a_new[i];
   		_v[i] += -2*(_friction * _v[i]);
   	}
   
   	return _x[output_node];
}

void CoupledOscillators::getStringAcceleration(float x[N_MASSES], float (&a)[N_MASSES], float stiffness) {
	if(_string_len==1) {
		a[0] = -stiffness * x[0];
		return;
	}
	a[0] = -stiffness * (x[0]-x[1]);
	
	for (int i=1; i<_string_len-1; i++){
		a[i] = -stiffness * (-x[i-1]+2*x[i]-x[i+1]);
	}
	a[_string_len-1] = -stiffness * (x[_string_len-1]-x[_string_len-2]);
}