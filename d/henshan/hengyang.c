inherit ROOM;

void create()
{
        set("short", "������");
        set("long", @LONG
������Ǻ����سǡ������ش�ƫԶ���Լ�Զ������ԭ�����ˡ�
LONG );
        set("outdoors", "henshan");

        set("exits", ([
               "east"   : __DIR__"hengyang2",
               "west"   : __DIR__"hengyang1",
               "south"  : __DIR__"chaguan",
               "north"  : __DIR__"lingxingmen",
        ]));

        set("objects", ([
                "/d/henshan/npc/lu" : 1,
        ]));
	set("coor/x", -6890);
	set("coor/y", -5700);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}