/* arch/arm/mach-msm/include/mach/memory.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009-2012, Code Aurora Forum. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H
#include <linux/types.h>

#define PLAT_PHYS_OFFSET UL(CONFIG_PHYS_OFFSET)

#if defined(CONFIG_KEXEC_HARDBOOT)
#if defined(CONFIG_MACH_VILLE)
#define KEXEC_HB_PAGE_ADDR		UL(0x88700000)
#else
#error "Adress for kexec hardboot page not defined"
#endif
#endif

#define MAX_PHYSMEM_BITS 32
#define SECTION_SIZE_BITS 28

#define MAX_NR_REGIONS 32

#define NR_REGIONS_PER_BANK 8


#if defined(CONFIG_ARCH_MSM7X30)

#define EBI0_PHYS_OFFSET PHYS_OFFSET
#define EBI0_PAGE_OFFSET PAGE_OFFSET
#define EBI0_SIZE 0x10000000

#ifndef __ASSEMBLY__

extern unsigned long ebi1_phys_offset;

#define EBI1_PHYS_OFFSET (ebi1_phys_offset)
#define EBI1_PAGE_OFFSET (EBI0_PAGE_OFFSET + EBI0_SIZE)

#if (defined(CONFIG_SPARSEMEM) && defined(CONFIG_VMSPLIT_3G))

#define __phys_to_virt(phys)				\
	((phys) >= EBI1_PHYS_OFFSET ?			\
	(phys) - EBI1_PHYS_OFFSET + EBI1_PAGE_OFFSET :	\
	(phys) - EBI0_PHYS_OFFSET + EBI0_PAGE_OFFSET)

#define __virt_to_phys(virt)				\
	((virt) >= EBI1_PAGE_OFFSET ?			\
	(virt) - EBI1_PAGE_OFFSET + EBI1_PHYS_OFFSET :	\
	(virt) - EBI0_PAGE_OFFSET + EBI0_PHYS_OFFSET)

#endif
#endif

#endif

#ifndef __ASSEMBLY__
void *alloc_bootmem_aligned(unsigned long size, unsigned long alignment);
void *allocate_contiguous_ebi(unsigned long, unsigned long, int);
unsigned long allocate_contiguous_ebi_nomap(unsigned long, unsigned long);
void clean_and_invalidate_caches(unsigned long, unsigned long, unsigned long);
void clean_caches(unsigned long, unsigned long, unsigned long);
void invalidate_caches(unsigned long, unsigned long, unsigned long);
int platform_physical_remove_pages(u64, u64);
int platform_physical_active_pages(u64, u64);
int platform_physical_low_power_pages(u64, u64);

extern int (*change_memory_power)(u64, u64, int);

#if defined(CONFIG_ARCH_MSM_ARM11) || defined(CONFIG_ARCH_MSM_CORTEX_A5)
void write_to_strongly_ordered_memory(void);
void map_page_strongly_ordered(void);
#endif

#ifdef CONFIG_CACHE_L2X0
extern void l2x0_cache_sync(void);
#define finish_arch_switch(prev)     do { l2x0_cache_sync(); } while (0)
#endif

#if defined(CONFIG_ARCH_MSM8X60) || defined(CONFIG_ARCH_MSM8960)
extern void store_ttbr0(void);
#define finish_arch_switch(prev)	do { store_ttbr0(); } while (0)
#endif

#ifdef CONFIG_DONT_MAP_HOLE_AFTER_MEMBANK0
extern unsigned long membank0_size;
extern unsigned long membank1_start;
void find_membank0_hole(void);

#define MEMBANK0_PHYS_OFFSET PHYS_OFFSET
#define MEMBANK0_PAGE_OFFSET PAGE_OFFSET

#define MEMBANK1_PHYS_OFFSET (membank1_start)
#define MEMBANK1_PAGE_OFFSET (MEMBANK0_PAGE_OFFSET + (membank0_size))

#define __phys_to_virt(phys)				\
	((MEMBANK1_PHYS_OFFSET && ((phys) >= MEMBANK1_PHYS_OFFSET)) ?	\
	(phys) - MEMBANK1_PHYS_OFFSET + MEMBANK1_PAGE_OFFSET :	\
	(phys) - MEMBANK0_PHYS_OFFSET + MEMBANK0_PAGE_OFFSET)

#define __virt_to_phys(virt)				\
	((MEMBANK1_PHYS_OFFSET && ((virt) >= MEMBANK1_PAGE_OFFSET)) ?	\
	(virt) - MEMBANK1_PAGE_OFFSET + MEMBANK1_PHYS_OFFSET :	\
	(virt) - MEMBANK0_PAGE_OFFSET + MEMBANK0_PHYS_OFFSET)
#endif

#endif

#if defined CONFIG_ARCH_MSM_SCORPION || defined CONFIG_ARCH_MSM_KRAIT
#define arch_has_speculative_dfetch()	1
#endif

#endif

#define MEMORY_DEEP_POWERDOWN	0
#define MEMORY_SELF_REFRESH	1
#define MEMORY_ACTIVE		2

#define NPA_MEMORY_NODE_NAME	"/mem/apps/ddr_dpd"

#ifndef CONFIG_ARCH_MSM7X27
#define CONSISTENT_DMA_SIZE	(SZ_1M * 14)
#endif
