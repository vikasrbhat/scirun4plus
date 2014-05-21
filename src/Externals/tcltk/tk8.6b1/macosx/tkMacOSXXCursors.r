/*
 * tkMacOSXXCursors.r --
 *
 *	This file defines a set of Macintosh cursor resources that
 *	emulate the X cursor set. All of these cursors were
 *	constructed and donated by Grant Neufeld. (gneufeld@ccs.carleton.ca)
 *
 * Copyright (c) 1995-1996 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tkMacOSXXCursors.r,v 1.3 2007/04/23 21:24:34 das Exp $
 */

/*
 * All of the X cursors are defined as 'CURS' resources. However, a
 * subset of the X cursors are also defined as 'crsr' resources. Tk
 * will attempt to first use the color cursors ('crsr') if it doesn't
 * exist it will attempt to use the black & white cursor ('CURS').
 */

data 'CURS' (3000, "X_cursor") {
	$"E007 F00F F81F 7C3E 3E7C 1FF8 0FF0 07E0"
	$"07E0 0FF0 1FF8 3E7C 7C3E F81F F00F E007"
	$"0000 6006 700E 381C 1C38 0E70 07E0 03C0"
	$"03C0 07E0 0E70 1C38 381C 700E 6006 0000"
	$"0007 0007"
};

data 'CURS' (3001, "arrow") {
	$"0000 0006 001E 007C 01FC 07F8 00F8 01F0"
	$"03B0 0720 0E20 1C00 3800 7000 2000 0000"
	$"0007 001F 007F 01FE 07FE 1FFC 7FFC 03F8"
	$"07F8 0FF0 1F70 3E60 7C60 F840 7040 2000"
	$"0001 000E"
};

data 'CURS' (3002, "based_arrow_down") {
	$"0000 0000 0000 1FE0 0000 1FE0 0300 0300"
	$"0300 0B40 0780 0300 0000 0000 0000 0000"
	$"0000 0000 0000 1FE0 0000 1FE0 0780 0780"
	$"3FF0 1FE0 0FC0 0780 0300 0000 0000 0000"
	$"000B 0006"
};

data 'CURS' (3003, "based_arrow_up") {
	$"0000 0000 0000 0000 0300 0780 0B40 0300"
	$"0300 0300 1FE0 0000 1FE0 0000 0000 0000"
	$"0000 0000 0000 0300 0780 0FC0 1FE0 3FF0"
	$"0780 0780 1FE0 0000 1FE0 0000 0000 0000"
	$"0004 0006"
};

data 'CURS' (3004, "boat") {
	$"0000 0000 0000 0000 0100 03C0 8460 FFFF"
	$"0018 0020 0040 FFC0 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0100 03C0 87E0 FFFF"
	$"FFF8 FFE0 FFC0 FFC0 0000 0000 0000 0000"
	$"0007 000F"
};

data 'CURS' (3005, "bogosity") {
	$"0000 711C 1110 1110 1110 7FFC 5114 5114"
	$"5114 5114 7FFC 1110 1110 1110 711C 0000"
	$"0000 0000 0000 0000 0000 7FFC 7FFC 7FFC"
	$"7FFC 7FFC 7FFC 0000 0000 0000 0000 0000"
	$"0001 0007"
};

data 'CURS' (3006, "bottom_left_corner") {
	$"0000 0000 0000 0000 C000 C020 C840 C880"
	$"C900 CA00 CC00 CFC0 C000 C000 FFF0 FFF0"
	$"0000 0000 0000 0000 0000 0020 0840 0880"
	$"0900 0A00 0C00 0FC0 0000 0000 0000 0000"
	$"000F 0000"
};

data 'CURS' (3007, "bottom_right_corner") {
	$"0000 0000 0000 0000 0003 0403 0213 0113"
	$"0093 0053 0033 03F3 0003 0003 0FFF 0FFF"
	$"0000 0000 0000 0000 0000 0400 0210 0110"
	$"0090 0050 0030 03F0 0000 0000 0000 0000"
	$"000F 000F"
};

data 'CURS' (3008, "bottom_side") {
	$"0000 0000 0100 0100 0100 0100 0100 1110"
	$"0920 0540 0380 0100 0000 7FFC 7FFC 0000"
	$"0000 0000 0100 0100 0100 0100 0100 1110"
	$"0920 0540 0380 0100 0000 0000 0000 0000"
	$"000B 0007"
};

data 'CURS' (3009, "bottom_tee") {
	$"0000 0000 0000 0180 0180 0180 0180 0180"
	$"0180 0180 7FFE 7FFE 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"000B 0007"
};

data 'CURS' (3010, "box_spiral") {
	$"FFFE 8000 BFFE A002 AFFA A80A ABEA AA2A"
	$"AAAA ABAA A82A AFEA A00A BFFA 8002 FFFE"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0008 0008"
};

data 'CURS' (3011, "center_ptr") {
	$"0000 0300 0300 0780 0780 0FC0 0FC0 1FE0"
	$"1FE0 3330 2310 0300 0300 0300 0300 0000"
	$"0300 0780 0780 0FC0 0FC0 1FE0 1FE0 3FF0"
	$"3FF0 7FF8 77B8 6798 0780 0780 0780 0780"
	$"0001 0006"
};

data 'CURS' (3012, "circle") {
	$"0000 03C0 0FF0 1FF8 3C3C 381C 700E 700E"
	$"700E 700E 381C 3C3C 1FF8 0FF0 03C0 0000"
	$"03C0 0FF0 1FF8 3FFC 7FFE 7C3E F81F F81F"
	$"F81F F81F 7C3E 7FFE 3FFC 1FF8 0FF0 03C0"
	$"0007 0007"
};

