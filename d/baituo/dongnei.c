inherit ROOM;

void create()
{
        set("short", "����");
        set("long", @LONG
������ɽ�������ѵ����ڡ��Ա���һ�ප��������������Ů�ӵ�
������һ����׳��ɽ�����������
LONG );
        set("exits", ([
                "west"  : __DIR__"cedong",
                "south" : __DIR__"dongkou",
        ]));

        set("objects", ([
                __DIR__"npc/shanzei3" : 1,
        ]));

        set("coor/x", -49950);
        set("coor/y", 20050);
        set("coor/z", 20);
        setup();
        // replace_program(ROOM);
}
int valid_leave(object me, string dir)
{
        object npc;
        
        if ( (dir == "west")
             && objectp(npc = present("shanzei tou", environment(me)))
             && living(npc) )
                return notify_fail("ɽ��ͷ����ȭͷ������ռ�����ŵı��ˣ������\n");
        return ::valid_leave(me, dir);
}