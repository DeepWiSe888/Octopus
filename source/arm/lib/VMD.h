#pragma once
//#pragma warning(disable: 4267)
//#pragma warning(disable: 4244)
//#pragma warning(disable: 26451)
//#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

// --- VMD function ---
// - input -
// data[len]   origin waveform
// - output -
// outBreath[len]     rpm waveform
// outHeart[len]      bpm waveform
int VMD_vital(float* data, int len, float* outBreath, float* outHeart);

void testVMD();

#ifdef __cplusplus
}
#endif
