#include <ansi.h>

inherit EQUIP_SOUL;

void create()
{
        set_name(HIM "սѥ֮��" NOR, ({"boots soul1", "boots", "soul1"}));
        set_weight(100);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIM "����һ��սѥ֮�ꡣ\n" NOR);
                set("unit", "��");
                set("value", 200000);
                set("no_sell", 1);
        }
        set("type", "boots");
        set("status", 1);
        set("auto_load", 1);
        
        setup();
}
