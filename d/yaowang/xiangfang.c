inherit ROOM;

void create()
{
        set("short", "�᷿");
        set("long", @LONG
����һ�䲼�þ����ķ��䣬������һ����ľ������������һ
������ܼ���ռ��һ��ǽ����ϸ������ȫ��ҽѧ������ĵ伮��
��Ȼû��һ���书�鼮��
LONG
        );
        set("exits", ([
  "south" : __DIR__"yitong",
]));

        setup();
        replace_program(ROOM);
}