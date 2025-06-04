#ifndef ALGAE_HPP
#define ALGAE_HPP

#include "entity.hpp"

class Algae : public Entity
{
public:
    void update(class Ocean& ocean, int x, int y) override;
    void draw(sf::RenderTarget& tgt,
              float cell, float gx, float gy) const override;
};

#endif
