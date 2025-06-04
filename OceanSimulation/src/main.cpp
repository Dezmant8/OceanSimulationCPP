#include "ocean.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <cstring>

int main()
{
    constexpr int kW = 120, kH = 80;
    const sf::Vector2u winSz{ static_cast<unsigned>(kW * CELL),
                              static_cast<unsigned>(kH * CELL) };

    const char* ru = u8"Живой океан";
    sf::String baseTitle = sf::String::fromUtf8(ru, ru + std::strlen(ru));

#if SFML_VERSION_MAJOR >= 3
    sf::RenderWindow window(sf::VideoMode(winSz), baseTitle);
#else
    sf::RenderWindow window(sf::VideoMode(winSz.x, winSz.y), baseTitle);
#endif

    Ocean ocean(kW, kH);
    bool paused = false;
    sf::Clock simClock, titleClock;
    const sf::Time step  = sf::milliseconds(TICK_MS);
    const sf::Time titlI = sf::seconds(1);

    auto updateTitle = [&]{
        Population p = ocean.population();
        std::string txt = baseTitle.toAnsiString() +
            " | A:" + std::to_string(p.algae) +
            " H:" + std::to_string(p.herbivores) +
            " P:" + std::to_string(p.predators);
        window.setTitle(sf::String(txt));
    };
    updateTitle();

    while (window.isOpen())
    {
#if SFML_VERSION_MAJOR >= 3           // ─── SFML-3 события
        while (auto e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>()) window.close();

            if (auto k = e->getIf<sf::Event::KeyPressed>())
            {
                using K = sf::Keyboard::Key;
                switch (k->code) {
                    case K::Escape: window.close(); break;
                    case K::Space:  paused = !paused; break;
                    case K::R:      ocean = Ocean(kW, kH); updateTitle(); break;
                    default: break;
                }
            }
        }
#else                                   // ─── SFML-2 события
        sf::Event e{};
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::KeyPressed)
                switch (e.key.code) {
                    case sf::Keyboard::Escape: window.close(); break;
                    case sf::Keyboard::Space:  paused = !paused; break;
                    case sf::Keyboard::R:      ocean = Ocean(kW, kH); updateTitle(); break;
                    default: break;
                }
        }
#endif
        if (!paused && simClock.getElapsedTime() >= step) {
            ocean.update();
            simClock.restart();
        }
        if (titleClock.getElapsedTime() >= titlI) {
            updateTitle();
            titleClock.restart();
        }

        window.clear({10, 10, 40});
        ocean.draw(window);
        window.display();
    }
    return 0;
}
