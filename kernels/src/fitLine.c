#include "detail/types.h"

vx_status fitLine2D(const vx_array x, const vx_array y, float* k, float* b) {
	if (x->size != y->size || x->size == 0)
		return VX_ERROR_INVALID_PARAMETERS;

	int N = x->size; // size of vectors
	float Sx = 0, Sy = 0; // Sx = Sum of elements in x arr, Sy = ... in y array
	float xy = 0, xx = 0; // xy = <x,y>; xx = <x,x>

	uint8_t *x_arr = x->data,
			*y_arr = y->data;

	for (int i = 0; i < N; i++) {
		Sx += x_arr[i];
		Sy += y_arr[i];
		xy += x_arr[i] * y_arr[i];
		xx += x_arr[i] * y_arr[i];
	}

	*k = (N * xy - Sx * Sy) / (N * xx - Sx * Sx);
	*b = (Sy - *k * Sx) / N;

	return VX_SUCCESS;
}