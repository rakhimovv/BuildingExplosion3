#include "AosParticleSystem.h"
#include <iostream>

template<typename UserInfo>
void AosParticle<UserInfo>::Integrate(float dt) {
    Vector3f tmp = pos;
    pos = pos + (pos - prevPos) + acceleration * dt * dt;
    prevPos = tmp;
}

template<typename UserInfo>
AosLink<UserInfo>::AosLink(size_t particleId0, size_t particleId1, float condConst, float stiffness, float stretch,
                           AosParticleSystem<UserInfo> *sys) {
    this->particleId0 = particleId0;
    this->particleId1 = particleId1;
    this->stiffness = stiffness;
    this->defLength =
            (sys->GetParticleById(particleId0).pos - sys->GetParticleById(particleId1).pos).Length() * stretch;
    this->exists = true;
    this->condConst = condConst;
}

template<typename UserInfo>
void AosLink<UserInfo>::Solve(AosParticleSystem<UserInfo> *sys) {
    AosParticle<UserInfo> *p0 = &(sys->GetParticleById(particleId0));
    AosParticle<UserInfo> *p1 = &(sys->GetParticleById(particleId1));
    Vector3f delta = p1->pos - p0->pos;
    //std::cout << "delta: " << delta.Length() << "\n";
    Vector3f dir = delta * (1.0f / delta.Length());
    //std::cout << "delta - def: " << (delta.Length() - defLength) / defLength << "\n";

    double cond = (delta.Length() - defLength);// / defLength;

    cond = cond > 0.0 ? cond : -cond;

    //std::cout << "cond: " << cond << "\n";


    // 0.005 0.2
    // x stiffness


    // 0.0015

    if (exists && cond > condConst * stiffness) {
        std::cout << "UPSS! :)\n";
        exists = false;
    }


    if (exists) {
        if (!p0->isFixed) {
            p0->pos = p0->pos + dir *
                                (delta.Length() - defLength) * 0.5f * stiffness;
        }
        if (!p1->isFixed) {
            p1->pos = p1->pos + dir *
                                (delta.Length() - defLength) * -0.5f * stiffness;
        }
    }
}

template<typename UserInfo>
AosParticleSystem<UserInfo>::AosParticleSystem(Vector3f minPoint, Vector3f maxPoint, float dt) {
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;

    this->dt = dt;
}

template<typename UserInfo>
void AosParticleSystem<UserInfo>::Update() {
    particles.Update();

    for (size_t particleIndex = 0; particleIndex < particles.GetElementsCount(); particleIndex++) {
        size_t particleId = particles.GetId(particleIndex);
        AosParticle<UserInfo> &currParticle = particles.GetById(particleId);

        currParticle.Integrate(this->dt);

        if (currParticle.isFixed) {
            currParticle.acceleration = Vector3f(0.0f, 0.0f, 0.0f);
        } else {
            //currParticle.acceleration = Vector3f(0.0f, 20.0f, 0.0f);
        }

        if (currParticle.pos.y - currParticle.radius < minPoint.y) {

            //std::cout << currParticle.pos.y << "\n";
            //std::cout << "URAAA\n";

            currParticle.pos.y = minPoint.y + currParticle.radius;
            currParticle.prevPos.x = currParticle.pos.x;
            currParticle.prevPos.z = currParticle.pos.z;

        }
        /*
        if (currParticle.pos.y + currParticle.radius > maxPoint.y) {

            currParticle.pos.y = maxPoint.y - currParticle.radius;
            currParticle.prevPos.x = currParticle.pos.x;
            currParticle.prevPos.z = currParticle.pos.z;

        }
        if (currParticle.pos.x + currParticle.radius > maxPoint.x) {

            currParticle.pos.x = maxPoint.x - currParticle.radius;
            currParticle.prevPos.y = currParticle.pos.y;
            currParticle.prevPos.z = currParticle.pos.z;

        }
        if (currParticle.pos.x - currParticle.radius < minPoint.x) {

            currParticle.pos.x = minPoint.x + currParticle.radius;
            currParticle.prevPos.y = currParticle.pos.y;
            currParticle.prevPos.z = currParticle.pos.z;

        }*/
    }
    for (size_t linkIndex = 0;
         linkIndex < links.size();
         linkIndex++) {
        links[linkIndex].Solve(this);
    }
}

