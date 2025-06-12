#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr float CELL     = 8.f;
constexpr int   FIELD_W  = 120;
constexpr int   FIELD_H  = 80;
constexpr int   MAX_ALGAE = 20;

constexpr int TICK_MS           = 200;
constexpr int ALGAE_GROW_CHANCE = 10;

constexpr int HUNGRY_HERB = 20;
constexpr int HUNGRY_PRED = 25;

constexpr int BIRTH_HERB = 30;   // %
constexpr int BIRTH_PRED = 15;   // %

constexpr int AGE_LIMIT = 200;   // тиков до старости
constexpr int MAX_POP   = 10;    // порог популяции для старения

#endif
