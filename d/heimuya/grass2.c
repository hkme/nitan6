inherit ROOM;
 
void create()
{
        set("short", "�ݵ�");
        set("long", @LONG
������һƬ�ݵء��ݵ������ƺ��������磬���ߺ�����һ��С��
԰��
LONG );
        set("exits", ([
            "west"   : __DIR__"chufang1",
            "east"   : __DIR__"hua1",
             "north" :__DIR__"dating1",
             "south" :__DIR__"grass1",
        ]));
        set("objects", ([
                __DIR__"npc/shinu" : 1,
        ]));
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}