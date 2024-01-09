//
// Created by Тимур Ахметзянов on 20.10.2023.
//

#pragma once

#include "Particle.h"


class Dynamic {
 public:
  Dynamic() = default;
  Dynamic(Vector3 force, const std::vector<Particle> &particles)
          : force_(force), particles_(particles) {
    calculateForce_ = [](Particle par1, Particle par2) { return Vector3(); };
  }
  Dynamic(Vector3 force, const std::vector<Particle> &particles,
          const std::function<Vector3(Particle, Particle)>& calculateForce)
          : force_(force), particles_(particles), calculateForce_(calculateForce) {}

  void setForce(const Vector3& force) {
    force_.setP1(force.getP1());
    force_.setP2(force.getP2());
    force_.setP3(force.getP3());
  }

  void setCalculateForce(const std::function<Vector3(Particle, Particle)> &calculateForce) {
    calculateForce_ = calculateForce;
  }

  const Vector3 &getForce() const {
    return force_;
  }

  const std::vector<Particle> &getParticles() const {
    return particles_;
  }

  const std::function<Vector3(Particle, Particle)> &getCalculateForce() const {
    return calculateForce_;
  }

  Particle& operator[](int i) {
    return particles_[i];
  }

  const size_t size() const {
    return particles_.size();
  }

 protected:
  Vector3 force_;
  std::vector<Particle> particles_;
  std::function<Vector3(Particle, Particle)> calculateForce_;
};