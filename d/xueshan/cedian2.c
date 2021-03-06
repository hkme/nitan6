#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "侧殿");
        set("long", @LONG
这里是雪山寺的西侧殿。这里的墙壁上描绘着佛祖传道的故事，
佛祖手拈金花宝相庄严。
LONG );
        set("exits", ([
                "northup" : __DIR__"zoulang2",
                "east"    : __DIR__"dadian",
        ]));
        set("objects", ([
                CLASS_D("xueshan")+"/samu" : 1,
                __DIR__"npc/xiang-ke" : 3,
        ]));
        setup();
}

int valid_leave(object me, string dir)
{
        mapping myfam;
        myfam=query("family", me);

        if ((! myfam || myfam["family_name"] != "雪山寺")
           && dir=="northup"
           && objectp(present ("samu huofo", environment(me)))) 
                return notify_fail(CYN "萨木活佛说道：你非雪山寺"
                                   "弟子，不得入内。\n" NOR);

        return ::valid_leave(me,dir);
}