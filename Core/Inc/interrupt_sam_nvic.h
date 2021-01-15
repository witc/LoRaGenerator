/*
 * interrupt_sam_nvic.h
 *
 *  Created on: 5. 2. 2017
 *      Author: jirik
 */

#ifndef UTILS_INTERRUPT_INTERRUPT_H
#define UTILS_INTERRUPT_INTERRUPT_H

#include "cmsis_gcc.h"


#ifdef __cplusplus
extern "C" {
#endif

#  define cpu_irq_enable()                     \
	do {                                       \
		__DMB();                               \
		__enable_irq();                        \
	} while (0)
#  define cpu_irq_disable()                    \
	do {                                       \
		__disable_irq();                       \
		__DMB();                               \
	} while (0)

typedef uint32_t irqflags_t;

#define cpu_irq_is_enabled()    (__get_PRIMASK() == 0)

static volatile uint32_t cpu_irq_critical_section_counter;
static volatile bool     cpu_irq_prev_interrupt_state;

static inline irqflags_t cpu_irq_save(void)
{
	irqflags_t flags = cpu_irq_is_enabled();
	cpu_irq_disable();
	return flags;
}

static inline bool cpu_irq_is_enabled_flags(irqflags_t flags)
{
	return (flags);
}

static inline void cpu_irq_restore(irqflags_t flags)
{
	if (cpu_irq_is_enabled_flags(flags))
		cpu_irq_enable();
}

/**
 * \weakgroup interrupt_deprecated_group
 * @{
 */

#define Enable_global_interrupt()            cpu_irq_enable()
#define Disable_global_interrupt()           cpu_irq_disable()
#define Is_global_interrupt_enabled()        cpu_irq_is_enabled()


#ifdef __cplusplus
}
#endif

#endif /* UTILS_INTERRUPT_INTERRUPT_H */
