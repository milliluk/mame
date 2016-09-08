-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   tiny.lua
--
--   Small driver-specific example makefile
--   Use make SUBTARGET=tiny to build
--
---------------------------------------------------------------------------


--------------------------------------------------
-- Specify all the CPU cores necessary for the
-- drivers referenced in tiny.lst.
--------------------------------------------------

CPUS["Z80"] = true
CPUS["M6502"] = true
CPUS["MCS48"] = true
CPUS["MCS51"] = true
CPUS["M6800"] = true
CPUS["M6809"] = true
CPUS["M680X0"] = true
CPUS["TMS9900"] = true
CPUS["COP400"] = true

--------------------------------------------------
-- Specify all the sound cores necessary for the
-- drivers referenced in tiny.lst.
--------------------------------------------------

SOUNDS["SAMPLES"] = true
SOUNDS["DAC"] = true
SOUNDS["DISCRETE"] = true
SOUNDS["AY8910"] = true
SOUNDS["YM2151"] = true
SOUNDS["ASTROCADE"] = true
SOUNDS["TMS5220"] = true
SOUNDS["OKIM6295"] = true
SOUNDS["HC55516"] = true
SOUNDS["YM3812"] = true
SOUNDS["CEM3394"] = true
SOUNDS["VOTRAX"] = true
SOUNDS["WAVE"] = true
SOUNDS["BEEP"] = true

--------------------------------------------------
-- specify available video cores
--------------------------------------------------

VIDEOS["MC6847"] = true
VIDEOS["EF9345"] = true

--------------------------------------------------
-- specify available machine cores
--------------------------------------------------

MACHINES["6821PIA"] = true
MACHINES["TTL74148"] = true
MACHINES["TTL74153"] = true
MACHINES["TTL7474"] = true
MACHINES["RIOT6532"] = true
MACHINES["PIT8253"] = true
MACHINES["Z80CTC"] = true
MACHINES["68681"] = true
MACHINES["BANKDEV"] = true
MACHINES["GEN_LATCH"] = true
MACHINES["WATCHDOG"] = true
MACHINES["MOS6551"] = true
MACHINES["MSM6242"] = true
MACHINES["DS1315"] = true

--------------------------------------------------
-- specify available bus cores
--------------------------------------------------

BUSES["CENTRONICS"] = true
BUSES["COCO"] = true
BUSES["RS232"] = true
BUSES["SUNKBD"] = true

--------------------------------------------------
-- specify available formats
--------------------------------------------------

FORMATS["COCO_CAS"] = true
FORMATS["BASICDSK"] = true
FORMATS["VDK_DSK"] = true
FORMATS["DMK_DSK"] = true

--------------------------------------------------
-- specify available formats
--------------------------------------------------

FORMATS["COCO_CAS"] = true
FORMATS["BASICDSK"] = true
FORMATS["VDK_DSK"] = true
FORMATS["DMK_DSK"] = true
FORMATS["JVC_DSK"] = true

--------------------------------------------------
-- This is the list of files that are necessary
-- for building all of the drivers referenced
-- in tiny.lst
--------------------------------------------------

function createProjects_mame_tiny(_target, _subtarget)
	project ("mame_tiny")
	targetsubdir(_target .."_" .. _subtarget)
	kind (LIBTYPE)
	uuid (os.uuid("drv-mame-tiny"))
	addprojectflags()
	precompiledheaders()

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/mame",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "3rdparty",
		GEN_DIR  .. "mame/layout",
	}

	files{
		MAME_DIR .. "src/mame/drivers/coco12.cpp",    
		MAME_DIR .. "src/mame/drivers/coco3.cpp",     
		MAME_DIR .. "src/mame/drivers/dragon.cpp",    
		MAME_DIR .. "src/mame/drivers/mc10.cpp",      
		MAME_DIR .. "src/mame/machine/6883sam.cpp",   
		MAME_DIR .. "src/mame/machine/coco.cpp",      
		MAME_DIR .. "src/mame/machine/coco12.cpp",    
		MAME_DIR .. "src/mame/machine/coco3.cpp",     
		MAME_DIR .. "src/mame/machine/coco_vhd.cpp",  
		MAME_DIR .. "src/mame/machine/dragon.cpp",    
		MAME_DIR .. "src/mame/machine/dgnalpha.cpp",  
		MAME_DIR .. "src/mame/video/gime.cpp",
		MAME_DIR .. "src/devices/machine/wd17xx.cpp",
		MAME_DIR .. "src/devices/machine/wd_fdc.cpp",
		MAME_DIR .. "src/devices/machine/fdc_pll.cpp",
		MAME_DIR .. "src/tools/imgtool/formats/coco_dsk.cpp",
		MAME_DIR .. "src/devices/cpu/m6809/formatter.cpp",

		MAME_DIR .. "src/mame/drivers/dgn_beta.cpp",
		MAME_DIR .. "src/mame/machine/dgn_beta.cpp",
		MAME_DIR .. "src/mame/video/dgn_beta.cpp", 

		MAME_DIR .. "src/lib/formats/flopimg.cpp",
		MAME_DIR .. "src/lib/formats/flopimg.h"
	}
end

function linkProjects_mame_tiny(_target, _subtarget)
	links {
		"mame_tiny",
	}
end
