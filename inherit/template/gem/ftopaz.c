// $file_name.c                                                 /* COMMENT_ENTRY */
// Create by TEMPLATE_D. $time.                                 /* COMMENT_ENTRY */
// skull.c
// Written by Lonely                                            /* COMMENT_EXIT  */

#include <ansi.h>

inherit TESSERA;

void create()
{
//**    set_name("$name", ({ "$id", }));                        /* NAME_ENTRY */
        set_name(HIY "ϡ���Ʊ�ʯ" NOR, ({ "flawless topaz" }) );   /* NAME_EXIT  */
        set_weight(20);
        if( clonep() )
                set_default_object(__FILE__);
        else {
//**            set("$dbase_key", "$dbase_value\n");            /* DBASE_ENTRY */
                set("long", HIY "һ�����������ױȵĴ�Ʊ�ʯ��\n" NOR);  /* DBASE_EXIT  */
                set("value", 100000);
                set("unit", "��");
                set("level", 3);
                set("material", "ftopaz");
                set("can_be_enchased", 1);
                set("magic/type", "earth");
                set("magic/power", 200);
                set("auto_load", 1);
        }
        set("enchase/weapon_prop/add_earth", 3);
        set("enchase/rings_prop/add_earth", 3);
        set("enchase/armor_prop/reduce_water", 1);
        setup();
}
