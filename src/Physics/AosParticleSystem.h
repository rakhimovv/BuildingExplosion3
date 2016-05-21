#pragma once

#include "ParticleSystem.h"
#include <vector>
#include <math.h>
#include "../Utils/CachedArray.h"

template<typename UserInfo>
class AosParticle {
public:
    void Integrate(float dt);

    Vector3f pos;
    Vector3f prevPos;
    Vector3f acceleration;
    float radius;
    UserInfo userInfo;
    bool isFixed;
};

template<typename UserInfo>
class AosParticleSystem;

template<typename UserInfo>
class AosLink {
public:
    AosLink() { }

    AosLink(size_t particleId0, size_t particleId1, float condConst, float stiffness, float stretch,
            AosParticleSystem<UserInfo> *sys);

    void Solve(AosParticleSystem<UserInfo> *sys);

    size_t particleId0;
    size_t particleId1;

    float defLength;
    float stiffness;

    float condConst;

    bool exists;
};

template<typename UserInfo>
class AosParticleSystem : public ParticleSystem<UserInfo> {
public:
    AosParticleSystem(Vector3f minPoint, Vector3f maxPoint, float dt);

    void Update();

    size_t AddLink(ParticleHandle<UserInfo> particle0, ParticleHandle<UserInfo> particle1, float condConst,
                   float stiffness = 0.5f,
                   float stretch = 1.0f);

    ParticleHandle<UserInfo> AddParticle(Vector3f pos, float radius = 20.0f, bool isFixed = false);

    void SetBounds(Vector3f minPoint, Vector3f maxPoint);

    //size_t GetParticlesCount();
    Vector3f GetParticlePos(size_t particleId);

    Vector3f GetParticleVelocity(size_t particleId);

    float GetParticleRadius(size_t particleId);

    Vector3f GetParticleAcceleration(size_t particleId);

    void SetParticleAcceleration(size_t particleId, Vector3f acceleration);

    void SetParticleRadius(size_t particleId, float radius);

    void SetParticleVelocity(size_t particleId, Vector3f velocity);

    bool IsParticleFixed(size_t particleId);

    UserInfo GetParticleUserInfo(size_t particleId);

    void ReleaseParticle(size_t particleId);

    size_t GetParticlesCount();

    ParticleHandle<UserInfo> GetParticle(size_t particleIndex);

    AosParticle<UserInfo> &GetParticleById(size_t particleId);

    bool IsLinkExists(size_t id);

private:
    float dt;
    CachedArray<AosParticle<UserInfo> > particles;
    std::vector<AosLink<UserInfo> > links;
    Vector3f minPoint, maxPoint;
};

#include "AosParticleSystem.inl"