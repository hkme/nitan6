// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// pi.c �Խ� ��ȸ����

#include <ansi.h>
inherit NPC;

void create()
{
        set_name("�Խ�", ({ "pi jiang", "pi", "jiang" }));
        set("gender", "����");
        set("age", random(10) + 30);
        set("str", 25);
        set("dex", 16);
        set("long", "����һ���������Ὣ�죬����������\n");
        set("combat_exp", 45000);
        set("shen_type", 1);
        set("attitude", "peaceful");

        set_skill("unarmed", 60);
        set_skill("force", 60);
        set_skill("sword", 60);
        set_skill("dodge", 60);
        set_skill("parry", 60);
        set_temp("apply/attack", 50);
        set_temp("apply/defense", 50);
        set_temp("apply/armor", 50);
        set_temp("apply/damage", 30);

        set("neili", 400); 
        set("max_neili", 400);
        set("jiali", 10);

        setup();
        carry_object("/d/city/obj/changjian")->wield();
        carry_object("/d/city/obj/tiejia")->wear();
}

void init()
{       
        object ob;

        ::init();

        if (objectp(ob = this_player()) && ! is_fighting()) 
        {
                // remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}

void greeting(object ob)
{
        if (! ob || environment(ob) != environment()) 
                return;

        if( query_temp("guo_shoucheng/start_job", ob) == 1 )
                say("�Խ�������ü����λ" + RANK_D->query_respect(ob) + "���㻹�ǰ����سǰɡ�\n");
        else 
        if( query_temp("guo_shoucheng/mark_shadi", ob) == 3
         && !query_temp("guo_shoucheng/job_over", ob) )
        {
                say("�Խ�����˵������λ" + RANK_D->query_respect(ob) + "������������ʱ���ɹ������Ѿ��칥�ϳ��ˡ�\n");
                
                set("env/combatd", 4, ob);
                ob->apply_condition("jobshadi_limit", 10 + random(5));
                set_temp("guo_shoucheng/start_job", 1, ob);
                set_temp("guo_shoucheng/where", environment(ob), ob);
                // remove_call_out("to_rob");
                call_out("to_rob", 10, ob);
        } else
        if( query_temp("guo_shoucheng/job_over", ob) )
        {
                say("�Խ�����˵������λ" + RANK_D->query_respect(ob) + "�����Ѿ����Ի�ȥ�����ˡ�\n");
                return;
        } 

}

void to_rob(object ob)
{
        object *target, robber, room;              
        int i;
        
        if (! ob || environment(ob) != environment()) 
                return;

        room = environment(ob);
        
        if( query("short", environment(ob)) == "��ȸ����" )
        {
                robber = new(__DIR__"robber1");
                robber->do_change(ob);
                set("want_kill", ob, robber);
                robber->move(room);
                message_vision(HIR "ͻȻ����������һ���ɹű�ʿ��\n" NOR, this_player()); 
                robber->kill_ob(ob);
                ob->kill_ob(robber);
        }
}

int accept_kill(object me) 
{
        message_vision(CYN "$N" CYN "��ŭ���������ҹ�����˵��һ�ư�$n" CYN "�����ڵء�\n" NOR, this_object(), me); 
        me->unconcious(); 
        return -1; 
}

int accept_ansuan(object who) 
{
        return notify_fail("����밵�㣬������ǰһ��������̫�����\n"); 
}

int accept_hit(object me) 
{

        message_vision(CYN "$N" CYN "��ŭ���������ҹ�����˵��һ�ư�$n" CYN "�����ڵء�\n" NOR, this_object(), me); 
        me->unconcious(); 
        return -1; 
}

int accept_fight(object who) 
{
        command("say û��Ȥ��"); 
        return 0; 
}

void fight_ob(object ob) 
{
        message_vision(CYN "$N" CYN "��ŭ���������ҹ�����˵��һ�ư�$n" CYN "�����ڵء�\n" NOR, this_object(), ob); 
        ob->unconcious(); 
        return; 
}

void unconcious()
{
        return;
}

void die()
{
        return;
}