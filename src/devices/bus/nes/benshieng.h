// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef __NES_BENSHIENG_H
#define __NES_BENSHIENG_H

#include "nxrom.h"


// ======================> nes_benshieng_device

class nes_benshieng_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_benshieng_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;
	virtual DECLARE_WRITE8_MEMBER(write_h) override;

	virtual void pcb_reset() override;

private:
	void update_banks();
	uint8_t m_dipsetting;
	uint8_t m_mmc_prg_bank[4];
	uint8_t m_mmc_vrom_bank[4];
};


// device type definition
extern const device_type NES_BENSHIENG;

#endif
