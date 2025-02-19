/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* NES 2.0 Mapper 313 is used for MMC3-based multicarts that switch
 * between 128 KiB PRG-ROM/128 KiB CHR-ROM-sized games on each reset and
 * thus require no additional registers.
 * Its UNIF board name is BMC-RESET-TXROM.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 chip;

static void M313CW(uint32 A, uint8 V) {
	setchr1(A, (EXPREGS[0] << 7) | (V & 0x7F));
}

static void M313PW(uint32 A, uint8 V) {
	setprg8(A, (EXPREGS[0] << 4) | (V & 0x0F));
}

static void M313Reset(void) {
	EXPREGS[0]++;
	EXPREGS[0] &= 0x03;
	MMC3RegReset();
}

static void M313Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
}

/* NES 2.0 313, UNIF BMC-RESET-TXROM */
void BMCRESETTXROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 0, 0);
	cwrap = M313CW;
	pwrap = M313PW;
	info->Power = M313Power;
	info->Reset = M313Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
