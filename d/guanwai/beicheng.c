inherit ROOM;

void create()
{
        set("short", "����");
        set("long", @LONG
�������û����̾͵Ľֵ������ϵ����˲��࣬�����������塣ż���м�
������������ε������߹��������������壬����ľ���������һ������
��С���֪ͨ��δ���
LONG );
        set("exits", ([
                "south" : __DIR__"jishi",
                "west"  : __DIR__"xiaoxiang",
        ]));
        set("objects", ([
                __DIR__"npc/xiucai" : 1,
                "/clone/npc/walker" : 1,
        ]));
        set("outdoors", "guanwai");
        set("coor/x", 3990);
	set("coor/y", 9250);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}