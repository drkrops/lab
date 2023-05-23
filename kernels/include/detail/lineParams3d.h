#ifndef __LINE_PARAMS__
#define __LINE_PARAMS__

// Параметры уравнения прямой в пространстве:
struct _lineParams3d {
	float x0, y0, z0, a, b, c;
};

typedef struct _lineParams3d* lineParams3d;

#endif

