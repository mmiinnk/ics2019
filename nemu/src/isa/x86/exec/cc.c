#include "rtl/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };
  char CF = cpu.eflags.CF;
  char OF = cpu.eflags.OF;
  char ZF = cpu.eflags.ZF;
  char SF = cpu.eflags.SF;

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:{
	        if (OF == 0){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;		
	      }
    case CC_B:{
		if (CF == 1 && ZF == 0){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	      }
    case CC_E:{
	        if(ZF == 1){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	      }
    case CC_BE:{
		if (CF == 1 || ZF == 1){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	      }
    case CC_S:{
		if (SF == 1){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	      }
    case CC_L:{
		if (SF != OF && ZF == 0){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	      }
    case CC_LE:{
		if (SF != OF || ZF == 1){
		  *dest = 1;
		}
		else{
		  *dest = 0;
		}
		break;
	       }
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}