data 'CURS' (3013, "clock") {
	$"1FF8 33CC 6466 4992 4F12 4422 63C6 3FFC"
	$"2994 2994 2994 2BD4 6996 781E 7FFE 7FFE"
	$"1FF8 3FFC 7FFE 7FFE 7FFE 7FFE 7FFE 3FFC"
	$"3FFC 3FFC 3FFC 3FFC 7FFE 7FFE 7FFE 7FFE"
	$"0004 0008"
};

data 'CURS' (3014, "coffee_mug") {
	$"03F8 0C06 1001 1C07 33F9 7001 D001 9001"
	$"960D DA55 7A55 36ED 10A1 1001 0802 07FC"
	$"03F8 0FFE 1FFF 1FFF 3FFF 7FFF FFFF FFFF"
	$"FFFF FFFF 7FFF 3FFF 1FFF 1FFF 0FFE 07FC"
	$"0004 0003"
};

data 'CURS' (3015, "cross") {
	$"0280 0280 0280 0280 0280 0280 FEFE 0000"
	$"FEFE 0280 0280 0280 0280 0280 0280 0000"
	$"0380 0380 0380 0380 0380 0380 FFFE FFFE"
	$"FFFE 0380 0380 0380 0380 0380 0380 0000"
	$"0007 0007"
};

data 'CURS' (3016, "cross_reverse") {
	$"4284 A28A 5294 2AA8 16D0 0AA0 FD7E 0280"
	$"FD7E 0AA0 16D0 2AA8 5294 A28A 4284 0000"
	$"4384 E38E 739C 3BB8 1FF0 0FE0 FFFE FFFE"
	$"FFFE 0FE0 1FF0 3BB8 739C E38E 4384 0000"
	$"0007 0007"
};

data 'CURS' (3017, "crosshair") {
	$"0100 0100 0100 0100 0100 0100 0100 FEFE"
	$"0100 0100 0100 0100 0100 0100 0100 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0007 0007"
};

data 'CURS' (3018, "diamond_cross") {
	$"0280 06C0 0AA0 1290 2288 4284 FEFE 0000"
	$"FEFE 4284 2288 1290 0AA0 06C0 0280 0000"
	$"0280 06C0 0EE0 1EF0 3EF8 7EFC FEFE 0000"
	$"FEFE 7EFC 3EF8 1EF0 0EE0 06C0 0280 0000"
	$"0007 0007"
};

data 'CURS' (3019, "dot") {
	$"0000 0000 0780 1FE0 1FE0 3FF0 3FF0 3FF0"
	$"3FF0 1FE0 1FE0 0780 0000 0000 0000 0000"
	$"0000 0780 1FE0 3FF0 3FF0 7FF8 7FF8 7FF8"
	$"7FF8 3FF0 3FF0 1FE0 0780 0000 0000 0000"
	$"0006 0006"
};

data 'CURS' (3020, "dotbox") {
	$"0000 0000 3FFC 2004 2004 2004 2004 2184"
	$"2184 2004 2004 2004 2004 3FFC 0000 0000"
	$"0000 0000 3FFC 3FFC 300C 300C 318C 33CC"
	$"33CC 318C 300C 300C 3FFC 3FFC 0000 0000"
	$"0007 0007"
};

data 'CURS' (3021, "double_arrow") {
	$"0000 0180 03C0 07E0 0DB0 1998 0180 0180"
	$"0180 0180 1998 0DB0 07E0 03C0 0180 0000"
	$"0180 03C0 07E0 0FF0 1FF8 3FFC 3BDC 03C0"
	$"03C0 3BDC 3FFC 1FF8 0FF0 07E0 03C0 0180"
	$"0007 0007"
};

data 'CURS' (3022, "draft_large") {
	$"0000 0002 000C 003C 00F8 03F8 0FF0 00F0"
	$"0160 0260 0440 0840 1000 2000 4000 0000"
	$"0003 000F 003E 00FE 03FC 0FFC 3FF8 FFF8"
	$"03F0 07F0 0EE0 1CE0 38C0 70C0 E080 4080"
	$"0001 000E"
};

data 'CURS' (3023, "draft_small") {
	$"0000 0002 000C 003C 00F8 03F8 0070 00B0"
	$"0120 0220 0400 0800 1000 2000 4000 0000"
	$"0003 000F 003E 00FE 03FC 0FFC 3FF8 01F8"
	$"03F0 0770 0E60 1C60 3840 7040 E000 4000"
	$"0001 000E"
};

data 'CURS' (3024, "draped_box") {
	$"0000 0000 3FFC 2244 2664 2C34 381C 2184"
	$"2184 381C 2C34 2664 2244 3FFC 0000 0000"
	$"0000 0000 3FFC 3E7C 3E7C 3C3C 399C 23C4"
	$"23C4 399C 3C3C 3E7C 3E7C 3FFC 0000 0000"
	$"0007 0007"
};

data 'CURS' (3025, "exchange") {
	$"0000 47C0 6FE0 7C30 4810 4C00 7E00 0000"
	$"0000 00FC 0064 1024 187C 0FEC 07C4 0000"
	$"C7C0 EFE0 FFF0 FFF8 FC38 FE10 FF00 FF80"
	$"03FE 01FE 10FE 387E 3FFE 1FFE 0FEE 07C6"
	$"0007 0007"
};

data 'CURS' (3026, "fleur") {
	$"0000 0180 03C0 07E0 0180 1188 318C 7FFE"
	$"7FFE 318C 1188 0180 07E0 03C0 0180 0000"
	$"0180 03C0 07E0 0FF0 17E8 3BDC 7FFE FFFF"
	$"FFFF 7FFE 3BDC 17E8 0FF0 07E0 03C0 0180"
	$"0007 0007"
};

