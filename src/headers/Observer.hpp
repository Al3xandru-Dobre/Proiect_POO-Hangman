//
// Created by Alex on 07.01.2025.
//

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

#endif //OBSERVER_HPP
