inherit ROOM;

void create()
{
        set("short", "ʯ�¿�ջ");
        set("long", @LONG
������ѩɽɽ·�ϵ�һ���ջ����Ȼ�������ʵ��ƽ��Ҳ
��������ʲô�ο�����ѩ����ɽ��ˮ���������ѩɽ�ϣ�ȴҲ��
��һ�޶��Ŀ�ջ��������ط���Ҫ�ǲ��뱻�������������
���ϣ�ֻ�»��ǵ��չ�һ����������⡣������ҿ�ջ��Ҳ����
����¡��
LONG );
        set("outdoors", "lingxiao");
        set("exits", ([
                "west" : __DIR__"shiya",
        ]));
        set("objects", ([
                __DIR__"npc/xiaoer" : 1,
                "/clone/npc/walker" : 1,
                __DIR__"npc/dizi" : 2,
        ]));
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}