data 'CURS' (3027, "gobbler") {
	$"0000 0078 0070 4036 4FB0 7FF0 7E30 7C30"
	$"3038 00F0 0FE0 0400 0400 0400 0F00 0000"
	$"00FC 00FC E0FF FFFF FFFF FFF8 FFF8 FFF8"
	$"FFFC 7FFC 3FF8 1FF0 0E00 1F80 1F80 1F80"
	$"0003 000E"
};

data 'CURS' (3028, "gumby") {
	$"3F00 10C0 C820 EAA0 C820 CBA0 F838 383E"
	$"0826 0826 092E 0926 0920 1110 2108 3EF8"
	$"3F00 1FC0 CFE0 EFE0 CFE0 CFE0 FFF8 3FFE"
	$"0FE6 0FE6 0FEE 0FE6 0FE0 1FF0 3FF8 3EF8"
	$"0000 0002"
};

data 'CURS' (3029, "hand1") {
	$"000C 003C 00F0 01E0 03C0 07E0 0FF0 2FE0"
	$"7FF0 5FF0 07E0 07C0 4A00 6200 3400 1800"
	$"000C 003C 00F0 01E0 03C0 07E0 0FF0 2FE0"
	$"7FF0 7FF0 7FE0 7FC0 7E00 7E00 3C00 1800"
	$"0000 000D"
};

data 'CURS' (3030, "hand2") {
	$"0000 3FC0 4020 3F10 0808 0708 0808 0714"
	$"0822 0641 0182 0124 0088 0050 0020 0000"
	$"0000 3FC0 7FE0 3FF0 0FF8 07F8 0FF8 07FC"
	$"0FFE 07FF 01FE 01FC 00F8 0070 0020 0000"
	$"0002 0001"
};

data 'CURS' (3031, "heart") {
	$"0000 3EF8 638C C106 8002 8002 8002 8002"
	$"C006 600C 3018 1830 0C60 06C0 0380 0000"
	$"0000 3EF8 7FFC FFFE FFFE FFFE FFFE FFFE"
	$"FFFE 7FFC 3FF8 1FF0 0FE0 07C0 0380 0000"
	$"0003 0007"
};

data 'CURS' (3032, "icon") {
	$"FFFF D555 AAAB D555 A00B D005 A00B D005"
	$"A00B D005 A00B D005 AAAB D555 AAAB FFFF"
	$"FFFF FFFF FFFF FFFF F00F F00F F00F F00F"
	$"F00F F00F F00F F00F FFFF FFFF FFFF FFFF"
	$"0007 0007"
};

data 'CURS' (3033, "iron_cross") {
	$"0000 3FFC 1FF8 4FF2 67E6 73CE 799E 7FFE"
	$"7FFE 799E 73CE 67E6 4FF2 1FF8 3FFC 0000"
	$"7FFE 7FFE FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 7FFE 7FFE"
	$"0007 0006"
};

data 'CURS' (3034, "left_ptr") {
	$"0000 0800 0C00 0E00 0F00 0F80 0FC0 0FE0"
	$"0FF0 0F80 0D80 08C0 00C0 0060 0060 0000"
	$"1800 1C00 1E00 1F00 1F80 1FC0 1FE0 1FF0"
	$"1FF8 1FFC 1FC0 1DE0 19E0 10F0 00F0 0070"
	$"0001 0004"
};

data 'CURS' (3035, "left_side") {
	$"0000 6000 6000 6080 6100 6200 6400 6FFC"
	$"6400 6200 6100 6080 6000 6000 0000 0000"
	$"0000 0000 0000 0080 0100 0200 0400 0FFC"
	$"0400 0200 0100 0080 0000 0000 0000 0000"
	$"0007 0004"
};

data 'CURS' (3036, "left_tee") {
	$"0000 0C00 0C00 0C00 0C00 0C00 0C00 0FF8"
	$"0FF8 0C00 0C00 0C00 0C00 0C00 0C00 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0007 0004"
};

data 'CURS' (3037, "leftbutton") {
	$"8002 7FFC 7FFC 4444 4554 4554 4554 4554"
	$"4444 7FFC 7FFC 7FFC 7FFC 7FFC 7FFC 8002"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"0004 0003"
};

data 'CURS' (3038, "ll_angle") {
	$"0000 0000 0000 0C00 0C00 0C00 0C00 0C00"
	$"0C00 0C00 0FF8 0FF8 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"000B 0004"
};

data 'CURS' (3039, "lr_angle") {
	$"0000 0000 0000 0030 0030 0030 0030 0030"
	$"0030 0030 1FF0 1FF0 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"000B 000B"
};

data 'CURS' (3040, "man") {
	$"0380 1EF0 0280 8100 4387 244B 1D70 0540"
	$"0440 0280 0440 0920 1290 1450 783C F83F"
	$"0380 1FF0 0380 8100 4387 27CB 1FF0 07C0"
	$"07C0 0380 07C0 0FE0 1EF0 1C70 783C F83F"
	$"0001 0007"
};

data 'CURS' (3041, "middlebutton") {
	$"8002 7FFC 7FFC 4444 5454 5454 5454 5454"
	$"4444 7FFC 7FFC 7FFC 7FFC 7FFC 7FFC 8002"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"0004 0007"
};

data 'CURS' (3042, "mouse") {
	$"0600 0100 0180 0FF0 1008 17E8 1428 1428"
	$"17E8 1008 1008 1008 1008 1008 1008 0FF0"
	$"0600 0100 0180 0FF0 1FF8 1FF8 1FF8 1FF8"
	$"1FF8 1FF8 1FF8 1FF8 1FF8 1FF8 1FF8 0FF0"
	$"0000 0000"
};

data 'CURS' (3043, "pencil") {
	$"0000 00F0 0088 0108 0190 0270 0220 0440"
	$"0440 0880 0880 1100 1E00 1C00 1800 1000"
	$"0000 00F0 00F8 01F8 01F0 03F0 03E0 07C0"
	$"07C0 0F80 0F80 1F00 1E00 1C00 1800 1000"
	$"000F 0003"
};

