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
        unsigned char prio,
        unsigned int intSubPrioOffset,
        unsigned char subPrio,
        unsigned int intMask ) {
  
  // configure interrupt priority and sub-priority levels in the IPCx register
  *intPrio |= prio<<intPrioOffset;           // set priority level = x
  *intPrio |= subPrio<<intSubPrioOffset;     // set sub-priority level = y
  
  // clear the TxIF interrupt flag bit
  *intFlag &= ~intMask;
  
  // set the TxIE interrupt enable bit
  *intEnb |= intMask;
  
  return;
}