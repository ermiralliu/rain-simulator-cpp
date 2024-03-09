#include "Atmosphere.hpp"
#include <random>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

void Atmosphere::updateWind(double newWind){
    wind = newWind;
    angle = atan(wind/baseSpeed)*180/  M_PI;
}

void Atmosphere::arrange(int temperature){
    if(temperature<-273 || temperature >200){
        type = NONE;
    }
        
    else if(temperature>80){
        type = HEATED_RAIN;
        baseSpeed = 4;
    }
        
    else if(temperature>=0){
        if(temperature<10){
            std::subtract_with_carry_engine<unsigned int,3,7,11> engine((unsigned)time(NULL));
            if(engine()%4 ==0){
                type = HAIL;
                baseSpeed = 6;
                return;
            }
        }
        type = RAIN;
        baseSpeed = 5;
    } 
    else{
        type = SNOW;
        baseSpeed = 4;
    }
};