data 'CURS' (3044, "pirate") {
	$"03C0 07E0 0FF0 1998 1998 0FF0 07E0 03C0"
	$"43C2 43C3 2184 1C38 03C0 0FF1 781F 4002"
	$"07E0 0FF0 1FF8 3FFC 3FFC 1FF8 0FF0 47E2"
	$"E7E7 E7E7 7FFF 3FFC 1FF9 7FFF FFFF F81F"
	$"000A 0007"
};

data 'CURS' (3045, "plus") {
	$"0000 0000 0000 0180 0180 0180 0180 1FF8"
	$"1FF8 0180 0180 0180 0180 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0007 0007"
};

data 'CURS' (3046, "question_arrow") {
	$"07C0 0FE0 1C70 1830 1C30 0C70 00E0 03C0"
	$"0380 0280 0280 0EE0 06C0 0380 0100 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 3FF8 1FF0 0FE0 07C0 0380 0100"
	$"000E 0007"
};

data 'CURS' (3047, "right_ptr") {
	$"0000 0010 0030 0070 00F0 01F0 03F0 07F0"
	$"0FF0 01F0 01B0 0310 0300 0600 0600 0000"
	$"0018 0038 0078 00F8 01F8 03F8 07F8 0FF8"
	$"1FF8 3FF8 03F8 07B8 0798 0F08 0F00 0E00"
	$"0001 000B"
};

data 'CURS' (3048, "right_side") {
	$"0000 0000 0006 0006 0106 0086 0046 0026"
	$"3FF6 0026 0046 0086 0106 0006 0006 0000"
	$"0000 0000 0000 0000 0100 0080 0040 0020"
	$"3FF0 0020 0040 0080 0100 0000 0000 0000"
	$"0008 000B"
};

data 'CURS' (3049, "right_tee") {
	$"0000 0030 0030 0030 0030 0030 0030 1FF0"
	$"1FF0 0030 0030 0030 0030 0030 0030 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0007 000A"
};

data 'CURS' (3050, "rightbutton") {
	$"8002 7FFC 7FFC 4444 5544 5544 5544 5544"
	$"4444 7FFC 7FFC 7FFC 7FFC 7FFC 7FFC 8002"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"FFFE FFFE FFFE FFFE FFFE FFFE FFFE FFFE"
	$"0004 0003"
};

data 'CURS' (3051, "rtl_logo") {
	$"0000 7FFE 4022 4022 4022 7FE2 4422 4422"
	$"4422 4422 47FE 4402 4402 4402 7FFE 0000"
	$"0000 7FFE 7FFE 6076 7FF6 7FF6 7C36 6C36"
	$"6C36 6C3E 6FFE 6FFE 6E06 7FFE 7FFE 0000"
	$"0007 0007"
};

data 'CURS' (3052, "sailboat") {
	$"0000 0040 0040 0160 0160 0360 0370 0770"
	$"0770 0F78 0F78 1F78 1F7C 3E38 0000 0000"
	$"0040 00E0 01E0 03F0 03F0 07F0 07F8 0FF8"
	$"0FF8 1FFC 1FFC 3FFC 3FFE 7F7C 7E38 0000"
	$"000C 0008"
};

data 'CURS' (3053, "sb_down_arrow") {
	$"0280 0280 0280 0280 0280 0280 0280 0280"
	$"0280 0280 0280 0FE0 07C0 0380 0100 0000"
	$"0380 0380 0380 0380 0380 0380 0380 0380"
	$"0380 0380 0380 1FF0 0FE0 07C0 0380 0100"
	$"000E 0007"
};

data 'CURS' (3054, "sb_h_double_arrow") {
	$"0000 0000 0000 0000 0810 1818 3FFC 781E"
	$"3FFC 1818 0810 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0810 1818 381C 7FFE FFFF"
	$"7FFE 381C 1818 0810 0000 0000 0000 0000"
	$"0007 0007"
};

data 'CURS' (3055, "sb_left_arrow") {
	$"0000 0000 0000 0000 0800 1800 3FFF 7800"
	$"3FFF 1800 0800 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0800 1800 3800 7FFF FFFF"
	$"7FFF 3800 1800 0800 0000 0000 0000 0000"
	$"0007 0001"
};

data 'CURS' (3056, "sb_right_arrow") {
	$"0000 0000 0000 0000 0000 0010 0018 FFFC"
	$"001E FFFC 0018 0010 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0010 0018 001C FFFE"
	$"FFFF FFFE 001C 0018 0010 0000 0000 0000"
	$"0008 000E"
};

data 'CURS' (3057, "sb_up_arrow") {
	$"0000 0080 01C0 03E0 07F0 0140 0140 0140"
	$"0140 0140 0140 0140 0140 0140 0140 0140"
	$"0080 01C0 03E0 07F0 0FF8 01C0 01C0 01C0"
	$"01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0"
	$"0001 0008"
};

data 'CURS' (3058, "sb_v_double_arrow") {
	$"0000 0100 0380 07C0 0FE0 0280 0280 0280"
	$"0280 0280 0280 0FE0 07C0 0380 0100 0000"
	$"0100 0380 07C0 0FE0 1FF0 0380 0380 0380"
	$"0380 0380 0380 1FF0 0FE0 07C0 0380 0100"
	$"0007 0007"
};

data 'CURS' (3059, "shuttle") {
	$"0020 0070 00F8 01DE 05DE 09DE 11DE 11DE"
	$"11DE 11DE 31DE 71DE FDDE 1888 0078 0030"
	$"0020 0070 00F8 01FE 07FE 0FFE 1FFE 1FFE"
	$"1FFE 1FFE 3FFE 7FFE FFFE 18F8 0078 0030"
	$"0000 000A"
};

