//ROOM : gongbu.c

inherit ROOM;

void create()
{
        set("short", "��������");
        set("long", @LONG
����������֮��--������Ա�򳯵ĳ���.
LONG
        );
        set("no_clean_up", 0);
        set("exits", ([
                "north" : __DIR__"zuomen",
        ]));
	set("coor/x", -2810);
	set("coor/y", 7681);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}