# MAME

This is the MILLILUK fork of MAME -- the Multiple Arcade Machine Emulator.

You probably want the official version:  
Please visit <https://github.com/mamedev/mame>.

Project goals:

* hackishly provide an improved development environment for the TRS-80 CoCo and its relatives (MC-10, Dragon, etc.)
* cleanly provide ideas for possible inclusion in MAME

### Changelog

#### 0.167 M1 
* Enable GIME to set screen height in legacy graphics modes (Robert Gault bug report)

#### 0.166 M1 
* Add more accurate CoCo 3 composite palette and support for phase invert
* Improve 6309 register display in the debugger
* Add LOG (0x103E) and BREAK (0x113E) support to 6809/6309
[inspired by Tim Lindner's EMUBRK]
* Add DEBUGCONTROL port and frame and cycle counts
* Do not break into MAME debugger on launch (use ~)
* Improved mouse cursor hiding on Windows
* Optionally enable old Western Digital floppy code (less accurate but much faster)
* Build a subset of CoCo-related drivers as "mametiny"
* Include Visual Studio build files: /build/projects/windows/mametiny/vs2015/mametiny.sln

#### Fake Opcodes

BREAK - breaks into the MAME debugger  
LOG - writes printf-style output to the MAME debug window

          LOG
          FDB   FSTR
          FDB   X1
          FDB   Y1
          FDB   X2
          FDB   Y2

    ; execution continues here

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