data 'CURS' (3060, "sizing") {
	$"0000 7F80 4000 4000 4000 47E0 4420 4422"
	$"4422 0422 07E2 0012 000A 0006 01FE 0000"
	$"FFC0 FFC0 FFC0 E000 EFF0 EFF0 EC37 EC37"
	$"EC37 EC37 0FF7 0FFF 001F 03FF 03FF 03FF"
	$"000E 000E"
};

data 'CURS' (3061, "spider") {
	$"2010 1020 1020 0840 0840 8787 6798 1FE0"
	$"1FE0 6798 8787 0840 0840 1020 1020 2010"
	$"7038 3870 3870 1CE0 9FE7 EFDF FFFF 7FF8"
	$"7FF8 FFFF EFDF 9FE7 1CE0 3870 3870 7038"
	$"0007 0007"
};

data 'CURS' (3062, "spraycan") {
	$"0018 0040 0D18 1E40 1A18 3F00 2100 3900"
	$"2900 3900 2900 3900 3900 2100 2100 3F00"
	$"0000 0000 0C00 1E00 1E00 3F00 3F00 3F00"
	$"3F00 3F00 3F00 3F00 3F00 3F00 3F00 3F00"
	$"0002 0007"
};

data 'CURS' (3063, "star") {
	$"0100 0280 0280 0280 0440 0440 0440 3938"
	$"C006 3838 0920 1290 2448 2828 3018 2008"
	$"0100 0380 0380 0380 07C0 07C0 07C0 3FF8"
	$"FFFE 3FF8 0FE0 1EF0 3C78 3838 3018 2008"
	$"0007 0007"
};

data 'CURS' (3064, "target") {
	$"0000 0380 0FE0 1C70 3018 600C C106 C286"
	$"C106 600C 3018 1C70 0FE0 0380 0000 0000"
	$"0000 0380 0FE0 1FF0 3C78 701C E38E E38E"
	$"E38E 701C 3C78 1FF0 0FE0 0380 0000 0000"
	$"0007 0007"
};

data 'CURS' (3065, "tcross") {
	$"0100 0100 0100 0100 0100 0100 0100 FFFE"
	$"0100 0100 0100 0100 0100 0100 0100 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0007 0007"
};

data 'CURS' (3066, "top_left_arrow") {
	$"0000 6000 7800 3E00 3F80 1FE0 1E00 0D00"
	$"0C80 0440 0420 0010 0008 0004 0000 0000"
	$"E000 F800 FE00 7F80 7FE0 3FF8 3FFE 1F80"
	$"1FC0 0EE0 0E70 0638 061C 020E 0204 0000"
	$"0001 0001"
};

data 'CURS' (3067, "top_left_corner") {
	$"FFF0 FFF0 C000 C000 CFC0 CC00 CA00 C900"
	$"C880 C840 C020 C000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0FC0 0C00 0A00 0900"
	$"0880 0840 0020 0000 0000 0000 0000 0000"
	$"0000 0000"
};

data 'CURS' (3068, "top_right_corner") {
	$"0FFF 0FFF 0003 0003 03F3 0033 0053 0093"
	$"0113 0213 0403 0003 0000 0000 0000 0000"
	$"0000 0000 0000 0000 03F0 0030 0050 0090"
	$"0110 0210 0400 0000 0000 0000 0000 0000"
	$"0000 000F"
};

data 'CURS' (3069, "top_side") {
	$"0000 7FFC 7FFC 0000 0100 0380 0540 0920"
	$"1110 0100 0100 0100 0100 0100 0000 0000"
	$"0000 0000 0000 0000 0100 0380 0540 0920"
	$"1110 0100 0100 0100 0100 0100 0000 0000"
	$"0004 0007"
};

data 'CURS' (3070, "top_tee") {
	$"0000 0000 0000 0000 7FFE 7FFE 0180 0180"
	$"0180 0180 0180 0180 0180 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0004 0007"
};

data 'CURS' (3071, "trek") {
	$"0100 0000 0380 07C0 0FE0 0EE0 0FE0 07C0"
	$"0380 0100 0BA0 0D60 0920 0820 0820 0000"
	$"0000 0380 07C0 0FE0 1FF0 1FF0 1FF0 0FE0"
	$"07C0 0BA0 1FF0 1FF0 1FF0 1D70 1C70 0820"
	$"0000 0007"
};

data 'CURS' (3072, "ul_angle") {
	$"0000 0000 0000 0FF8 0FF8 0C00 0C00 0C00"
	$"0C00 0C00 0C00 0C00 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0003 0004"
};

data 'CURS' (3073, "umbrella") {
	$"0000 0890 0228 49A6 27C8 1930 610C 0100"
	$"0100 0100 0100 0100 0140 0140 0080 0000"
	$"0000 0FF0 1FF8 7FFE 7FFC FFFE FBBE E38E"
	$"0380 0380 0380 03C0 03E0 03E0 01C0 0080"
	$"0004 0007"
};

data 'CURS' (3074, "ur_angle") {
	$"0000 0000 0000 0000 1FF0 1FF0 0030 0030"
	$"0030 0030 0030 0030 0030 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0004 000B"
};

data 'CURS' (3075, "watch") {
	$"07E0 07E0 07E0 07E0 0810 1088 1088 108C"
	$"138C 1008 1008 0810 07E0 07E0 07E0 07E0"
	$"07E0 07E0 07E0 07E0 0FF0 1FF8 1FF8 1FFC"
	$"1FFC 1FF8 1FF8 0FF0 07E0 07E0 07E0 07E0"
	$"0008 000D"
};

