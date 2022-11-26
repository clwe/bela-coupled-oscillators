#include <vector>
using namespace std;

#define N_MASSES 1

class CoupledOscillators
{
public:
	CoupledOscillators(){
		for (int i=0; i<N_MASSES; i++){
			_x_pos[i]= 1;
			_x_pos_old[i]= 1;
		}
		_stiffness =  0.001;
		_friction  = 0.0001;
		_mass = 1;
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
	
	float step(float in) {
		float force_on_x = spring(0, _x_pos[0], _stiffness) + damper(0, 0, _x_pos[0], _x_pos_old[0], _friction)+0.05*in;
		float x_update = massPosUpdate( _x_pos[0], _x_pos_old[0], force_on_x, _mass);
		_x_pos_old[0] = _x_pos[0];
		_x_pos[0]= x_update;
		return x_update;
	}
private:
	float _x_pos[N_MASSES];
	float _x_pos_old[N_MASSES];
	float _friction;
	float _stiffness;
	float _mass;
};