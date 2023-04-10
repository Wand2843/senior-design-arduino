#include "debouncer.h"
#include <math.h>
enum dbStates {
  NPUSH, MAYPUSH, PUSHED, MAYNPUSH,DBa_SMStart
} dbState;
int debouncerOut;
bool tick_Debouncer(bool pressed){
  switch(dbState){
        case DBa_SMStart:
            dbState =  NPUSH;
            break;
        case NPUSH:
            if (pressed){
                dbState = MAYPUSH;
            } else if (!pressed){
                dbState =  NPUSH;
            } 
            break;
        
        case MAYPUSH:
            if (pressed){
                dbState = PUSHED;
                debouncerOut=true;
            } else if (!pressed){
                dbState =  NPUSH;
            } break;
        
        case PUSHED:
            if (pressed){
                dbState = PUSHED;
            } else if (!pressed){
                dbState =  MAYNPUSH;
            } 
            break;
        
        case MAYNPUSH:
            if (pressed){
                dbState = PUSHED;
            } else if (!pressed){
                dbState =  NPUSH;
            } 
            break;
  }
  return debouncerOut;
}
void initDebouncer(){
  dbState = DBa_SMStart;
  debouncerOut=false;
}