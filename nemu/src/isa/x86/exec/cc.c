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
  char CF = cpu.CF;
  char OF = cpu.OF;
  char ZF = cpu.ZF;
  char SF = cpu.SF;

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:  *dest = (OF == 1) ? 1 : 0; break;
    //case CC_NO: *dest = (OF == 0) ? 1 : 0; break;
    case CC_B:  *dest = (CF == 1 && ZF == 0) ? 1 : 0; break;
    //case CC_NB: *dest = (CF == 0) ? 1 : 0; break;
    case CC_E:  *dest = (ZF == 1) ? 1 : 0; break;
	  //case CC_NE: *dest = (ZF == 0) ? 1 : 0; break;
	  case CC_BE: *dest = (CF == 1 || ZF == 1) ? 1 : 0; break;
	  //case CC_NBE:*dest = (CF == 0 && ZF == 0) ? 1 : 0; break;
    case CC_S:  *dest = (SF == 1) ? 1 : 0; break;
	  //case CC_NS: *dest = (SF == 0) ? 1 : 0; break;
	  //case CC_P:  *dest = (PF == 1) ? 1 : 0; break;
	  //case CC_NP: *dest = (PF == 0) ? 1 : 0; break;
    case CC_L:  *dest = (SF != OF && ZF == 0) ? 1 : 0; break;
	  //case CC_NL: *dest = (SF == OF) ? 1 : 0; break;
    case CC_LE: *dest = (SF != OF || ZF == 1) ? 1 : 0; break;
    //case CC_NLE:*dest = (ZF == 0 && SF == OF) ? 1 : 0; break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}
