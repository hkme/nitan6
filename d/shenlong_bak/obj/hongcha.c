// hongcha.c 红茶
// Last Modified by winder on Jul. 12 2002

inherit ITEM;

void create()
{
        set_name("红茶",({"tea", "cha", "hong cha"}));
        set_weight(50);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "一杯红茶，悠悠地冒着香气～～～\n");
                set("unit", "杯");
                set("value", 300);
                set("remaining", 2);
                set("drink_supply", 25);
        }
        setup();
}

void init()
{
        add_action("do_drink", "drink");
}

int do_drink(string arg)
{

        int heal, recover, jing, e_jing, m_jing;

        recover = 5;
        if( !this_object()->id(arg) ) return 0;
        if( this_player()->is_busy() )
                return notify_fail("你上一个动作还没有完成。\n");
        if( query("water", this_player()) >= 
                (int)this_player()->max_water_capacity() )
                return notify_fail("你已经喝太多了，再也灌不下一滴水了。\n");

        set("value", 0);
        addn("water", query("drink_supply"), this_player());
        jing=query("jing", this_player());
        e_jing=query("eff_jing", this_player());
        m_jing=query("max_jing", this_player());

        if (jing < e_jing )
        {
                if ( (jing + recover) >= e_jing )
                {
                        set("jing", e_jing, this_player());
                }
                else
                {
                        set("jing", jing+recover, this_player());
                }
        }
        if( this_player()->is_fighting() ) this_player()->start_busy(2);
        addn("remaining", -1);
        if ( query("remaining") )
        {
                 message_vision( "$N端起杯红茶，有滋有味地品了几口。\n一股香气直入心脾，$N觉得精神好多了。\n", this_player());
        }
        else
        {
                 message_vision( "$N端起雕花小杯，把剩下的红茶一饮而尽。\n一股香气直入心脾，$N觉得精神好多了。\n", this_player());
                destruct(this_object());
        }
        return 1;
}