inherit ROOM;

void create()
{
        set("short", "ɽ·");
        set("long", @LONG
�����ں�ɽɽ·�ϣ�����ɽ����䣬�������ϪȪ���٣�����
���ƣ��������ش�졣ɽ·����ͨ��ĥ��̨�������塣
LONG );
        set("exits", ([
               "westup"    : __DIR__"tianzhu",
               "eastdown"  : __DIR__"mojingtai",
        ]));

        set("outdoors", "henshan");

        set("no_clean_up", 0);

	set("coor/x", -6960);
	set("coor/y", -5550);
	set("coor/z", 20);
	setup();
        replace_program(ROOM);
}