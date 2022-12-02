#include "coupled_oscillators.hpp"
#include "mass_interaction.hpp"
using namespace std;

void CoupledOscillators::addSpring(int i, int j, float stiffness) {
	_mi.addSpring(i, j, stiffness);
}

void CoupledOscillators::addWall(int i, float stiffness) {
	_mi.addWall(i, stiffness);
}

CoupledOscillators::CoupledOscillators(COConfig conf):_conf(conf) {
	_mod_stiffness = 1;
	_mod_stiffness_fac = 1;
	switch (_conf.type) {
		case PLATE:
			_n_masses = _conf.x*_conf.y;
			_mi.buildUpPlate(_conf.x,_conf.y, _conf.stiffness);
			break;
		case STRING:
			_n_masses = _conf.size;
			break;
		case EMPTY:
			_n_masses = _conf.size;
			break;
	}
	_x.resize(_n_masses);
	_v.resize(_n_masses);
	_a.resize(_n_masses);
	_a_new.resize(_n_masses);
};

void CoupledOscillators::modulateGlobalStiffness(float modulation) {
	//_stiff_M.modulateGlobalStiffness(modulation);
	_mod_stiffness_fac = 1+modulation;
	_mod_stiffness = _conf.stiffness*_mod_stiffness_fac;
}

float CoupledOscillators::simpleHighPass(float in) {
	float hp = in-_in_old;
	_in_old = in;
	return hp;
}