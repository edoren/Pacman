#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <SFML/System/Time.hpp>

class Clock {
 public:
    explicit Clock(bool running = true);

    sf::Time getElapsedTime() const;
    void stop();

    void pause();
    void resume();

    void restart();

 private:
    // return actual time since epoch
    sf::Time getActualTime() const;

 private:
    bool running_;
    sf::Time startTime_;
    sf::Time pauseTime_;
    sf::Time timeBuffer_;

};

#endif  // CLOCK_HPP
