#include "detail/types.h"

vx_status fitLine2D(const _vx_array x, const _vx_array y, float& k, float& b) {
	if (x.size != y.size || x.size == 0)
		return VX_ERROR_INVALID_PARAMETERS;

	int N = x.size; // size of vectors
	float Sx = 0, Sy = 0; // Sx = Sum of elements in x arr, Sy = ... in y array
	float xy = 0, xx = 0; // xy = <x,y>; xx = <x,x>
	for (int i = 0; i < N; i++) {
		Sx += x.data[i];
		Sy += y.data[i];
		xy += x.data[i] * y.data[i];
		xx += x.data[i] * x.data[i];
	}

	k = (N * xy - Sx * Sy) / (N * xx - Sx * Sx);
	b = (Sy - k * Sx) / N;

	return VX_SUCCESS;
}