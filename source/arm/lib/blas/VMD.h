/*
 * Project Octopus
 *
 */


#ifndef _OCTOPUS_VMD_H_
#define _OCTOPUS_VMD_H_
//#pragma warning(disable: 4267)
//#pragma warning(disable: 4244)
//#pragma warning(disable: 26451)

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

#endif
