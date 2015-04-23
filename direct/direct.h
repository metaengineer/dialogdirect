#pragma once

#include "resource.h"
#include "ParserMeta.h"

// flavor script syntax
/*
	flavor global floating-point variables ($g1 - $g<quantity>)
	set by "vars ###" (32 by default, max 999)
	
	meta script loaded by "meta "metascriptname""

	frame:
	begin frame "name"
		bg "bitmap_name"
		
		...
	end

	menu:
	begin menu x y
		option "name" "action"
		...
		option "name" "action"
	end

	list:
	begin list x y
		option "name" "action"
		...
		option "name" "action"
	end
	
	tabs (up to 4):
	begin tabs
		tab "name" number
		...
	end

	button "regex_name" x y "action"

	text "regex_name" x y width

	slider "name" x y var_num min max

	field "name"

	checkbox "name"

	bitmap "filename" x y

	that's all. once they are loaded, program picks the frame "index"

	actions:

	goto frame

	run    - run something that this interface is for

	group number - show another tab

	set var_num value

	exit
*/