data 'CURS' (3076, "xterm") {
	$"0C60 0280 0100 0100 0100 0100 0100 0100"
	$"0100 0100 0100 0100 0100 0100 0280 0C60"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"000B 0007"
};

/*
 * The following are color versions of some of the
 * cursors defined above. The color cursors will be
 * used if the exist in preference to the black & white
 * cursors.
 */

data 'crsr' (3004, "boat", purgeable) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0100 03C0"
	$"8460 FFFF 0018 0020 0040 FFC0 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0100 03C0"
	$"87E0 FFFF FFF8 FFE0 FFC0 FFC0 0000 0000"
	$"0000 0000 0007 000F 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 000F 0000 0000 0000 00FF FF00"
	$"0000 F000 0F32 25F0 0000 6FFF FFFF FFFF"
	$"FFFF 2222 2222 221F F000 2222 2222 21F0"
	$"0000 3333 3333 4F00 0000 FFFF FFFF FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0007 0000 FFFF FFFF"
	$"FFFF 0001 BBBB BBBB BBBB 0002 EEEE EEEE"
	$"EEEE 0003 DDDD DDDD DDDD 0004 CCCC CCCC"
	$"CCCC 0005 4444 4444 4444 0006 1111 1111"
	$"1111 000F 0000 0000 0000"
};

data 'crsr' (3013, "clock") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 1FF8 33CC 6466 4992 4F12 4422"
	$"63C6 3FFC 2994 2994 2994 2BD4 6996 781E"
	$"7FFE 7FFE 1FF8 3FFC 7FFE 7FFE 7FFE 7FFE"
	$"7FFE 3FFC 3FFC 3FFC 3FFC 3FFC 7FFE 7FFE"
	$"7FFE 7FFE 0004 0008 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 000F FFFF FFFF F000 00F6 05FF FF50"
	$"6F00 0F60 5F00 56F5 06F0 0F00 F021 F30F"
	$"00F0 0F00 F6F1 000F 00F0 0F00 5F00 00F5"
	$"00F0 0F60 05FF FF50 06F0 00FF FFFF FFFF"
	$"FF00 00F0 F001 100F 0F00 00F0 F001 100F"
	$"0F00 00F0 F021 120F 0F00 00F0 F01F F10F"
	$"0F00 0FF0 F021 120F 0FF0 0FF4 F500 005F"
	$"4FF0 0FFF FFFF FFFF FFF0 0FFF FFFF FFFF"
	$"FFF0 0000 0000 0000 0007 0000 FFFF FFFF"
	$"FFFF 0001 4444 4444 4444 0002 AAAA AAAA"
	$"AAAA 0003 EEEE EEEE EEEE 0004 5555 5555"
	$"5555 0005 DDDD DDDD DDDD 0006 7777 7777"
	$"7777 000F 0000 0000 0000"
};

data 'crsr' (3014, "coffee_mug") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 03F8 0C06 1001 1C07 33F9 7001"
	$"D001 9001 960D DA55 7A55 36ED 10A1 1001"
	$"0802 07FC 03F8 0FFE 1FFF 1FFF 3FFF 7FFF"
	$"FFFF FFFF FFFF FFFF 7FFF 3FFF 1FFF 1FFF"
	$"0FFE 07FC 0004 0003 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 00FF FFFF F000 0000 FF42 2222"
	$"4FF0 000F 4221 1111 224F 000F FF11 1111"
	$"1FFF 00FF 24FF FFFF F42F 0F5F 2222 2222"
	$"222F F52F 2222 2222 222F F40F 2222 2222"
	$"222F F40F 4FF2 2224 FF2F F52F F2F2 2F2F"
	$"2F2F 0F5F F2F2 535F 2F2F 00FF 4FF2 F3F4"
	$"FF2F 000F 2222 F2F2 222F 000F 4222 2222"
	$"224F 0000 F422 2222 24F0 0000 0FFF FFFF"
	$"FF00 0000 0000 0000 0006 0000 FFFF FFFF"
	$"FFFF 0001 CCCC 9999 6666 0002 CCCC CCCC"
	$"FFFF 0003 3333 3333 6666 0004 9999 9999"
	$"FFFF 0005 6666 6666 CCCC 000F 0000 0000"
	$"0000"
};

data 'crsr' (3027, "gobbler") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0000 0078 0070 4036 4FB0 7FF0"
	$"7E30 7C30 3038 00F0 0FE0 0400 0400 0400"
	$"0F00 0000 00FC 00FC E0FF FFFF FFFF FFF8"
	$"FFF8 FFF8 FFFC 7FFC 3FF8 1FF0 0E00 1F80"
	$"1F80 1F80 0003 000E 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0222"
	$"2000 0000 0000 0111 0000 0300 0000 0011"
	$"0220 0100 1616 1011 0000 0361 6111 1111"
	$"0000 0111 1114 4415 0000 0311 1144 4451"
	$"0000 0011 4444 4415 1000 0004 4444 5151"
	$"0000 0000 1515 1510 0000 0000 0200 0000"
	$"0000 0000 0300 0000 0000 0000 0200 0000"
	$"0000 0000 2323 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0006 0000 FFFF FFFF"
	$"FFFF 0001 CCCC 9999 6666 0002 DDDD 0000"
	$"0000 0003 FFFF 6666 3333 0004 CCCC CCCC"
	$"CCCC 0005 8888 8888 8888 0006 FFFF CCCC"
	$"9999"
};

