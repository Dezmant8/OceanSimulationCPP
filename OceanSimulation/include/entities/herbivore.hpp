#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP
#include "entity.hpp"
#include "constants.hpp"

class Herbivore : public Entity
{
public:
    Herbivore() = default;                 // hunger_=0, age_=0 по умолчанию
    void update(class Ocean& oc,int x,int y) override;
    void draw (sf::RenderTarget&,float,float,float) const override;
private:
    int hunger_ = 0;
    int age_    = 0;
};
#endif
