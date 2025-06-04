#include "entities/herbivore.hpp"
#include "entities/algae.hpp"
#include "entities/predator.hpp"
#include "ocean.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"
#include <array>
#include <algorithm>

namespace {
const std::array<std::pair<int,int>,4> dirs{{{0,-1},{-1,0},{1,0},{0,1}}};
auto& rng = Rng::instance().engine();
}
static bool isPred (const Ocean& oc,int x,int y){ return dynamic_cast<Predator* >(oc.entityAt(x,y)); }
static bool isAlgae(const Ocean& oc,int x,int y){ return dynamic_cast<Algae*   >(oc.entityAt(x,y)); }

void Herbivore::update(Ocean& oc,int x,int y)
{
    ++age_;                                 // ▲ старение
    ++hunger_;

    /* смерть от старости при перенаселении */
    if(age_ >= AGE_LIMIT && oc.population().herbivores >= MAX_POP)
    { oc.clearCell(x,y); return; }

    bool hungry = hunger_ >= HUNGRY_HERB;
    int  waterLine = oc.minAlgaeRow()-3;

    /* ---- выбираем ход: максимальный score ---- */
    std::array<int,4> order{0,1,2,3};
    std::shuffle(order.begin(),order.end(),rng);

    int bestDir = order[0], bestScore = -9999;
    for(int d:order){
        int nx=x+dirs[d].first, ny=y+dirs[d].second;
        if(nx<0||ny<0||nx>=oc.width()||ny>=oc.height()) continue;
        if(!hungry && ny>waterLine) continue;              // сытые не ниже
        if(!oc.isEmpty(nx,ny) && !isAlgae(oc,nx,ny)) continue;

        int score = 0;

        /* ++ приоритет: если именно здесь водоросль и мы голодны */
        if(hungry && isAlgae(oc,nx,ny)) score += 500;

        /* избегаем хищников */
        for(int yy=ny-3; yy<=ny+3; ++yy)
            for(int xx=nx-3; xx<=nx+3; ++xx)
                if(isPred(oc,xx,yy))
                    score -= std::max(std::abs(xx-nx), std::abs(yy-ny));

        if(score>bestScore){ bestScore=score; bestDir=d; }
    }

    int nx=x+dirs[bestDir].first, ny=y+dirs[bestDir].second;
    bool ate=false;

    if(isAlgae(oc,nx,ny)){
        bool top = oc.entityAt(nx,ny-1)==nullptr;
        if(hungry || top){
            oc.clearCell(nx,ny);
            oc.shiftColumnDown(nx,ny-1);
            ate=true; hunger_=0;
        }
    }

    if(oc.isEmpty(nx,ny)) oc.moveEntity(x,y,nx,ny);

    /* размножение с 80 % */
    if(ate && Rng::instance().chance(BIRTH_HERB) && oc.isEmpty(x,y))
        oc.spawn<Herbivore>(x,y);           // детёныш с hunger_=0, age_=0
}

void Herbivore::draw(sf::RenderTarget& tgt,float c,float gx,float gy) const
{
    sf::ConvexShape fish(4);
    fish.setPoint(0,{0, c*0.5f}); fish.setPoint(1,{c*0.5f,0});
    fish.setPoint(2,{c, c*0.5f}); fish.setPoint(3,{c*0.5f,c});
    fish.setFillColor(sf::Color(230,230,50));
    fish.setPosition({c*gx,c*gy});
    tgt.draw(fish);
}
