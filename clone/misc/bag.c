#include <ansi.h>
inherit ITEM;

#define GIFT_DIR        "/clone/gift/"
#define NORM_DIR        "/d/item/obj/"

// ������Ʒ�б�
string *VA_LIST = ({ "jiuzhuan", "puti-zi", "tianxiang", });

// ��ͨ��Ʒ�б�  
string *NORMAL_LIST = ({ "xuantie", "wujins", "butian",
                         "tiancs", });

// ������Ʒ�б�
string *SM_LIST = ({ "unknowdan", "xiandan", "xisuidan", "shenliwan", });

int is_bag(){ return 1; }

void create()
{
        set_name(WHT "����" NOR, ({ "bag", "baoguo", "bao", "guo" }));
        set_weight(200);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", WHT "����һ���Ҳ������������͹Ĺĵģ���֪װ"
                            "��Щʲô��\n" NOR);
                set("no_sell", 1);
                set("value", 500);
                set("material", "cloth");
        }
        set("gift_count", 1);
}

void init()
{
        if (this_player() == environment())
        {
                add_action("do_open", "open");
                add_action("do_open", "unpack");
                add_action("do_open", "dakai");
        }
}

int do_open(string arg)
{
        object me, gift;
        string un;

        if (! arg || ! id(arg))
                return 0;

        if (query("gift_count") < 1)
        {
                write("��������ʲôҲû���ˡ�\n");
                return 1;
        }

        me = this_player();
        message_vision(WHT "\n$N" WHT "�𿪰�������������������"
                       "������д�š�" HIR "ʦ�ż��£�����ٸϻ�"
                       NOR + WHT "����\n����֮�������º���ѹ"
                       "��ʲô�����������ú����ܣ�$N" WHT "��״"
                       "��æȡ����\n" NOR, me);

        if (random(5) <= 3)
                gift = new(GIFT_DIR + VA_LIST[random(sizeof(VA_LIST))]);
        else
        if (random(50) == 1)
                gift = new(GIFT_DIR + SM_LIST[random(sizeof(SM_LIST))]);
        else
                gift = new(NORM_DIR + NORMAL_LIST[random(sizeof(NORMAL_LIST))]);

        if( query("base_unit", gift) )
                un=query("base_unit", gift);
        else
                un=query("unit", gift);

        tell_object(me, HIC "������һ" + un + HIC "��" + gift->name() +
                        HIC "����\n" NOR);

        gift->move(me, 1);
        addn("gift_count", -1);
        set("long", WHT "����һ���Ҳ�����������Ķ����Ѿ���ȡ�����ˡ�\n" NOR);
        set("value", 0);
        return 1;
}
