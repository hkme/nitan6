inherit ROOM;

void create()
{
        set("short", "�˵�");
        set("long", @LONG
��Ƭ��԰�Ӽ����������һЩʱ���߲ˡ������м���ľ׮�����
�˼��ӣ���һЩ���ٵ��߲��������������Ŀ��ȥ���ϴ��һƬ��԰
�ӣ���Ȼ���������û�м��òˡ�
LONG );
        set("no_clean_up", 0);
        set("exits", ([
                "south" : __DIR__"caidi",
                  "west" : __DIR__"caifang",
        ]));
        set("outdoors", "kaifeng");

	set("coor/x", -5000);
	set("coor/y", 2220);
	set("coor/z", 10);
	setup();
        replace_program(ROOM);
}