#include <ansi.h>
inherit __DIR__"pill";

void create()
{
        set_name(HIW "帝者舍利" NOR, ({ "dizhe sheli", "sheli", "dizhe" }));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIW "佛门高僧在圆寂前通常都将毕生功力注入舍利子中。\n" NOR);
                set("base_unit", "颗");
                set("base_value", 10000);
                set("base_weight", 50);
                set("only_do_effect", 1);
                set("pill_skill", "etc");
                set("force_point", 20);
                set("pill_msg3", HIM "你感到精力又增加了一些。\n" NOR);
                set("pill_msg4", HIR "你感到内息澎湃，难以吸收药力。\n" NOR);
        }
        setup();
}