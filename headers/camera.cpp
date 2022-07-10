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

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    assert(point.x() >= 0 && point.x() <= 1 && point.y() >= 0 && point.y() <= 1);
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    horizon.Normalize();

    Vec3f temp = direction + 2 * tan(fov / 2) * up * (point.x() - 0.5) + 2 * tan(fov / 2) * horizon * (point.y() - 0.5);
    temp.Normalize();
    std::cout << temp << std::endl;
    return Ray(center, temp);
}