data 'crsr' (3028, "gumby") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 3F00 10C0 C820 EAA0 C820 CBA0"
	$"F838 383E 0826 0826 092E 0926 0920 1110"
	$"2108 3EF8 3F00 1FC0 CFE0 EFE0 CFE0 CFE0"
	$"FFF8 3FFE 0FE6 0FE6 0FEE 0FE6 0FE0 1FF0"
	$"3FF8 3EF8 0000 0002 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 00FF FFFF 0000 0000 000F 1212 FF00"
	$"0000 FF00 F131 31F0 0000 FFF0 F3F3 F3F0"
	$"0000 FF00 F131 31F0 0000 FF00 F2FF F2F0"
	$"0000 4FFF F121 21FF F000 00FF F212 12FF"
	$"FF40 0000 F121 21F0 0FF0 0000 F212 12F0"
	$"0FF0 0000 F12F 21F0 FFF0 0000 F21F 12F0"
	$"0FF0 0000 F12F 21F0 0000 000F 121F 121F"
	$"0000 00F1 212F 2121 F000 00FF FFF0 FFFF"
	$"F000 0000 0000 0000 0005 0000 FFFF FFFF"
	$"FFFF 0001 0000 BBBB 0000 0002 CCCC CCCC"
	$"CCCC 0003 AAAA AAAA AAAA 0004 4444 4444"
	$"4444 000F 0000 0000 0000"
};

data 'crsr' (3031, "heart") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0000 3EF8 638C C106 8002 8002"
	$"8002 8002 C006 600C 3018 1830 0C60 06C0"
	$"0380 0000 0000 3EF8 7FFC FFFE FFFE FFFE"
	$"FFFE FFFE FFFE 7FFC 3FF8 1FF0 0FE0 07C0"
	$"0380 0000 0003 0007 0000 0000 0000 0000"
	$"0000 0000 8004 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0002 0001 0002 0000 0000 0000 00D2 0000"
	$"0000 0000 0000 0FFC FFC0 3AAB AA70 E99B"
	$"999C E665 A65C E999 999C E666 665C E999"
	$"999C D666 665C 3599 9970 0D66 65C0 0359"
	$"9700 00D6 5C00 0035 7000 000F C000 0000"
	$"0000 0000 0000 0000 0003 0000 FFFF FFFF"
	$"FFFF 0001 DDDD 0000 0000 0002 FFFF 6666"
	$"CCCC 0003 0000 0000 0000"
};

data 'crsr' (3042, "mouse", purgeable) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 BE00 0100 0180 0FF0 1008 17E8"
	$"1428 1428 17E8 1008 1008 1008 1008 1008"
	$"1008 0FF0 FE00 0100 0180 0FF0 1FF8 1FF8"
	$"1FF8 1FF8 1FF8 1FF8 1FF8 1FF8 1FF8 1FF8"
	$"1FF8 0FF0 0001 0007 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 1379 4AF0 0000 0000 0000 000F 0000"
	$"0000 0000 000F F000 0000 0000 FFFF FFFF"
	$"0000 000F 2111 1112 F000 000F 3655 5563"
	$"F000 000F 3513 1351 F000 000F 3533 3351"
	$"F000 000F 3655 5561 F000 000F 3311 1111"
	$"F000 000F 3333 3333 F000 000F 3333 3333"
	$"F000 000F 2222 2222 F000 000F 8888 8888"
	$"F000 000F 7888 8887 F000 0000 FFFF FFFF"
	$"0000 0000 0000 0000 000B 0000 FFFF FFFF"
	$"FFFF 0001 EEEE EEEE EEEE 0002 CCCC CCCC"
	$"CCCC 0003 DDDD DDDD DDDD 0004 4444 4444"
	$"4444 0005 2222 2222 2222 0006 5555 5555"
	$"5555 0007 AAAA AAAA AAAA 0008 BBBB BBBB"
	$"BBBB 0009 7777 7777 7777 000A 1111 1111"
	$"1111 000F 0000 0000 0000"
};

data 'crsr' (3043, "pencil", purgeable) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0000 00F0 0088 0108 0190 0270"
	$"0220 0440 0440 0880 0880 1100 1E00 1C00"
	$"1800 1000 0000 00F0 00F8 01F8 01F0 03F0"
	$"03E0 07C0 07C0 0F80 0F80 1F00 1E00 1C00"
	$"1800 1000 000F 0003 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0000 0000 0000 0000 0000 FFFF"
	$"0000 0000 0000 F404 F000 0000 000F 4042"
	$"F000 0000 000F F42F 0000 0000 00F5 3FFF"
	$"0000 0000 00F3 52F0 0000 0000 0F35 1F00"
	$"0000 0000 0F53 2F00 0000 0000 F532 F000"
	$"0000 0000 F312 F000 0000 000F 352F 0000"
	$"0000 000F FFF0 0000 0000 000F FF00 0000"
	$"0000 000F F000 0000 0000 000F 0000 0000"
	$"0000 0000 0000 0000 0006 0000 FFFF FFFF"
	$"FFFF 0001 CCCC CCCC CCCC 0002 8888 8888"
	$"8888 0003 FFFF FFFF 0000 0004 DDDD 0000"
	$"0000 0005 FFFF 6666 3333 000F 0000 0000"
	$"0000"
};

data 'crsr' (3059, "shuttle") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0020 0070 00F8 01DE 05DE 09DE"
	$"11DE 11DE 11DE 11DE 31DE 71DE FDDE 1888"
	$"0078 0030 0020 0070 00F8 01FE 07FE 0FFE"
	$"1FFE 1FFE 1FFE 1FFE 3FFE 7FFE FFFE 18F8"
	$"0078 0030 0000 000A 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0000 00F0 0000 0000 0000 0F3F"
	$"0000 0000 0000 F343 F000 0000 000F 3404"
	$"3FF0 0000 0F4F 3404 3FF0 0000 F55F 3404"
	$"3FF0 000F 505F 3404 3FF0 000F 005F 3404"
	$"3FF0 000F 005F 3404 3FF0 000F 005F 3404"
	$"3FF0 00F3 005F 3404 3FF0 0F33 505F 3404"
	$"3FF0 FFF3 3F4F 3404 3FF0 000F F000 1222"
	$"1000 0000 0000 0111 1000 0000 0000 0011"
	$"0000 0000 0000 0000 0006 0000 FFFF FFFF"
	$"FFFF 0001 FFFF 6666 3333 0002 DDDD 0000"
	$"0000 0003 4444 4444 4444 0004 8888 8888"
	$"8888 0005 DDDD DDDD DDDD 000F 0000 0000"
	$"0000"
};

