inherit DEMONROOM;

void create()
{
        set("short", "����");
        set("long",@LONG
���а�����9��18�±�����������ʿ��Ϊ���ǵ�Ӣ�۱���ɣ�
LONG);

        set("exits", ([
                "west"   : __DIR__"japan4",
                "east"   : __DIR__"changqi2",

        ]));

                set("n_time", 30); 
                set("n_npc", 5); 
                set("n_max_npc", 6);
                set("s_npc", "/clone/dream_npc/japangame/japannpc01");

        setup();
}