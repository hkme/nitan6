#include <ansi.h>

inherit ROOM;

void create()
{
        set("short", "ɽ��");
        set("long", @LONG
������ǽ������ŷ�ɥ���ġ����ư���Դӡ����Ƽߺ�ɽ��һ�۽���
ɽ�ɴ��һ�겻��֮�󣬰�������ˮ��Ư����ͷ���𽭺��������ǽ�ɽ�ı�
��֮·������С·ͨ��ɽ�·��һ������һ������(paizi) �����Ե�����
�ڲ�ʱ����һЩ�������ƺ������ڰ��м��ӡ�
LONG        );
    set("outdoors", "tiezhang");
        set("exits", ([ /* sizeof() == 2 */
  "northup" : __DIR__"shanlu-1",
  "southdown" : __DIR__"shanjiao",
]));
    set("objects", ([ /* sizeof() == 2 */
    __DIR__"npc/heiyi" : 2,
                CLASS_D("tiezhang") + "/first" : 1,
]));
        set("item_desc", ([ /* sizeof() == 1 */
  "paizi" : "[31m��������--���� 
[0;37;0m",
]));
        set("no_clean_up", 0);

        setup();
}

int valid_leave(object me, string dir)
{
        mapping myfam;
        myfam=query("family", me);

        if( (!myfam || myfam["family_name"] != "���ư�") && dir == "northup" && query("combat_exp", me) >= 10000 && 
                objectp(present("heiyi bangzhong", environment(me))))
        return notify_fail(CYN "���°��ڳ鵶��ס���ȥ·��˵�������㲻�����ư���ӣ�������ɽ��\n" NOR);

        return ::valid_leave(me, dir);
}