//chahua7.c
#include <ansi.h>
#include <armor.h>;
inherit HEAD;

void create()
{
        set_name(HIR"��"WHT"��"NOR, ({"cha hua", "hua"}));
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", 
"һ��軨��ͬ�꿪�����仨��һ�仨ɫ"HIR"����"NOR"��һ��"WHT"����"NOR"���������\n");
                set("value", 75);
                set("material", "plant");
                set_weight(10);
                set("armor_prop/armor", 0);
                set("armor_prop/per", 3);
                set("wear_msg", "$N����ذ�һ��$n����ͷ�ϡ�\n");
                set("unequip_msg", "$N����ذ�$n��ͷ��ժ��������\n");
        }
        setup();
}
