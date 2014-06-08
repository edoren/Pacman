#include <chrono>

#include "Engine/Utils/Clock.hpp"

Clock::Clock(bool running) :
        running_(running),
        startTime_(this->getActualTime()),
        pauseTime_(this->getActualTime()),
        timeBuffer_(sf::Time::Zero) {}

sf::Time Clock::getElapsedTime() const {
    if (running_)
        return this->getActualTime() - startTime_ - timeBuffer_;
    else
        return pauseTime_ - startTime_ - timeBuffer_;
}

sf::Time Clock::getActualTime() const {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto now_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now);
    return sf::microseconds(now_microseconds.count());
}

void Clock::stop() {
    this->restart();
    running_ = false;
}

void Clock::pause() {
    if (running_) {
        pauseTime_ = this->getActualTime();
        running_ = false;
    }
}

void Clock::resume() {
    if (!running_) {
        timeBuffer_ += this->getActualTime() - pauseTime_;
        running_ = true;
    }
}

void Clock::restart() {
    sf::Time now = this->getActualTime();
    startTime_ = now;
    pauseTime_ = now;
    timeBuffer_ = sf::Time::Zero;
}
