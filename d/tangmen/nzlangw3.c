//nzlangw3.c                �Ĵ����š���ʯС·

#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
        set("short", "��ʯС·");
        set("long",
"����һ����ʯ�̾͵�С·��������������������̫�࣬�ֲڵ���ʯ��\n"
"����ĥ�Ĺ⻬�羵��һЩ���ŵĵ��������������ﻹ���Ǻ����֡�������\n"
);
        set("exits", ([
                        "east" : __DIR__"nzlangw2",
                        "west" : __DIR__"shufang",
                        "north" : __DIR__"brestroom",
                        "south" : __DIR__"grestroom",
        ]));
        set("area", "tangmen");
        set("outdoors", "tangmen");
        setup();
//        replace_program(ROOM);
}

int valid_leave(object me, string dir)
{
        if( (dir == "north") && (query("gender", me) != "����") )
                return notify_fail("ι�����ﶼ������ѽ��\n");

        if( (dir == "south") && (query("gender", me) != "Ů��") )
                return notify_fail("ι���ô�Ů������Ϣ�ң����Ź�ģ�\n");

        return ::valid_leave(me, dir);
}