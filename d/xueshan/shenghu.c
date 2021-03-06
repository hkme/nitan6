#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "圣湖");
        set("long", @LONG
眼前就是著名的圣湖，碧蓝的湖面倒影着蓝天和白云，浩淼的水
面让人心旷神怡。金碧辉煌的雪山寺清晰可见，再远处是几座延绵的
大雪山，其中最高大的是神山，相传山中有神仙居住。雪山中融化的
雪水都汇入湖中。但是周围却是被极浓的雾气所笼罩，使你的眼前一
片朦胧，瞧不清四周。
LONG );
        set("resource/water", 1);
        set("exits", ([
                "east" :  __FILE__,
                "west" :  __FILE__,
                "south" : __FILE__,
                "north" : __FILE__,
        ]));
        set("objects", ([
                "/clone/npc/walker" : 1,
        ]));
        set("outdoors", "xuedao");

        setup();
}

int valid_leave(object me, string dir)
{
        int total_steps;

        if (! userp(me)
            || query("family/family_name", me) == "雪山寺"
            || query("family/family_name", me) == "血刀门" )
        {
                total_steps = 1; 
        } else
        {
                me->receive_damage("jing", 5);
                me->receive_wound("jing", 5);
                message_vision(HIM "\n突然间$N" HIM "发现眼前的景象有"
                               "些迷乱，不禁觉得头晕目眩。\n\n" NOR, me);

                total_steps = 5;
        }

        if (dir == "east")
                addn_temp("taohua/steps", 1, me);

        if (dir == "south")
                addn_temp("taohua/steps", -1, me);

        if( query_temp("taohua/steps", me) == total_steps )
        {
                me->move(__DIR__"hubian1");
                delete_temp("taohua/steps", me);
                write(HIC "\n雾气散开，你的眼前豁然开朗起来。\n\n" NOR);
                return -1;
        }  

        if( query_temp("taohua/steps", me) == -total_steps )
        {  
                me->move(__DIR__"tulu3");
                delete_temp("taohua/steps", me);
                write(HIC "\n雾气散开，你的眼前豁然开朗起来。\n\n" NOR);
                return -1;
        }

        return ::valid_leave(me, dir);
}