// taiyi2.c
// Java Oct.10 1998
#include <ansi.h>;
inherit ROOM;

void create()
{
        set("short", "̫�ҳ�");
        set("long", @LONG
�����ں�ˮ�ֻ���ú�ˮ����͸�ǡ��������ܸ߷廷�У�����
�̲�������ɽ��ˮӰ���羰���ˡ�����ԭ����̫�ҳصĶ�����̫�۵�
�����������(climb) �ϰ���ԶԶ��ȥ�����ߵ�ɽ��֮���ƺ���һ��
�󶴡��벻����(swim)��ȥ����
LONG
        );
        set("outdoors", "zhongnan");
        set("coor/x", -3190);
        set("coor/y", 180);
        set("coor/z", 40);
        setup();
}
void init()
{
        add_action("do_climb", "climb");
        add_action("do_swim", "swim");
}
int do_climb(string arg)
{
        object me = this_player(); 
        int c_exp;
        c_exp=query("combat_exp", me);
        if( !arg || arg != "up")
        {
                write("�Ǹ�����û������\n");
                return 1;
        }

        message_vision("$N��̫�ҳ���������������ȥ��\n", me);
        me->move("/d/quanzhen/taiyi1");
        message_vision("$N��̫�ҳ���������ȥ��\n", me);
        return 1;
}
int do_swim()
{
        int c_exp,c_skill;
        object me = this_player();

        c_exp=query("combat_exp", me);
        c_skill=me->query_skill("jinyan-gong",1);
        if( query("qi", me)<10 )
                me->unconcious();
        else
        {
                me->receive_damage("qi",10);
                if (((c_skill*c_skill*c_skill/10)< c_exp) && (c_skill < 51))
                        me->improve_skill("jinyan-gong", (int)me->query_skill("jinyan-gong", 1));
                message_vision("$N��̫�ҳ��л��˻�ˮ����������������\n", me);
        }
        return 1;
}