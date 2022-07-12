#include "raytracer.hpp"
#include "rayTree.hpp"
#include "../headers/light.hpp"

#define EPSILON 0.0001

Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
{
    Vec3f normal_t = normal, incoming_t = incoming;
    normal_t.Normalize();
    incoming_t.Normalize();
    float cosTheta = incoming_t.Dot3(normal_t);
    return incoming_t - 2 * cosTheta * normal_t;
}

bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
                          float index_i, float index_t, Vec3f &transmitted)
{
    Vec3f normal_t = normal, incoming_t = incoming;
    normal_t.Normalize();
    incoming_t.Normalize();
    float theta = incoming_t.Dot3(normal_t);
    Vec3f mid = -theta * normal_t + incoming_t; // 入射向量的水平分向量
    float inSin = mid.Length();
    float outSin = inSin * index_i / index_t;

    if (abs(outSin) > 1) // 全反射
        return false;

    float outCos = sqrt(1 - outSin * outSin);
    mid.Normalize();
    if (theta < 0) // 外部
        transmitted = -outCos * normal_t + outSin * mid;
    else
        transmitted = outCos * normal_t + outSin * mid;

    return true;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight,
                          float indexOfRefraction, Hit &hit) const
{
    if (bounces > max_bounce || weight < cutoff_weight)
    {
        return Vec3f(0, 0, 0);
    }

    Group *group = s->getGroup();
    if (group->intersect(ray, hit, tmin))
    {
        if (bounces == 0)
        {
            RayTree::SetMainSegment(ray, 0, hit.getT());
        }
        Vec3f diffuse = hit.getMaterial()->getDiffuseColor();
        Vec3f sumColor = Vec3f(0, 0, 0);
        sumColor += diffuse * s->getAmbientLight();
        for (int i = 0; i < s->getNumLights(); ++i)
        {
            Light *light = s->getLight(i);
            Vec3f p = hit.getIntersectionPoint();
            Vec3f dirToLight, lightColor;
            float distance;
            light->getIllumination(p, dirToLight, lightColor, distance);

            Vec3f shadowMask(1, 1, 1);
            if (shadow)
            {
                Hit directLightHit;
                Ray directLightRay(p, dirToLight);
                if (group->intersect(directLightRay, directLightHit, EPSILON))
                {
                    if (directLightHit.getT() < distance)
                    {
                        RayTree::AddShadowSegment(directLightRay, 0, directLightHit.getT());
                    }
                    else
                    {
                        RayTree::AddShadowSegment(directLightRay, 0, distance);
                    }

                    // 半透明阴影
                    if(directLightHit.getMaterial()->getTransparentColor().Length() > 0)
                    {
                        float lastT = -1;
                        Vec3f lastColor = Vec3f(-1, -1, -1);
                        Hit transparentHit(directLightHit);
                        float lastHitT;
                        bool isTransmitted = true;
                        do
                        {
                            if(transparentHit.getMaterial()->getTransparentColor().Length() <= 0)
                            {
                                isTransmitted = false;
                                break;
                            }
                            RayTree::AddTransmittedSegment(directLightRay, lastHitT, transparentHit.getT());
                            lastHitT = transparentHit.getT();
                            if (lastColor == transparentHit.getMaterial()->getTransparentColor())
                            {
                                Vec3f mask_temp = Vec3f(1, 1, 1) * (1 - (lastHitT - lastT)) + directLightHit.getMaterial()->getTransparentColor() * (lastHitT - lastT);
                                Vec3f::Mult(shadowMask, shadowMask, mask_temp);
                            }

                            lastT = lastHitT;
                            lastColor = transparentHit.getMaterial()->getTransparentColor();
                            transparentHit.setInfT();
                        } while (group->intersect(directLightRay, transparentHit, lastHitT + EPSILON));
                        
                        if(isTransmitted){
                            RayTree::AddShadowSegment(directLightRay, lastHitT, 100);
                        }
                        else{
                            RayTree::AddShadowSegment(directLightRay, lastHitT, transparentHit.getT());
                            shadowMask = Vec3f(0, 0, 0);
                        }
                    }
                    else
                        shadowMask = Vec3f(0, 0, 0);
                }
                else
                {
                    RayTree::AddShadowSegment(directLightRay, 0, 100);
                }
            }
            sumColor += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor) * shadowMask; // calDirectionalLight(h, light);
        }

        // 反射
        if (hit.getMaterial()->getReflectiveColor().Length() > 0 && bounces < max_bounce)
        {
            Ray reflectRay(hit.getIntersectionPoint(), mirrorDirection(hit.getNormal(), ray.getDirection()));
            Hit reflectHit;
            Vec3f reflectColor = traceRay(reflectRay, EPSILON, bounces + 1,
                                          weight * hit.getMaterial()->getReflectiveColor().Length(), indexOfRefraction, reflectHit);

            sumColor += reflectColor * hit.getMaterial()->getReflectiveColor() * weight;
            // cout << reflectColor << ' ' << hit.getMaterial()->getReflectiveColor() << ' ' << weight << endl;
            RayTree::AddReflectedSegment(reflectRay, 0, reflectHit.getT());
        }

        // 折射
        if (hit.getMaterial()->getTransparentColor().Length() && bounces < max_bounce)
        {
            Hit transparentHit;
            Vec3f transparentDir;
            bool inside = hit.getNormal().Dot3(ray.getDirection()) > 0;
            bool check = false;
            if (inside)
                check = transmittedDirection(hit.getNormal(), ray.getDirection(),
                                             indexOfRefraction, 1.0, transparentDir);
            else
                check = transmittedDirection(hit.getNormal(), ray.getDirection(),
                                             1.0, hit.getMaterial()->getIndexOfRefraction(), transparentDir);

            Ray transRay(hit.getIntersectionPoint(), transparentDir);
            if (check)
            {
                if (inside)
                    sumColor += traceRay(transRay, EPSILON, bounces + 1,
                                         weight * hit.getMaterial()->getTransparentColor().Length(),
                                         1.0, transparentHit) *
                                hit.getMaterial()->getTransparentColor() * weight;
                else
                    sumColor += traceRay(transRay, EPSILON, bounces + 1,
                                         weight * hit.getMaterial()->getTransparentColor().Length(),
                                         hit.getMaterial()->getIndexOfRefraction(), transparentHit) *
                                hit.getMaterial()->getTransparentColor() * weight;

                RayTree::AddTransmittedSegment(transRay, 0, transparentHit.getT());
            }
        }

        return sumColor;
    }
    else if (bounces == 0)
    {
        RayTree::SetMainSegment(ray, 0, 100);
    }

    return s->getBackgroundColor();
}