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
			_mod_stiffness = _conf.stiffness;
			_stiff_M.buildUpString(_conf.stiffness);
			break;
	}
	_x.resize(_n_masses);
	_v.resize(_n_masses);
	_a.resize(_n_masses);
	_a_new.resize(_n_masses);
};

void CoupledOscillators::modulateGlobalStiffness(float modulation) {
	_stiff_M.modulateGlobalStiffness(modulation);
	_mod_stiffness = _conf.stiffness*(1+modulation);
}

float CoupledOscillators::simpleHighPass(float in) {
	float hp = in-_in_old;
	_in_old = in;
	return hp;
}