/* 
 * File:        interrupt.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include "interrupt.h"

void initInterrupt( volatile unsigned int * intEnb,
        volatile unsigned int * intPrio,
        volatile unsigned int * intFlag,
        unsigned int intPrioOffset,
        unsigned int intSubPrioOffset,
        unsigned int intMask ) {
  
  // configure interrupt priority and sub-priority levels in the IPCx register
  *intPrio |= 5<<intPrioOffset;      // set priority level = 3
  *intPrio |= 0b1<<intSubPrioOffset;     // set sub-priority level = 1
  
  // clear the TxIF interrupt flag bit
  *intFlag &= ~intMask;
  
  // set the TxIE interrupt enable bit
  *intEnb |= intMask;
  
  return;
}