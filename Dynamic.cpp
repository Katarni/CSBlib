//
// Created by Тимур Ахметзянов on 20.10.2023.
//

#include "Dynamic.h"

void Dynamic::updateParticles(long double d_t) {
  std::vector<Vector3> forces(particles_.size());
  for (int i = 0; i < particles_.size(); ++i) {
    for (int j = 0; j < particles_.size(); ++j) {
      if (i == j) continue;

      // рассчитываем силу действия каждой частицы
    }

    // считаем сумму сил
  }

  for (int i = 0; i < particles_.size(); ++i) {
    // рассчитываем скорость
  }

  for (int i = 0; i < particles_.size(); ++i) {
    // пересчитываем позицию
  }
}
