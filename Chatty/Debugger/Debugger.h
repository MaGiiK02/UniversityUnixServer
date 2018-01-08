//
// Created by Mattia Angelini on 23/11/2017.
//

#ifndef SOL_2017_DEBUGGER_H
#define SOL_2017_DEBUGGER_H

#ifdef DEBUG
#define Log(x) printf x
#define ON_DEBUG(code) code
#else
#define Log(x) do {} while (0)
#define ON_DEBUG(code) do{} while(0);
#endif



#endif //SOL_2017_DEBUGGER_H
