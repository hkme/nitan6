#include <ansi.h>
#include <armor.h>

inherit BOOTS;

void create()
{
        set_name( HIM "ս��" NOR, ({ "zhan hun", "zhanhun" }) );
        set_weight(4000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "˫");
                set("long", HIM "��㴫˵������콫����֮��ѥ��ѥ������ɢ���ź��⡣\n" NOR);
                set("value", 2000000);
                set("armor_prop/armor", 100);
                                set("armor_prop/dex", 4);
                                set("armor_prop/str", 4);
                                set("armor_prop/damage", 100);
                                set("limit", ([ 
                                        "exp" : 6000000,
                                ]));
                set("material", "steal");
        }
        setup();
}

int query_autoload()
{
        return 1;
}