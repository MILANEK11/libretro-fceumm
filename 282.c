﻿#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_JY.h"
#include 	"..\Hardware\h_DIP.h"

namespace {
void	sync (void) {
	JY::syncPRG(0x1F, JY::outerBank.prgHigh <<5);
	if (JY::outerBank.chr4M) {
		JY::syncCHR(0x1FF, 0x00);
		JY::syncNT (0x1FF, 0x00);
	} else {
		int chrHigh =JY::outerBank.chrA18 | (JY::outerBank.chrHigh <<1);
		JY::syncCHR(0x0FF, chrHigh <<8);
		JY::syncNT (0x0FF, chrHigh <<8);
	}
}

BOOL	MAPINT	load (void) {
	JY::load(sync, true);
	iNES_SetSRAM();
	return TRUE;
}

uint16_t mapperNum =282;
} // namespace

MapperInfo MapperInfo_282 = {
	&mapperNum,
	_T("晶太 860224C"),
	COMPAT_FULL,
	load,
	JY::reset,
	NULL,
	JY::cpuCycle,
	JY::ppuCycle,
	JY::saveLoad,
	NULL,
	DIP::Config
};