data 'crsr' (3062, "spraycan") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0018 0040 0D18 1E40 1A18 3F00"
	$"2100 3900 2900 3900 2900 3900 3900 2100"
	$"2100 3F00 0000 0000 0C00 1E00 1E00 3F00"
	$"3F00 3F00 3F00 3F00 3F00 3F00 3F00 3F00"
	$"3F00 3F00 0002 0007 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0000 0005 2000 0000 0000 0460"
	$"0000 0000 FF1F 6005 2000 000F 33F0 0460"
	$"0000 000F 10F0 0005 2000 00FF FFFF 0000"
	$"0000 00F8 170F 0000 0000 00F5 F70F 0000"
	$"0000 00FA F70F 0000 0000 00F9 F70F 0000"
	$"0000 00FA F70F 0000 0000 00F9 F70F 0000"
	$"0000 00F5 F70F 0000 0000 00F8 170F 0000"
	$"0000 00F8 170F 0000 0000 00FF FFFF 0000"
	$"0000 0000 0000 0000 000B 0000 FFFF FFFF"
	$"FFFF 0001 AAAA AAAA AAAA 0002 7777 7777"
	$"7777 0003 5555 5555 5555 0004 2222 2222"
	$"2222 0005 4444 4444 4444 0006 BBBB BBBB"
	$"BBBB 0007 DDDD DDDD DDDD 0008 EEEE EEEE"
	$"EEEE 0009 6666 6666 CCCC 000A CCCC CCCC"
	$"FFFF 000F 0000 0000 0000"
};

data 'crsr' (3063, "star") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0100 0280 0280 0280 0440 0440"
	$"0440 3938 C006 3838 0920 1290 2448 2828"
	$"3018 2008 0100 0380 0380 0380 07C0 07C0"
	$"07C0 3FF8 FFFE 3FF8 0FE0 1EF0 3C78 3838"
	$"3018 2008 0007 0007 0000 0000 0000 0000"
	$"0000 0000 8004 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0002 0001 0002 0000 0000 0000 00D2 0000"
	$"0000 0003 0000 000D C000 000D C000 000D"
	$"C000 0035 7000 0035 7000 0035 7000 0FD7"
	$"5FC0 F555 557C 0FD5 5FC0 00D7 5C00 035C"
	$"D700 0D70 35C0 0DC0 0DC0 0F00 03C0 0C00"
	$"00C0 0000 0000 0000 0002 0000 FFFF FFFF"
	$"FFFF 0001 FFFF FFFF 0000 0003 0000 0000"
	$"0000"
};

data 'crsr' (3071, "trek") {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 0100 0000 0380 07C0 0FE0 0EE0"
	$"0FE0 07C0 0380 0100 0BA0 0D60 0920 0820"
	$"0820 0000 0000 0380 07C0 0FE0 1FF0 1FF0"
	$"1FF0 0FE0 07C0 0BA0 1FF0 1FF0 1FF0 1D70"
	$"1C70 0820 0000 0007 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0005 0000 0000 0000 0005 0000"
	$"0000 0000 00FF F000 0000 0000 0F31 3F00"
	$"0000 0000 F322 23F0 0000 0000 F110 11F0"
	$"0000 0000 F311 13F0 0000 0000 0F31 3F00"
	$"0000 0000 00FF F000 0000 0000 000F 0000"
	$"0000 0000 F0FF F0F0 0000 0000 FF0F 0FF0"
	$"0000 0000 400F 0040 0000 0000 4000 0040"
	$"0000 0000 4000 0040 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0006 0000 FFFF FFFF"
	$"FFFF 0001 EEEE EEEE EEEE 0002 9999 9999"
	$"FFFF 0003 DDDD DDDD DDDD 0004 3333 3333"
	$"6666 0005 DDDD 0000 0000 000F 0000 0000"
	$"0000"
};

data 'crsr' (3075, "watch", purgeable) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"
	$"0000 0000 07E0 07E0 07E0 07E0 0810 1088"
	$"1088 108C 138C 1008 1008 0810 07E0 07E0"
	$"07E0 07E0 07E0 07E0 07E0 07E0 0FF0 1FF8"
	$"1FF8 1FF8 1FF8 1FF8 1FF8 0FF0 07E0 07E0"
	$"07E0 07E0 0008 000D 0000 0000 0000 0000"
	$"0000 0000 8008 0000 0000 0010 0010 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0004 0001 0004 0000 0000 0000 0112 0000"
	$"0000 0000 0FFF FFF0 0000 0000 0FFF FFF0"
	$"0000 0000 0FFF FFF0 0000 0000 0FFF FFF0"
	$"0000 0000 F020 202F 0000 000F 0222 F221"
	$"F000 000F 2222 F123 F000 000F 0222 F121"
	$"FF00 000F 22FF F123 FF00 000F 0222 2221"
	$"F000 000F 2222 2213 F000 0000 F131 313F"
	$"0000 0000 0FFF FFF0 0000 0000 0FFF FFF0"
	$"0000 0000 0FFF FFF0 0000 0000 0FFF FFF0"
	$"0000 0000 0000 0000 0004 0000 FFFF FFFF"
	$"FFFF 0001 CCCC CCCC CCCC 0002 EEEE EEEE"
	$"EEEE 0003 BBBB BBBB BBBB 000F 0000 0000"
	$"0000"
};
