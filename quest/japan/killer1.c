// /job/japan/killer1.c
// by akuma 8:40 PM 8/3/2003

#include <ansi.h>
inherit NPC;

void create()
{
        int i;
        set_name("�����ʿ", ({ "dongying wushi", "dongyin", "wushi", "shi" }));
        set("gender", "����");
        set("long", "���������ڶ�孵���٣���ɱ�����޶�����\n");
        set("age", random(18) + 10);
        set("str", 20 + random(10));
        set("int", 20 + random(10));
        set("con", 20 + random(10));
        set("dex", 20 + random(10));
        create_family("��ʿ��", 4, "��ʿ");
        //shifu("������");
        if (random(10)>6)i=60+random(30);
        else i=40-random(20);
        set("chat_chance_combat", i);
        set("chat_msg_combat", ({
                (: perform_action, "blade.slash" :),
                (: perform_action, "cuff.dao" :),
        }));
        setup();
        //if ( random(5) > 1 ) carry_object(BINGQI_D("blade"))->wield();
        //carry_object(MISC_D("cloth"))->wear();
        //carry_object(ARMOR_D("armor"))->wear();
        //carry_object(MISC_D("kuzi"))->wear();
        //carry_object(ARMOR_D("udwear/underwear"))->wear();
        add_money("silver", random(20) + 10);
        this_object()->start_call_out( (: call_other, this_object(), "dest", this_object() :), 300);
}

void init()
{
        object me, ob;

        ::init();
        ob = this_object();

        if( interactive(me=this_player()) && query_temp("japan/job", me)){
                message_vision(HIR "$N��$n˵���������������ץ�����\n" NOR, ob, me);
                ob->kill_ob(me);
                me->fight_ob(ob);
                me->start_busy(1);
        }
}

int do_clone()
{
        int number;
        object ob = this_object();

        number=query("max", ob);
        if ( number < 1 ) number = 300;
        /*
        ob->set_skill("cuff", number);
        ob->set_skill("dodge", number);
        ob->set_skill("force", number);
        ob->set_skill("parry", number);
        ob->set_skill("blade", number);
        ob->set_skill("kongshou-dao", number);
        ob->set_skill("renshu", number);
        ob->set_skill("wushi-dao", number);
        ob->map_skill("cuff","kongshou-dao");
        ob->map_skill("dodge","kongshou-dao");
        ob->map_skill("force","renshu");
        ob->map_skill("parry","kongshou-dao");
        ob->map_skill("blade","wushi-dao");
        prepare_skill("cuff", "kongshou-dao");
        */
        return 1;
}

void dest()
{
        object me = this_object();

        if ( !me ) return;

        message_vision("$N����ææ�뿪�ˡ�\n", me);
        destruct(me);
}

void die()
{
        int exp, i, pot;
        mapping reward;
        object corpse, *inv, me, victim = this_object();

        COMBAT_D->announce(this_object(), "dead");
        me=query_temp("last_damage_from", victim);

        if( query_temp("japan/job", me)){
                reward=TASK_D->get_reward(me,100,1,40+random(20),query("job/japan_job", me));
                exp = reward["exp_reward"];
                pot = reward["pot_reward"];
                addn("combat_exp", exp, me);
                addn("potential", pot, me);
                tell_object(me, MAG"ɱ���й���������" + exp + "��ʵս���飬" + pot + "��Ǳ�ܡ�\n"NOR);
                set_temp("japan/kill", 1, me);
        }
        corpse = new(CORPSE_OB);
        corpse->set_name( victim->name(1) + "��ʬ��", ({ "corpse" }) );
        set("age",query("age",  victim), corpse);
        set("gender",query("gender",  victim), corpse);
        set("victim_name", victim->name(1), corpse);
        set("victim_id",query("id",  victim), corpse);
        set("victim_user", userp(victim), corpse);
        set("victim_exp",query("combat_exp",  victim), corpse);
        set("victim_shen",query("shen",  victim), corpse);
        set("kill_by",query_temp("last_damage_from",  victim), corpse);
        corpse->set_weight( victim->query_weight() );
        corpse->set_max_encumbrance( victim->query_max_encumbrance() );
        set_temp("die_by_from",query("kill_by",  corpse), victim);

        if ( query_temp("faint_by", victim) ) set("kill_by", query_temp("faint_by", victim), corpse);
        else set("kill_by", me, corpse);

        inv = all_inventory(victim);
        inv->owner_is_killed(me);
        inv -= ({ 0 });
        i = sizeof(inv);
        while( i-- ) {
                if( query("equipped", inv[i]) == "worn"){
                        inv[i]->move(corpse);
                        if ( !inv[i]->wear() ) inv[i]->move(environment(victim));
                }
                if( query("embedded", inv[i])){
                        delete("embedded", inv[i]);
                        inv[i]->move(corpse);
                }
                else inv[i]->move(corpse);
        }
        set("long", victim->long(1) + "Ȼ����" + gender_pronoun(query("gender", victim)) + "�Ѿ����ˣ�ֻʣ��һ��ʬ�徲�����������\n", corpse);
        corpse->move(environment(victim));
        destruct(victim);
}