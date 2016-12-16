// Room: eastroad3.c
// Date: Feb.14 1998 by Java

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "�����");
        set("long", @LONG
�����ڶ�����ϣ����ż�ʵ����ʯ����档����һ����ֱ�Ĵ�
�������߿���ͨ�������ţ����ϱ�ͨ���ϴ�֣����Ϸ�������������
��¥�ˡ��������Ǵ��������������˳ɶ��ֲ�����Զ����һ�����õ�
���ݣ��������ӵ����� (liang)�Ѿ����������ŷ��˸���ʵ��
LONG );
        set("outdoors", "chengdu");
        set("exits", ([
            "southeast"  : __DIR__"wangjianglou",
            "southwest"  : __DIR__"southroad1",
            "north"      : __DIR__"eastroad2",
            "east"      : __DIR__"xym_cdfb",
        ]));

        set("item_desc", ([
                    "liang" : "���ӵ��������������ŷ��˸���ʵ��\n",
        ]) );

//        set("no_clean_up", 0);
        set("coor/x", -15200);
	set("coor/y", -1840);
	set("coor/z", 0);
	setup();
}

void init()
{
        add_action("do_break", "break");
}

int do_break(string arg)
{
        int n;
        object weapon, me = this_player();

        if (arg != "liang")
                return 0;

        if (query("break"))
                return notify_fail("�����Ѿ�����ɣ��ò����ٷ����ˡ�\n");

        message_vision(HIY "$N" HIY "�ߵ���ǰ������˫�ƣ��͵ػ�"
                       "������ķ�����\n" NOR, me);

        if( query("max_neili", me)<1000 )
        {
               message_vision(HIR "���ֻ��һ���ƺߣ�$N" HIR "��"
                              "�����ã�������������ǰ��ʱһ�ڡ�"
                              "��\n" NOR, me);
               set("neili", 0, me);
               me->unconcious();
               return 1;
        }

        message_vision(HIY "ֻ��һ�����죬������ʱ��$N" HIY "��"
                       "�÷���¶���˴��ţ�\n" NOR, me);

        set("exits/enter", __DIR__"minju");
        set("neili", 0, me);
        set("break", 1);
        return 1;
}