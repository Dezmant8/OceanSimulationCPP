#include "entities/predator.hpp"
#include "entities/herbivore.hpp"
#include "entities/algae.hpp"
#include "ocean.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"

#include <array>
#include <algorithm>
#include <vector>

namespace {
    const std::array<std::pair<int,int>,4> dirs{{{0,-1},{-1,0},{1,0},{0,1}}};
}

static bool      isHerb (const Ocean& oc,int x,int y){ return dynamic_cast<Herbivore*>(oc.entityAt(x,y)); }
static Predator* getPred(const Ocean& oc,int x,int y){ return dynamic_cast<Predator* >(oc.entityAt(x,y)); }
static bool      isAlgae(const Ocean& oc,int x,int y){ return dynamic_cast<Algae*   >(oc.entityAt(x,y)); }

// глубже этой строки хищники не заплывают
inline int fishDepthLimit(const Ocean& oc){ return oc.height() - 5; }

void Predator::update(Ocean& oc,int x,int y)
{
    ++age_;
    ++hunger_;

    bool veryHungry = (hunger_ >= HUNGRY_PRED);
    if(mateReady_ && veryHungry) mateReady_ = false;     // при сильном голоде никакой романтики, приоритеты)

    // смерть от старости при перенаселении
    if(age_ >= AGE_LIMIT && oc.population().predators >= MAX_POP) {
        oc.clearCell(x,y);
        return;
    }

    // поиск целей

    // жертва
    int tx=-1,ty=-1,minD=99;
    for(int yy=0; yy < fishDepthLimit(oc); ++yy)
        for(int xx=0; xx < oc.width(); ++xx)
            if(isHerb(oc,xx,yy)){
                int d = std::abs(xx-x)+std::abs(yy-y);
                if(d < minD){ minD=d; tx=xx; ty=yy; }
            }

    // партнёр (если готов)
    int mx=-1,my=-1;
    if(mateReady_)
        for(int yy=0; yy < fishDepthLimit(oc); ++yy)
            for(int xx=0; xx < oc.width(); ++xx)
                if(auto* p=getPred(oc,xx,yy))
                    if(p!=this && p->mateReady_){
                        int d = std::abs(xx-x)+std::abs(yy-y);
                        if(d < minD){ minD=d; mx=xx; my=yy; }
                    }

    // выбор направления

    int dir;
    if(tx>=0 && veryHungry)
        dir = (std::abs(tx-x) > std::abs(ty-y)) ? ((tx<x)?1:2) : ((ty<y)?0:3);
    else if(mx>=0)                               // ищем пару
        dir = (std::abs(mx-x) > std::abs(my-y)) ? ((mx<x)?1:2) : ((my<y)?0:3);
    else {                                       // случайный дрейф
        std::array<int,4> rnd{0,1,2,3};
        std::shuffle(rnd.begin(),rnd.end(),Rng::instance().engine());
        dir = rnd[0];
    }

    int nx = x + dirs[dir].first,
        ny = y + dirs[dir].second;

    // фильтр допустимости клетки 

    if (nx < 0 || ny < 0 || nx >= oc.width()) return;
    if (ny >= fishDepthLimit(oc)) return;                       // глубже нельзя
    if (!oc.isEmpty(nx,ny) && !isAlgae(oc,nx,ny) && !isHerb(oc,nx,ny)) return; // занята хищником
    if (!veryHungry && isHerb(oc,nx,ny)) return;                // сытый не нападает

    // действие в клетке

    bool ate = false;
    if (veryHungry && isHerb(oc,nx,ny)) {                       // охота
        oc.clearCell(nx,ny);
        ate = true;
        hunger_ = 0;
        mateReady_ = true;                                      // сытый => готов к спариванию
    }
    if (isAlgae(oc,nx,ny)) oc.clearCell(nx,ny);                 // водоросль просто убираем

    oc.moveEntity(x,y,nx,ny);

    // спаривание

    if(mateReady_)
    {
        const std::array<std::pair<int,int>,4> nbr{{{0,-1},{-1,0},{1,0},{0,1}}};
        for(int d=0; d<4; ++d)
        {
            int ax=x+nbr[d].first, ay=y+nbr[d].second;
            if(auto* p=getPred(oc,ax,ay); p && p->mateReady_)
            {
                // свободные клетки вокруг пары
                std::vector<std::pair<int,int>> spots;
                for(auto [dx,dy]:nbr){
                    int sx=ax+dx, sy=ay+dy;
                    if(oc.isEmpty(sx,sy) && sy < fishDepthLimit(oc)) spots.push_back({sx,sy});
                    sx=x+dx; sy=y+dy;
                    if(oc.isEmpty(sx,sy) && sy < fishDepthLimit(oc)) spots.push_back({sx,sy});
                }
                if(!spots.empty() && Rng::instance().chance(BIRTH_PRED)){
                    std::shuffle(spots.begin(),spots.end(),Rng::instance().engine());
                    oc.spawn<Predator>(spots.front().first, spots.front().second);
                }
                mateReady_ = p->mateReady_ = false;
                break;
            }
        }
    }
}

void Predator::draw(sf::RenderTarget& tgt,float c,float gx,float gy) const
{
    sf::ConvexShape tri(3);
    tri.setPoint(0,{0,0});
    tri.setPoint(1,{c,c*0.5f});
    tri.setPoint(2,{0,c});
    tri.setFillColor(sf::Color(200,40,40));
    tri.setPosition({c*gx,c*gy});
    tgt.draw(tri);
}
