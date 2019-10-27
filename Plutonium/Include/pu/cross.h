#pragma once
#ifdef __SWITCH__
#include <switch.h>
#else
#include "types.h"
inline void hidScanInput() {}
inline u64 hidKeysDown(HidControllerID sth) {return 0;}
inline u64 hidKeysUp(HidControllerID sth) {return 0;}
inline u64 hidKeysHeld(HidControllerID sth) {return 0;}
inline void hidTouchRead(touchPosition *pos, u32 point_id) {}
inline u32 hidSixAxisSensorValuesRead(SixAxisSensorValues *values, HidControllerID id, u32 num_entries) {return 0;}
inline bool hidIsControllerConnected(HidControllerID id) {return true;}
inline Result hidStartSixAxisSensor(u32 SixAxisSensorHandle) {return 0;}
inline Result hidStopSixAxisSensor(u32 SixAxisSensorHandle) {return 0;}
inline Result hidGetSixAxisSensorHandles(u32 *SixAxisSensorHandles, s32 total_handles, HidControllerID id, HidControllerType type) {return 0;}
#endif