inherit DEMONROOM;

#include <ansi.h>

void create()
{
        set("short", "������");
        set("long",@LONG
�����ɵ��ĳ�����������������ߴ��������ÿ��������ӿ��
�����ʱ�������ϵ������᲻ͣ��ҡ����֦����ҶҲ�׷����£�
�������̵������ġ�
LONG);

        set("exits", ([
                "north"     : __DIR__"xiuzhenchi",
        ]));
        set("no_rideto", 1);         // ���ò��������������ط�
        set("no_flyto", 1);          // ���ò��ܴ������ط�����������
        set("no_die", 1);            // �������ƶ�����ȵ��
        set("penglai", 1);           // ��ʾ��������

        set("n_time", 20); 
        set("n_npc", 2); 
        set("n_max_npc", 10);
        set("s_npc", __DIR__"npc/songshu");
                
        if (random(3) == 1)
        {
                set("objects", ([
                        __DIR__"obj/songye" : 1 + random(10),
                ]));
        }
                        
        setup();
}

void init ()
{
        if (! playerp(this_player()))return;
        
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