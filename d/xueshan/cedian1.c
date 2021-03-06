#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "侧殿");
        set("long", @LONG
   这里是雪山寺的东侧殿。殿内有神态各异的十八罗汉塑像，金旒
玉镶，很是庄严。
LONG );
        set("exits", ([
                "northup" : __DIR__"zoulang1",
                "west"    : __DIR__"dadian",
        ]));
        set("objects", ([
                CLASS_D("xueshan")+"/jiamu" : 1,
                __DIR__"npc/xiang-ke"       : 3,
        ]));
        setup();

}
int valid_leave(object me, string dir)
{
        mapping myfam;
        myfam=query("family", me);

        if ((! myfam || myfam["family_name"] != "雪山寺")
           && dir=="northup"
           && objectp(present ("jiamu huofo", environment(me))))
          return notify_fail(CYN "嘉木活佛说道：尔非雪山寺弟"
                                   "子，不能入内。\n" NOR);

        return ::valid_leave(me, dir);
}