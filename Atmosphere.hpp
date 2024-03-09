#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

#include "RainType.hpp"

class Atmosphere{
    public: 
        //should be private but idc
        Atmosphere(){};
        int baseSpeed = 5;
        double wind = 0.0;
        RainType type = SNOW;
        unsigned int background = 0;
        double angle = 0.0;  
        Atmosphere(const Atmosphere& obj) = delete;
        void arrange(int temperature);
        void updateWind(double wind);
};

#endif