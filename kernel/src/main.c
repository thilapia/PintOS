_Noreturn void kernel_main(void) {
  __asm__ volatile("cli"); // NOLINT(portability-no-assembler)
  for (;;) {
    __asm__ volatile("hlt"); // NOLINT(portability-no-assembler)
  }
}
