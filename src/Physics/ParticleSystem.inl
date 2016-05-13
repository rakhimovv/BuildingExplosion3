#include "ParticleSystem.h"

template<typename UserInfo>
Vector3f ParticleHandle<UserInfo>::GetPos() {
    return system->GetParticlePos(particleId);
}

template<typename UserInfo>
Vector3f ParticleHandle<UserInfo>::GetVelocity() {
    return system->GetParticleVelocity(particleId);
}

template<typename UserInfo>
float ParticleHandle<UserInfo>::GetRadius() {
    return system->GetParticleRadius(particleId);
}

template<typename UserInfo>
Vector3f ParticleHandle<UserInfo>::GetAcceleration() {
    return system->GetParticleAcceleration(particleId);
}

template<typename UserInfo>
void ParticleHandle<UserInfo>::SetAcceleration(Vector3f acceleration) {
    system->SetParticleAcceleration(particleId, acceleration);
}

template<typename UserInfo>
inline void ParticleHandle<UserInfo>::SetRadius(float radius) {
    system->SetParticleRadius(particleId, radius);
}

template<typename UserInfo>
inline void ParticleHandle<UserInfo>::SetVelocity(Vector3f velocity) {
    system->SetParticleVelocity(particleId, velocity);
}

template<typename UserInfo>
bool ParticleHandle<UserInfo>::IsFixed() {
    return system->IsParticleFixed(particleId);
}

template<typename UserInfo>
UserInfo ParticleHandle<UserInfo>::GetUserInfo() {
    return system->GetParticleUserInfo(particleId);
}

template<typename UserInfo>
void ParticleHandle<UserInfo>::Release() {
    return system->ReleaseParticle(particleId);
}

template<typename UserInfo>
size_t ParticleHandle<UserInfo>::GetParticleIndex() {
    return particleId;
}

