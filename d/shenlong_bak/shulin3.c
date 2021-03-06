// /d/shenlong/shulin3.c
// Last Modified by winder on Jul. 12 2002

inherit ROOM;
#include <ansi.h>

void create()
{
        set("short", "小树林");
        set("long", @LONG
这儿是一片小树林，树林中间有一小块空地。空地上除了坑洼
以外还堆了一些不同大小的石块，这些石块表面都比较光滑似乎是
人力所为。你一抬头发现空地四周的树干上都没了树皮。
LONG );

        set("exits", ([
                "southeast" : __DIR__"luanshi",
        ]));
        set("outdoors", "shenlong");
        set("no_clean_up", 0);
        setup();
}

void init()
{
        add_action("do_kick", "kick");
        add_action("do_kick", "ti");
        add_action("do_pick", "take");
        add_action("do_pick", "pick");
}

int do_pick(string arg)
{
        object me = this_player();

        if( !arg || !(arg == "shikuai" || arg == "shitou") )
                return notify_fail("什么？\n");

        if( query_temp("sg/leg", me)>1 )
                return notify_fail("你腿上绑不下那么多石块。\n");

        addn("qi", 200/(1+me->query_str()), me);
        if( random(query_temp("sg/shikuai", me))<3 )
        {
                message_vision("$N捡起一块石头，掂了掂份量，摇了摇头，便扔在地上。\n", me);
                addn_temp("sg/shikuai", 1, me);
                return 1;
        }
        delete_temp("sg/shikuai", me);
        if( !query_temp("sg/leg", me) )
        {
                message_vision("$N捡起一块石头，掂了掂份量，便把它绑在右腿上。\n", me);
                addn_temp("sg/leg", 1, me);
                addn_temp("apply/dodge", -10, me);
                return 1;
        }
        if( query_temp("sg/leg", me)>0 )
        {
                message_vision("$N捡起一块石头，掂了掂份量，便把它绑在左腿上。\n", me);
                addn_temp("apply/dodge", -10, me);
                addn_temp("sg/leg", 1, me);
        }
        return 1;
}

int do_kick(string arg)
{
        object me = this_player();

        if( !arg || !(arg == "tree" || arg == "shu") )
                return notify_fail("什么？\n");

        message_vision("$N腾空而起双腿踢向周围树干。\n", me);

        me->receive_damage("jing", 15, "心力绞瘁死了");
        me->receive_damage("qi", 25, "力尽而死");
//        me->improve_skill("kick", random(me->query_int()));

        if( query_temp("sg/leg", me) == 2 )
        {
                me->receive_damage("jing", 10, "心力绞瘁死了");
                me->receive_damage("qi", 20, "力尽而死");
                me->improve_skill("dodge", random(me->query_int()));
                return 1;
        }
        if( query_temp("sg/leg", me) == 1 )
        {
                me->receive_damage("jing", 8, "心力绞瘁死了");
                me->receive_damage("qi", 16, "力尽而死");
                me->improve_skill("dodge", random(me->query_int() / 2));
                return 1;
        }
        me->improve_skill("dodge", random(me->query_int() / 5));
        return 1;
}

int valid_leave(object me, string dir)
{
        if( query_temp("sg/leg", me) )
        {
                addn_temp("apply/dodge",query_temp("sg/leg",  me)*10, me);
                message_vision("$N松了绑，将腿上的石块扔在一边。\n", me);
                delete_temp("sg/leg", me);
                me->start_busy(1);
        }

        return ::valid_leave(me, dir);
}

