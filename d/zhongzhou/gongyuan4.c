inherit ROOM;

void create()
{
        set("short", "���ݹ�Ժ");
        set("long", @LONG
���ݹ�Ժ�����������ֵ�һ��������ÿ�³�һ��ȫ������
��ʿ�Ӷ����˴��Ͽ���Ժ��������ʿ�����أ�Ҫ��д��ɲ���
Χǽ�����������ĸ�ʾ��
LONG);
        set("objects", ([
                  "/d/kaifeng/npc/zhukao3" : 1,
        ]));
        set("no_clean_up", 0);

        set("exits", ([
                  "south" : __DIR__"yanling2",
        ]));

        set("outdoors", "zhongzhou");
        set("coor/x", -9070);
	set("coor/y", -990);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}