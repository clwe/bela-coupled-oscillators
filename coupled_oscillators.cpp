#include "coupled_oscillators.hpp"
#include "stiffness_matrix.hpp"
using namespace std;


CoupledOscillators::CoupledOscillators(COConfig conf):_conf(conf) {
	
	
	switch (_conf.type) {
		case PLATE:
			_n_masses = _conf.x*_conf.y;
			_stiff_M.setSize(_n_masses);
			_stiff_M.buildUpPlate(_conf.x,_conf.y, _conf.stiffness);
			break;
		case STRING:
			_n_masses = _conf.len;
			_stiff_M.setSize(_n_masses);
			_stiff_M.buildUpString(_conf.stiffness);
			break;
	}
	
	for (int i=0; i<_n_masses; i++){
		_x.push_back(0);
		_v.push_back(0);
		_a.push_back(0);
		_a_new.push_back(0);
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
   	
   	for (int i=0; i<_n_masses; i++) {
   		float dx = _v[i] + 0.5 * _a[i];
   		_x[i] += dx;
   		if(i==input_node) {
   			_x[i] += simpleHighPass(in);
   		}
   	}
   	
   	if(_conf.type == STRING) {
   		getStringAcceleration(_x, _a_new);
   	}
   	else {
   		_stiff_M.matMultiply(_x, _a_new);
   	}
   	
   	for (int i=0; i<_n_masses; i++) {
   		float dv = 0.5 * (_a[i] + _a_new[i]);
   		_v[i] += dv;
   		_a[i] = _a_new[i];
   		_v[i] += -2*(_conf.damping * _v[i]);
   	}
   
   	return _x[output_node];
}

void CoupledOscillators::getStringAcceleration(vector<float> x, vector<float> &a) {
	if(_n_masses==1) {
		a[0] = -_conf.stiffness * x[0];
		return;
	}
	a[0] = -_conf.stiffness * (x[0]-x[1]);
	
	for (int i=1; i<_n_masses-1; i++){
		a[i] = -_conf.stiffness * (-x[i-1]+2*x[i]-x[i+1]);
	}
	a[_n_masses-1] = -_conf.stiffness * (x[_n_masses-1]-x[_n_masses-2]);
}