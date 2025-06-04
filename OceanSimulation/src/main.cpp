#include "ocean.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <cstring>

int main()
{
    const sf::Vector2u winSz{ static_cast<unsigned>(FIELD_W * CELL),
                              static_cast<unsigned>(FIELD_H * CELL) };

    const char* ru = u8"Живой океан";
    sf::String baseTitle = sf::String::fromUtf8(ru, ru + std::strlen(ru));

#if SFML_VERSION_MAJOR >= 3
    sf::RenderWindow win(sf::VideoMode(winSz), baseTitle);
#else
    sf::RenderWindow win(sf::VideoMode(winSz.x, winSz.y), baseTitle);
#endif

    Ocean ocean(FIELD_W, FIELD_H);
    bool paused = false;
    sf::Clock tickTimer, titleTimer;
    const sf::Time step   = sf::milliseconds(TICK_MS);
    const sf::Time titInt = sf::seconds(1);

    auto updateTitle = [&]{
        auto p = ocean.population();
        std::string s = baseTitle.toAnsiString() +
            " | A:" + std::to_string(p.algae) +
            " H:"   + std::to_string(p.herbivores) +
            " P:"   + std::to_string(p.predators);
        win.setTitle(sf::String(s));
    };
    updateTitle();

    while (win.isOpen())
    {
        #if SFML_VERSION_MAJOR >= 3
        while (auto ev = win.pollEvent())
        {
            if (ev->is<sf::Event::Closed>()) win.close();

            if (auto kp = ev->getIf<sf::Event::KeyPressed>())
            {
                sf::Keyboard::Key  kc = kp->code;      // может быть Unknown
                sf::Keyboard::Scan sc = kp->scancode;  // всегда валиден

                auto match = [&](sf::Keyboard::Key wantK,
                                 sf::Keyboard::Scan wantS)
                {
                    return (kc == wantK && kc != sf::Keyboard::Key::Unknown)
                           || sc == wantS;
                };

                if (match(sf::Keyboard::Key::Escape, sf::Keyboard::Scan::Escape))
                    win.close();
                else if (match(sf::Keyboard::Key::Space,  sf::Keyboard::Scan::Space))
                    paused = !paused;
                else if (match(sf::Keyboard::Key::R,      sf::Keyboard::Scan::R))
                {
                    ocean = Ocean(FIELD_W, FIELD_H);
                    updateTitle();
                }
            }
        }
#else
        sf::Event e{};
        while (win.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) win.close();
            if (e.type == sf::Event::KeyPressed)
                switch (e.key.code) {
                    case sf::Keyboard::Escape: win.close(); break;
                    case sf::Keyboard::Space:  paused = !paused; break;
                    case sf::Keyboard::R:      ocean = Ocean(FIELD_W, FIELD_H); updateTitle(); break;
                    default: break;
                }
        }
#endif
        if (!paused && tickTimer.getElapsedTime() >= step) {
            ocean.update();
            tickTimer.restart();
        }
        if (titleTimer.getElapsedTime() >= titInt) {
            updateTitle();
            titleTimer.restart();
        }

        win.clear({10,10,40});
        ocean.draw(win);
        win.display();
    }
    return 0;
}
