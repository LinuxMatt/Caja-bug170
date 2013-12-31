Caja-bug170
===========

Demonstrates how to reproduce caja issue #170 with a simple program.

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
 
 

