// Create by BUNCH_D written by Lonely

#include <ansi.h>
inherit ROOM;

int is_bunch_room() { return 1; } 

void create()
{
        set("short", "�շ��� - ��ʹ�� cshort ���༭����");
        set("long", "����һ��ʲôҲû�еĿշ��䣬������ clong ���༭����������\n");
        
        set("exits", ([
                "north" : __DIR__"room5",
                "south" : __DIR__"room3",
        ]));

        setup();
}