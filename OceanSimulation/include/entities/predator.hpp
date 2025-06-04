#ifndef PREDATOR_HPP
#define PREDATOR_HPP
#include "entity.hpp"
#include "constants.hpp"

class Predator : public Entity
{
public:
    Predator() = default;
    void update(class Ocean& oc,int x,int y) override;
    void draw (sf::RenderTarget&,float,float,float) const override;
private:
    int  hunger_    = 0;
    int  age_       = 0;
    bool mateReady_ = false;
};
#endif
