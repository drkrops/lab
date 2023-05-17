/*! @file
 *  @brief Содержит реализацию функции аппроксимации полинома.
 *  @author Ivashkov Stas
 *  @date: 17 May 2023
 */

#include <detail/types.h>
#include <../src/pApproximation.h>

vx_status poly_approximate(const std::vector<point> datas, std::vector<point>& appr_pnts, const float accuracy) {
	try
	{
		approximatePoly approximation = approximatePoly(datas, accuracy);
		approximation.makeApproximation();
		appr_pnts = approximation.getPoints();
	}
	catch (...)
	{
		return VX_ERROR_INVALID_PARAMETERS;
	}
	return VX_SUCCESS;
}