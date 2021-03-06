// fangyi.c 方怡
// Last Modified by winder on Jul. 12 2002

#include <ansi.h>;
inherit NPC;

int do_work();
string *fjob = ({"毒蛇", "蟒蛇", "腹蛇", "金环蛇", "水蛇", "银环蛇", "竹叶青"});

void create()
{
        set_name("方怡", ({ "fang yi","fang" }));
        set("long", "一张瓜子脸，容貌甚美。\n");
        set("gender", "女性");
        set("age", 18);
        set("attitude", "peaceful");
        set("shen_type", 0);
        set("str", 23);
        set("int", 25);
        set("con", 25);
        set("dex", 26);
        set("max_qi", 600);
        set("max_jing", 600);
        set("neili", 600);
        set("max_neili", 600);
        set("jiali", 30);
        set("combat_exp", 50000);

        set_skill("force", 30);
        set_skill("dulong-dafa", 20);
        set_skill("dodge", 40);
        set_skill("yixingbu", 60);
        set_skill("strike", 30);
        set_skill("leg", 30);
        set_skill("parry", 40);
        set_skill("sword", 50);
        set_skill("jueming-leg", 40);
        set_skill("meiren-sanzhao", 40);
        set_skill("literate", 40);

        map_skill("force", "dulong-dafa");
        map_skill("dodge", "yixingbu");
        map_skill("leg", "jueming-leg");
        map_skill("sword", "meiren-sanzhao");
        map_skill("parry", "jueming-leg");
        prepare_skill("leg", "jueming-leg");

        set("party/party_name", HIY"神龙教"NOR);
        set("party/rank", HIR"赤龙门"NOR"教众");
        set("party/level", 1);
        create_family("神龙教", 3, "弟子");

        set("inquiry", ([
                "教主" : (: do_work :),
                "神龙教" : (: do_work :),
                "jiao" : (: do_work :),
        ]));
        setup();
        carry_object(VEGETABLE_DIR"xionghuang");
        carry_object(WEAPON_DIR"changjian")->wield();
        carry_object(CLOTH_DIR"female-cloth")->wear();

}
void init()
{
        add_action("do_comfort", "comfort");
}

int do_work()
{
        object me = this_player();
        string fword, fwant;

        if( !query("sg/spy", me) && !wizardp(me) )
        {
                say("方怡哼了一声说：假仁假义的东西，还不给我滚！\n");
                return 1;
        }
        if( query_temp("marks/方a", me) )
        {
                say("方怡不耐烦地说道：有完没完了？\n");
                return 1;
        }
        if( time()<query("marks/方c", me)+180 )
        {
                command("slap"+query("id", me));
                say("方怡大怒道：蠢猪，这么快就忘了！\n");
                return 1;
        }

        set_temp("marks/方a", 1, me);
        set("marks/方c", time(), me);

        fwant = fjob[random(sizeof(fjob))];
        fword = sprintf("方怡叹了口气，说道：奉教主之命，捉%s来配药。\n",fwant);
        say(fword);
        set_temp("marks/毒蛇", fwant, me);
        return 1;
}

int do_comfort(string arg)
{
        object me = this_player();

        if(!arg || !(arg == "fang yi" || arg == "fang")) return notify_fail("");
        message_vision("$N笑嘻嘻地安慰着方怡。\n", me);
        if( !query_temp("marks/方a", me) )
        {
                say("方怡冷笑道：别假惺惺了。\n");
                return 1;
        }
        delete_temp("marks/方a", me);
        message_vision("$N道：方姑娘，请放心！我这就给你去抓。\n", me);
        command("thank"+query("id", me));
        set_temp("marks/方b", 1, me);

        return 1;
}

int accept_object(object who, object ob)
{
        int expgain, faccept;

        if( !query_temp("marks/方b", who))return 0;
        if( query("name", ob) != query_temp("marks/毒蛇", who) )
        {
                command("angry"+query("id", who));
                command("disapp"+query("id", who));
                return 0;
        }

        delete_temp("marks/方b", who);
        delete_temp("marks/毒蛇", who);

        call_out("destroy_it", 1, ob);

        command("jump"+query("id", who));
        command("secret"+query("id", who));

        message_vision("方怡在$N耳边悄悄的说了几句话。\n", who);
        if( who->query_skill("medicine", 1) < 30 )
                who->improve_skill("medicine",random(2*query("int", who)));

        if( (expgain=query("combat_exp", who))>50000)return 1;

        addn("sg/exp", 1, who);
        expgain = (50000 - expgain) / 200;
        addn("combat_exp", expgain+random(expgain), who);

        return 1;
}

void destroy_it(object ob)
{
        if(ob) destruct (ob);
}
