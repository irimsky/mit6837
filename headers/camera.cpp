#include "camera.hpp"

Ray OrthographicCamera::generateRay(Vec2f point)
{
    assert(point.x() >= 0 && point.x() <= 1 && point.y() >= 0 && point.y() <= 1);
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    horizon.Normalize();
    Vec3f start = center + (point.x() - 0.5) * horizon * size + (point.y() - 0.5) * up * size;
    Ray ray(start, direction);
    // std::cout << ray << std::endl;
    return ray;
}
