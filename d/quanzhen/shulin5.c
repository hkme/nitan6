// /d/quanzhen/shulin5.c
// Java Oct.10 1998

inherit ROOM;

void create()
{
        set("short", "树林");
        set("long", @LONG
这位于终南山间的一大片树林，生的颇为茂密。走在其间，只听
得脚下沙沙的落叶声，四周昏昏暗暗的，不时几道日光穿过浓密的枝
头，投射而下。偶尔远处几声兽吼声，更添了一分神秘感。往北、南
各有一条小径。
LONG
        );
        set("outdoors", "zhongnan");
        set("exits", ([
                "south"     : __DIR__"shulin4",
                "northdown" : __DIR__"shulin6",
        ]));
        set("coor/x", -3190);
        set("coor/y", 150);
        set("coor/z", 110);
        setup();
}
#include "caiyao.h"
