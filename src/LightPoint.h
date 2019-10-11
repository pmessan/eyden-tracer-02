#pragma once

#include "ILight.h"

/**
 * @brief Point light source class
 */
class CLightPoint : public ILight
{
public:
	/**
	 * @brief Constructor
	 * @param position The position (origin) of the light source
	 * @param intensity The emission color and strength of the light source
	 */
	CLightPoint(Vec3f intensity, Vec3f position)
		: m_intensity(intensity)
		, m_position(position)
	{} 
	virtual ~CLightPoint(void) = default;

	virtual std::optional<Vec3f> Illuminate(Ray& ray) override
	{
		// --- PUT YOUR CODE HERE ---
		cv::normalize(m_position - ray.org, ray.dir);
		ray.t = cv::norm(m_position - ray.org);
		
		return 1/pow((norm(m_position - ray.org)), 1) * m_intensity;
	}


private:
	Vec3f m_intensity; ///< emission (red, green, blue)
	Vec3f m_position;  ///< The light source origin
};