inherit ROOM;

void create()
{
        set("short", "��ݺ�Ժ");
        set("long", @LONG
��������ݵĺ�Ժ��������ݵĹ���������������������һЩ������
Ϊ�ĵ��Ӵ����书���õ�����ָ����������ǳ��һ�����������Ĵ�Ժ�ӣ�
�����Ƚ��ľ���������������ô���������֣���һ����������ĺõط���
LONG);
        set("no_clean_up", 0);
        set("exits", ([
                  "north" : __DIR__"wuguan",
        ]));

	set("coor/x", -7010);
	set("coor/y", 2160);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}