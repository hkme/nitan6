//brestroom.c  by winder

#include <room.h>

inherit ROOM;

void create()
{        
        set("short", "Ů������Ϣ��");
        set("long",
"����һ���谵�ķ��䣬���ڱ����ĺڲ���ס��ֻ��һ˿���ߴ��ŷ�\n"
"��͸����ֻ����Լ�Ŀ���������Ӱ���ڿ�ǽ�Ĵ�����Ϣ��\n"
);        
        set("valid_startroom",1);
        set("no_steal", "1");
        set("no_fight", "1");
        set("sleep_room",1);
        set("exits", ([
                "north" : __DIR__"nzlangw3",
        ]));
        setup();
        replace_program(ROOM);
}