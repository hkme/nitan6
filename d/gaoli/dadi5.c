// /d/gaoli/xuanwumen
// Room in ����
// rich 99/03/28
inherit ROOM;
void create()        
{
        set("short", "���");
        set("long", @LONG
������һ����̡��Ա߾�����ӿ�Ĵ󺣣������紵���������Ĵ�
�ŵ̰������������϶��Ǻ�ˮ����������˱Ƚ϶࣬���������������
���ġ�
LONG
        );
set("outdoors", "gaoli");
set("exits", ([
                "northwest" : __DIR__"dadi4",
                "southeast":__DIR__"dadi6",
        ]));
       setup();
        
}