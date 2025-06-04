#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Ocean;

/// Базовый абстрактный организм
class Entity
{
public:
    virtual ~Entity() = default;

    /// логика за один «тик»
    virtual void update(Ocean& ocean, int x, int y) = 0;

    /// отрисовка; cell — размер клетки, x/y — координаты в сетке
    virtual void draw(sf::RenderTarget& target,
                      float cell, float x, float y) const = 0;
};

#endif
