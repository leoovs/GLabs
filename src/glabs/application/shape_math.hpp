#pragma once

namespace glabs
{
	constexpr float cPi = 3.14159265358979323846f;

	struct Vector2f
	{
		struct NoDefaults{} static NoInit;

		float X;
		float Y;

		Vector2f()
			: X(0.0f)
			, Y(0.0f)
		{}

		Vector2f(NoDefaults)
		{}

		Vector2f(float x, float y)
			: X(x)
			, Y(y)
		{}
	};

	// Рассчитывает координаты вершин правильного n-угольника с центром center
	// и радиусом radius.
	std::vector<Vector2f> CalculateNGonCorners(int32_t n, float radius, Vector2f center);
}

