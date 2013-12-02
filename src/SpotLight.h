#include <CGFlight.h>
#include "Light.h"

class SpotLight : public Light{
	float angle, exp;
	float direction[3];
	float id;
public:
	SpotLight(float angle, float exp, std::string id, bool doub, bool loc, bool enab, float* ambi, float *amb, float *spec, float *diff, bool enable, unsigned int lightid, float *pos, float *dir);
	~SpotLight();
	float getAngle();
	float* getDirection();
	float getExponent();
	void setVariables();
};
