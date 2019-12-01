#include "nemu.h"
#include "monitor/diff-test.h"
//#include "../../../tools/qemu-diff/src/isa/x86/include/isa.h"

// 从DUT host memory的`src`处拷贝`n`字节到REF guest memory的`dest`处
void difftest_memcpy_from_dut(paddr_t dest, void *src, size_t n);
// 获取REF的寄存器状态到`r`
void difftest_getregs(void *r);
// 设置REF的寄存器状态为`r`
void difftest_setregs(const void *r);
// 让REF执行`n`条指令
void difftest_exec(uint64_t n);
// 初始化REF的DiffTest功能
void difftest_init();

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  //union isa_gdb_regs r;
  //ref_difftest_getregs(&r);

  if (cpu.eax != ref_r->eax){
    printf("Eax was Wrong! And it should be 0x%x.\n", ref_r->eax);
    return false;
  }
    
  if (cpu.ecx != ref_r->ecx){
    printf("Ecx was Wrong!\n");
    return false;
  }
    
  if (cpu.edx != ref_r->edx){
    printf("Edx was Wrong!\n");
    return false;
  }

  if (cpu.ebx != ref_r->ebx){
    printf("Ebx was Wrong!\n");
    return false;
  }
    
  if (cpu.esp != ref_r->esp){
    printf("Esp was Wrong!\n And it should be 0x%x.\n", ref_r->esp);
    return false;
  }
    
  if (cpu.ebp != ref_r->ebp){
    printf("Ebp was Wrong!\n");
    return false;
  }
    
  if (cpu.esi != ref_r->esi){
    printf("Esi was Wrong! And it should be 0x%x.\n", ref_r->esi);
    return false;
  }
    
  if (cpu.edi != ref_r->edi){
    printf("Edi was Wrong!\n");
    return false;
  }
    
  if (cpu.pc != ref_r->pc){
    printf("PC was Wrong! And it should be 0x%x.\n", ref_r->pc);
    return false;
  }

  /*
  if (cpu.eflag != ref_r->eflag){
    printf("EFLAGS was wrong!\n");
    return false;
  }
  */
  /*
  if (cpu.eflags.CF != ref_r->eflags.CF){
    printf("CF was wrong!\n");
    return false;
  }

  if (cpu.eflags.ZF != ref_r->eflags.ZF){
    printf("ZF was wrong!\n");
    return false;
  }

  if (cpu.eflags.SF != ref_r->eflags.SF){
    printf("SF was wrong!\n");
    return false;
  }

  if (cpu.eflags.OF != ref_r->eflags.OF){
    printf("OF was wrong!\n");
    return false;
  }
  */
  return true;
}

void isa_difftest_attach(void) {
  difftest_memcpy_from_dut(0, (void *)0, 0x7c00);
  difftest_memcpy_from_dut(0x100000, (void *)0x100000, PMEM_SIZE - 0x100000);
  difftest_memcpy_from_dut(0x7e00, &cpu.idtr, 6);
  uint8_t instruction[] = {0x0f, 0x01, 0x1D, 0x00, 0x7e, 0x00, 0x00};
  difftest_memcpy_from_dut(0x7e40, instruction, sizeof(instruction));
  CPU_state temp = cpu;
  temp.pc = 0x7e40;
  difftest_setregs(&temp);
  difftest_exec(1);
  difftest_setregs(&cpu);
}
