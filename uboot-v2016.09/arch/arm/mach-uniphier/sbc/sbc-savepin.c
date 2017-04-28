/*
 * Copyright (C) 2011-2016 Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <linux/io.h>

#include "../init.h"
#include "sbc-regs.h"

/* slower but LED works */
#define SBCTRL0_SAVEPIN_PERI_VALUE	0x55450000
#define SBCTRL1_SAVEPIN_PERI_VALUE	0x07168d00
#define SBCTRL2_SAVEPIN_PERI_VALUE	0x34000009
#define SBCTRL4_SAVEPIN_PERI_VALUE	0x02110110

/* faster but LED does not work */
#define SBCTRL0_SAVEPIN_MEM_VALUE	0x55450000
#define SBCTRL1_SAVEPIN_MEM_VALUE	0x06057700
/* NOR flash needs more wait counts than SRAM */
#define SBCTRL2_SAVEPIN_MEM_VALUE	0x34000009
#define SBCTRL4_SAVEPIN_MEM_VALUE	0x02110210

int uniphier_sbc_init_savepin(const struct uniphier_board_data *bd)
{
	/*
	 * Only CS1 is connected to support card.
	 * BKSZ[1:0] should be set to "01".
	 */
	writel(SBCTRL0_SAVEPIN_PERI_VALUE, SBCTRL10);
	writel(SBCTRL1_SAVEPIN_PERI_VALUE, SBCTRL11);
	writel(SBCTRL2_SAVEPIN_PERI_VALUE, SBCTRL12);
	writel(SBCTRL4_SAVEPIN_PERI_VALUE, SBCTRL14);

	if (boot_is_swapped()) {
		/*
		 * Boot Swap On: boot from external NOR/SRAM
		 * 0x42000000-0x43ffffff is a mirror of 0x40000000-0x41ffffff.
		 *
		 * 0x40000000-0x41efffff, 0x42000000-0x43efffff: memory bank
		 * 0x41f00000-0x41ffffff, 0x43f00000-0x43ffffff: peripherals
		 */
		writel(0x0000bc01, SBBASE0);
	} else {
		/*
		 * Boot Swap Off: boot from mask ROM
		 * 0x40000000-0x41ffffff: mask ROM
		 * 0x42000000-0x43efffff: memory bank (31MB)
		 * 0x43f00000-0x43ffffff: peripherals (1MB)
		 */
		writel(0x0000be01, SBBASE0); /* dummy */
		writel(0x0200be01, SBBASE1);
	}

	return 0;
}
