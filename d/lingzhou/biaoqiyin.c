// Room: /lingzhou/biaoqiyin.c
// Java. Sep 21 1998

#include <room.h>

inherit ROOM;
void create()
{
        set("short", "����Ӫ");
        set("long", @LONG
�����ǻ��ҽ�������פ�أ������ݳ�����������Ӫռ������Ӵ�
������Ӫ����������СУ������������ų�ǽ��һ�����ǣ��ж�Ӫ��
����Ӫ��ͨ��ʹ��ʹ�֡�����Ӫ�ľ�ʿ�������ľ���ǧ����ѡ����
ʿ��ƽʱ����ʳ���Χ�ľ��䣬���ϳ�Ѳʱ�����𿪵�����������
սʱ����һ֧��������
LONG );
        set("exits", ([
                "south" : __DIR__"xiaoxiaochang",
                "north" : __DIR__"yinfang",
                "east"  : __DIR__"xidajie",
                "west"  : __DIR__"malan",
        ]));
        set("objects", ([
                __DIR__"npc/xixiabing" :2,
        ]));
        set("outdoors", "lingzhou");
        set("coor/x", -6285);
        set("coor/y", 2935);
        set("coor/z", 0);
        setup();
        replace_program(ROOM);
}