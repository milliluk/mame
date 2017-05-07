# MAME

This is the MILLILUK development fork of MAME -- the Multiple Arcade Machine Emulator.

Modifications current as of MAME 0.183.

| OS | Status | 
| --- |:---:| 
| Linux / Mac | [![Build Status](https://travis-ci.org/milliluk/mame.svg?branch=master)](https://travis-ci.org/milliluk/mame) |
| Windows | [![Build status](https://ci.appveyor.com/api/projects/status/3mf0eo75hsaoj2ox/branch/master?svg=true)](https://ci.appveyor.com/project/milliluk/mame/branch/master) |

### Downloads

Windows (64 bit): https://ci.appveyor.com/api/projects/milliluk/mame/artifacts/cocomame.zip?job=Platform%3A%20x64
Windows (32 bit): https://ci.appveyor.com/api/projects/milliluk/mame/artifacts/cocomame.zip?job=Platform%3A%20Win32

### Project Goals

* Improve the CoCo family of emulators (help wanted!)
* Cleanly provide ideas for possible inclusion in MAME
* Hackishly provide an improved development environment for the TRS-80 CoCo and its relatives (MC-10, Dragon, etc.)

### How to Contribute

Mac/Linux: Visit <http://mamedev.org/tools/>. This also works if you want a command line environment on Windows (you don't).

Windows: Generated ("make vs2017") Visual Studio 2017 project files are checked in for your convenience:

- Clone as follows: git clone https://github.com/milliluk/mame.git
- Then launch: mame/build/projects/windows/mametiny/vs2017/mametiny.sln

### Changelog

- [x] Enable $FF9C semigraphics emulation (thanks, Nick Marentes!)
- [x] Enable GIME to set screen height in legacy graphics modes (thanks, Robert Gault!)
- [x] Add more accurate CoCo 3 composite palette and support for phase invert
- [x] Improve 6309 register display in the debugger
- [ ] Add LOG (0x103E) and BREAK (0x113E) support to 6809/6309 (inspired by Tim Lindner's EMUBRK)
- [ ] Add DEBUGCONTROL port and frame and cycle counts
- [ ] Do not break into MAME debugger on launch (use ~)
- [ ] Improved mouse cursor hiding on Windows
- [ ] Build a subset of CoCo-related drivers as "mametiny"
- [ ] Include Visual Studio build files: /build/projects/windows/mametiny/vs2017/mametiny.sln

Checked boxes indicates patches accepted into the main MAME code base.

### Developer Enhancements

The following fake opcodes are added to the 6809 and 6309 CPU:

**BREAK** (0x113E) - breaks into the MAME debugger

**LOG** (0x103E) - writes printf-style output to the MAME debug window

          LOG           ; log output
          FDB   FSTR    ; pointer to format string
          FDB   PX1     ; 16 bit pointer to 16 bit value
          FDB   PY1     ; 16 bit pointer to 8 bit value (see format string!)
          FDB   PX2     ; 16 bit pointer to 16 bit value
          FDB   PY2     ; 16 bit pointer to 8 bit value
          ; execution continues here ...
          RTS
            
    ; format string
    FSTR  FCC   "%hu,%hhu - %hu,%hhu"
          FCB   10,0

#### Additional timing support

    ADDRESS    NAME            READ    WRITE

    FF04       DEBUGCONTROL    N/A     Bit 0: Set to '1' causes the System Cycle Count at $FF05 to be snapshot.
                               N/A     Bit 1: Set to '1' causes the video Frame count at $FF09 to be snapshot.
                               N/A     Bit 2...7: N/A

    FF05-FF08  CYCLECOUNT      When read, the values between $FF05 and $FF08 represent a snapshot of the system's 
                               internal cycle counter. The snapshot occurs when the appropriate write is made to 
                               the control register at $FF04. The most significant byte begins at $FF05, the 
                               least significant byte of the 32-bit value is at $FF08.

    FF09-FF0C  FRAMECOUNT      When read, the values between $FF09 and $FF0B represent a snapshot of the system's
                               video frame counter. The snapshot occurs when the appropriate write is made to the
                               control register at $FF04. The most significant byte begins at $FF09, the least
                               significant byte of the 32-bit value is at $FF0C.

### License

All modifications are licensed under the [BSD 3 Clause license](http://opensource.org/licenses/BSD-3-Clause).

