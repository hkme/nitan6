inherit ROOM;

#include <ansi.h>

void create()
{
        set("short", "������");
        set("long",@LONG
������λ�ڵ��ɹ��������֮�䣬������������Ҫ������˵��
�����ɳ������ػ�����ĵ��ڣ�Ϊ���Ƿ�ֹ���з������ȣ�Σ����
�䡣�ɲ����Ǿ���͵��˯�죬���Ƿ����ӳ�ɽ�ȡ����������õ���
��ͷ���������ʯ�����ڴ�·֮�ϣ������ػ������ȣ��˵�������
����������
LONG);

        set("exits", ([
                "west"     : __DIR__"diexiangu2",
                "east"     : __DIR__"jinxianqiao",        
                "south"    : __DIR__"changshengdao2",                
        ]));
        set("no_rideto", 1);         // ���ò��������������ط�
        set("no_flyto", 1);          // ���ò��ܴ������ط�����������
        set("no_die", 1);            // �������ƶ�����ȵ��
        set("penglai", 1);           // ��ʾ��������
        set("objects", ([ 
                "/clone/npc/walker": 2,
        ]));
        setup();
}

void init ()
{
        if (! this_player()->query_temp("apply/xianshu-lingwei"))
        {
                if (! this_player()->query("penglai/go_quest/ok"))
                {
                        this_player()->start_busy(3);
                        tell_object(this_player(), NOR + WHT "�㵽�˴˴�������������������һʱ�����Ա����\n" NOR);
                }
                else
                {
                        if (random(2))
                        {
                                this_player()->start_busy(1);
                                tell_object(this_player(), NOR + WHT "�㵽�˴˴�������������������һʱ�����Ա����\n" NOR);                
                        }
                }
        }
}