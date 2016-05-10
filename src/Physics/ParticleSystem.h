#pragma once

#include "../Maths/VectorMaths.h"
#include <vector>
#include <math.h>

template<typename UserInfo>
class ParticleSystem;

template<typename UserInfo>
struct ParticleHandle {
    ParticleHandle() { }

    ParticleHandle(size_t particleId, ParticleSystem<UserInfo> *system) {
        this->particleId = particleId;
        this->system = system;
    }

    Vector3f GetPos();

    Vector3f GetVelocity();

    float GetRadius();

    Vector3f GetAcceleration();

    void SetAcceleration(Vector3f acceleration);

    void SetRadius(float radius);

    void SetVelocity(Vector3f velocity);

    bool IsFixed();

    void Release();

    UserInfo GetUserInfo();

    size_t GetParticleIndex();

private:
    size_t particleId;
    ParticleSystem<UserInfo> *system;
};

template<typename UserInfo>
class CollisionProcessor {
    virtual void ProcessCollision(ParticleHandle<UserInfo> p0, ParticleHandle<UserInfo> p1) = 0;
};

template<typename UserInfo>
class ParticleSystem {
public:
    virtual void SetBounds(Vector3f minPoint, Vector3f maxPoint) = 0;

    virtual void Update() = 0;

    virtual size_t AddLink(ParticleHandle<UserInfo> particleHandle0, ParticleHandle<UserInfo> particleHandle1,
                           float stiffness = 0.5f, float stretch = 1.0f) = 0;

    virtual ParticleHandle<UserInfo> AddParticle(Vector3f pos, float radius = 20.0f, bool isFixed = false) = 0;

    //virtual size_t GetParticlesCount() = 0;
    virtual Vector3f GetParticlePos(size_t particleId) = 0;

    virtual Vector3f GetParticleVelocity(size_t particleId) = 0;

    virtual float GetParticleRadius(size_t particleId) = 0;

    virtual Vector3f GetParticleAcceleration(size_t particleId) = 0;

    virtual void SetParticleAcceleration(size_t particleId, Vector3f acceleration) = 0;

    virtual void SetParticleRadius(size_t particleId, float radius) = 0;

    virtual void SetParticleVelocity(size_t particleId, Vector3f velocity) = 0;

    virtual bool IsParticleFixed(size_t particleId) = 0;

    virtual UserInfo GetParticleUserInfo(size_t particleId) = 0;

    virtual void ReleaseParticle(size_t particleId) = 0;

    virtual size_t GetParticlesCount() = 0;

    virtual ParticleHandle<UserInfo> GetParticle(size_t particleIndex) = 0;
};

#include "ParticleSystem.inl"