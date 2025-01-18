#include "config.h"
#include "control.h"

#include <math.h>

#define RAD_TO_DEG(x) (x * 180.0f / (float)M_PI)

typedef struct _ControlDebug_
{
	int32_t CurrentPosition;
	int16_t CurrentAngle;
	int16_t TargetAngle;
} ControlDebug;

void Control_Debug(int32_t currPos, float currAngle, float targetAngle)
{
	ControlDebug data;

	data.CurrentPosition = currPos;
	data.CurrentAngle    = (int16_t)(RAD_TO_DEG(currAngle) * 100.0f);
	data.TargetAngle     = (int16_t)(RAD_TO_DEG(targetAngle) * 100.0f);

	Config_SendDebug(&data, sizeof(data));
}