template<typename UserInfo>
size_t AosParticleSystem<UserInfo>::AddLink(ParticleHandle<UserInfo> particle0, ParticleHandle<UserInfo> particle1,
                                            float condConst,
                                            float stiffness, float stretch) {
    links.push_back(AosLink<UserInfo>(
            particle0.GetParticleIndex(),
            particle1.GetParticleIndex(),
            condConst,
            stiffness,
            stretch,
            this));
    return links.size() - 1;
}

template<typename UserInfo>
ParticleHandle<UserInfo> AosParticleSystem<UserInfo>::AddParticle(Vector3f pos, float radius, bool isFixed) {
    AosParticle<UserInfo> newbie;
    newbie.pos = pos;
    newbie.prevPos = pos;
    newbie.radius = radius;
    newbie.isFixed = isFixed;

    newbie.acceleration = Vector3f(0.0f, 0.0f, 0.0f);

    return ParticleHandle<UserInfo>(particles.Add(newbie), this);
}

template<typename UserInfo>
Vector3f AosParticleSystem<UserInfo>::GetParticlePos(size_t particleId) {
    return particles.GetById(particleId).pos;
}

template<typename UserInfo>
Vector3f AosParticleSystem<UserInfo>::GetParticleVelocity(size_t particleId) {
    return (particles.GetById(particleId).pos - particles.GetById(particleId).prevPos) * (1.0f / dt);
}

template<typename UserInfo>
float AosParticleSystem<UserInfo>::GetParticleRadius(size_t particleId) {
    return particles.GetById(particleId).radius;
}

template<typename UserInfo>
Vector3f AosParticleSystem<UserInfo>::GetParticleAcceleration(size_t particleId) {
    return particles.GetById(particleId).acceleration;
}

template<typename UserInfo>
void AosParticleSystem<UserInfo>::SetParticleAcceleration(size_t particleId, Vector3f acceleration) {
    particles.GetById(particleId).acceleration = acceleration;
}

template<typename UserInfo>
inline void AosParticleSystem<UserInfo>::SetParticleRadius(size_t particleId, float radius) {
    particles.GetById(particleId).radius = radius;
}

template<typename UserInfo>
inline void AosParticleSystem<UserInfo>::SetParticleVelocity(size_t particleId, Vector3f velocity) {
    particles.GetById(particleId).prevPos = particles.GetById(particleId).pos - velocity * dt;
}

template<typename UserInfo>
bool AosParticleSystem<UserInfo>::IsParticleFixed(size_t particleId) {
    return particles.GetById(particleId).isFixed;
}

template<typename UserInfo>
UserInfo AosParticleSystem<UserInfo>::GetParticleUserInfo(size_t particleId) {
    return particles.GetById(particleId).userInfo;
}

template<typename UserInfo>
void AosParticleSystem<UserInfo>::ReleaseParticle(size_t particleId) {
    particles.RemoveById(particleId);
}

template<typename UserInfo>
size_t AosParticleSystem<UserInfo>::GetParticlesCount() {
    return particles.GetElementsCount();
}

template<typename UserInfo>
ParticleHandle<UserInfo> AosParticleSystem<UserInfo>::GetParticle(size_t particleIndex) {
    return ParticleHandle<UserInfo>(particles.GetId(particleIndex), this);
}

template<typename UserInfo>
AosParticle<UserInfo> &AosParticleSystem<UserInfo>::GetParticleById(size_t particleId) {
    return particles.GetById(particleId);
}


template<typename UserInfo>
void AosParticleSystem<UserInfo>::SetBounds(Vector3f minPoint, Vector3f maxPoint) {
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;
}

template<typename UserInfo>
bool AosParticleSystem<UserInfo>::IsLinkExists(size_t id) {
    return links[id].exists;
}