// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

inherit ROOM;

#include "maze.h"

void create()
{
        set("short", "ʯ��");
        set("long", @LONG
����һ����ԼĪ���˶�ߵĴ�ʯ�鹹�ɵ���ʯ�󣬹�ģ�ƺ����󣬵�
̧����ȥ�����ܾ������ص�������Ᾱ�ʯ����ȫ��������·ͨ��ʯ��֮�⣬
��֮���г�������������������������ͷ����ʯ������һ��б����ʯ��(s
hibei)�������ڸ����С�
LONG );

        set("exits", ([
                "east"  : __DIR__"maze6",
                "south" : __DIR__"maze28",
                "west"  : __DIR__"maze25",
        ]));

        set("item_desc", ([
                "shibei" : "
�������٣�Ů׳������ȡŮ��
����������ϵ�ڽ���ڣ��꼪�������������ף��������
�����Ŷ��������㣬�޾̣���������
�������������޷������д��ң������޴�̡�
�������ģ������㣬���ס�
�������壺��轰��ϣ����£��������졣
�����Ͼţ�������ǣ��ߣ��޾̡�
���� �� Ů �� ��
���� �� ľ �� ��
\n",
        ]));

        setup();
}

void init()
{
        object me;
        me = this_player();
        
        addn_temp("taohua/count", 44, me);
        check_count(me, 44);
        return;
}