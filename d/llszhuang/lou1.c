#include <ansi.h>
inherit ROOM;
void create()
{
        set("short",CYN"�� �� ¥"NOR);
        set("long",
"���ǽ���¥�ĵ�һ�㣬�������ֱ��գ�ʹ�����￴����ʮ�ֵĻ谵��\n"
);

        set("exits", ([ /* sizeof() == 2 */
          "up" : __DIR__"lou2",
          "out": __DIR__"lou0",
        ]));
        set("no_magic", "1");

        set("objects", ([
                        __DIR__"npc/qianerbai" : 1,
                ])
        );

        setup();
}