#include <CGFlight.h>

class Light : public CGFlight
{
	static bool doublesided, local, enabled;
	static float ambient[4];
	std::string id;
	float amb[4], spec[4], diff[4];

public:
	Light(std::string id, bool doub, bool loc, bool enab, float* ambi, float *amb, float *spec, float *diff, bool enable, unsigned int lightid, float *pos, float *dir);
	~Light();
	static bool getDoublesided();
	static bool getLocal();
	static bool getEnabled();
	static float* getAmbient();
	std::string getID();
	virtual void setVariables();
	float* getAmb();
	float* getSpecular();
	float* getDiffuse();
};