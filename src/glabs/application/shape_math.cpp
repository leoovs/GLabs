#include "glabs/application/shape_math.hpp"

namespace glabs
{
	std::vector<Vector2f> CalculateNGonCorners(int32_t n, float radius, Vector2f center)
	{
		std::vector<Vector2f> ngon(n);

		float startAngle = cPi / 2.0f;
		float angleStep = 2 * cPi / static_cast<float>(n);

		for (int32_t icorner = 0; icorner < n; icorner++)
		{
			float angle = icorner * angleStep + startAngle;

			ngon.at(icorner) = Vector2f
			{
				center.X + radius * cosf(angle),
				center.Y + radius * sinf(angle),
			};
		}

		return ngon;
	}
}

