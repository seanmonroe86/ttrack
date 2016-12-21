ttrack COMMAND [ARGS]

COMMAND
start	NAME	begin new tracker with NAME; default Timer
edit			modify currently running tracker with new flag info
list			print tracker NAME and RUNTIME or No Tracker otherwise
status			show details of current tracker
stop			stop current tracker and make record

report			prints full details of ended trackers in history
delete			delete all info of tracker with NAME
help			prints something like this doc, and total program use time

ARGS
-a(go)		HH:MM:SS	set start time to current time minus this value; default Current Time
-d(ate)		MM/DD/YY	set date; default Current Date
-n(ote)		STRING		set note; default “”
-s(tart)	HH:MM:SS	set start time; default Current Time
-t(ag)		STRING		set tag; default “”
