Caja-bug170
===========

Demonstrates how to reproduce caja issue #170 with a simple program.

https://github.com/mate-desktop/caja/issues/170

https://bugzilla.gnome.org/show_bug.cgi?id=721590

There 2 different bugs in Caja issue #170:
- an infinite loop of caja windows while MATE Desktop is running.
- 10 x-caja-windows showing up at session startup.

We are dealing with the second issue here.
The issue about "10 x-caja-windows showing up at session startup" is not a bug in caja itself.
We still don't know why GSettings triggers "change callback" functions in Caja at startup, but the bug can be reproduced without caja.

__The probable cause of the 10 windows showing up:__

Our tests showed that GSettings may trigger *all callbacks* of *all GSettings objects* in an application even if the corresponding parameters haven't changed at all.
That shouldn't happen because each callback is connected to only one GSettings object in Caja.

There are 10 (or 11) GSettings objects in Caja source code.
Caja does not check that parameters have really changed or not, so it opens 10 windows, because that's what one of the  callbacks does ('desktop\_location\_changed_callback').

Instructions
------------

DON'T USE YOUR NORMAL USER ACCOUNT FOR THAT. CREATE A TEST USER BEFORE.

- Use compile.sh in src/ directory to compile the executable
- Modify the 'Exec=' line in caja-bug170.desktop 
- Copy caja-bug170.desktop to /etc/xdg/autostart/
- Put the following line in your ~/.xinitrc file: exec ck-launch-session dbus-launch mate-session --debug
- Slow down your system as much as you can with some commands like : yes > /dev/null & (one time per cpu core) 
- Start X-Window with : rm -rf ~/.config && startx 2> /tmp/debug.log 1>&2
- Use 'killall yes' to restore normal system speed.
 
Result
------

If you have got the bug, the following line should appear 10 times in the log file:

 === CAJA BUG #170. xsmp + dconf + GSettings bug? count=1-10
 
 

