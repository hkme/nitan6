// Room: wuhouci.c
// Date: Feb.14 1998 by Java

inherit ROOM;
string look_gaoshi();
void create()
{
        set("short", "�������");
        set("long", @LONG
�������������������������������ڽ�һ����ǰ��������
��հ������и�������Ҳ�������������ڵ������ڵģ���ʫʥ�Ÿ���
����¶��ǧ��������������Ƶ�������£����������ϳ��ġ���������
���ǧ�ŷ緶��һ�����¡�
LONG );
        set("exits", ([
            "southdown" : __DIR__"liubeidian",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -15240);
	set("coor/y", -1840);
	set("coor/z", 10);
	setup();
        replace_program(ROOM);
}