// tduanqun.c

#include <armor.h>
inherit CLOTH;

void create()
{
        set_name("̨�Ķ�ȹ", ({ "duan qun" }));
        set("long", "����һ��̨�Ķ�ȹ��\n");
        set_weight(2000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("material", "cloth");
                set("unit", "��");
                set("value", 200);
                set("armor_prop/armor", 1);
        }
        setup();
}