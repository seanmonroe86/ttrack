ttrack COMMAND [ARGS]
A simple-looking stopwatch. Starts a new timer, edits the current timer, and
stops the current timer recording its label, note and run time. Tags can be
used to 'group' timers, giving additional cumulative run time for tags of the
same name. Additional arguments can modify the starting time and date of the
new or existing timer. Arguments without a command are ignored. Given no
command, it defaults to 'list'.

COMMAND
start	NAME	begin new timer with NAME; default Timer
edit			modify currently running timers with new name and/or flag info
list			print timer "NAME: RUNTIME" or "No timer" otherwise, without newline
status			show details of current timer
stop			stop current timer and make record

report			prints full details of stopped timers in history
delete			delete all info of timer with NAME ***
help			prints something like this doc ***

ARGS
-a(go)		HH:MM:SS	set start time to current time minus this value default 00:00:00 ***
-d(ate)		MM/DD/YY	set date; default Current Date ***
-s(tart)	HH:MM:SS	set start time; default Current Time ***

-n(ote)		STRING		set note; default blank
-t(ag)		STRING		set tag; default blank

*** Not implemented yet.
