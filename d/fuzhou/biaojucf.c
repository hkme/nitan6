// Room: /city/biaojucf.c

inherit ROOM;

void create()
{
        set("short", "�ھֳ���");
        set("long", @LONG
�����ھֶԻ����ʮ�ֿ��������ÿ����н�𣬶��һ�����ס����
����ǻ����һ�����͵ĵط���������ǽ�Ϲ����˹���ư�裬����һЩ
��ɵ�±ζ�������з���һ�ų�̨����̨����Χ���ż������ʡ�
LONG
        );
        set("exits", ([
                "south" : __DIR__"biaojuhy",
        ]));
        set("objects", ([
                __DIR__"npc/hua" : 1,
        ]));
	set("coor/x", -101);
	set("coor/y", -6141);
	set("coor/z", -1);
	setup();
        replace_program(ROOM);
}