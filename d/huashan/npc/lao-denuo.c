// lao-denuo.c

inherit NPC;
inherit F_GUARDER;

int permit_pass(object me, string dir);

void create()
{
        set_name("�͵�ŵ", ({ "lao denuo","lao","denuo" }) );
        set("nickname", "�Ϻ���");
        set("gender", "����");
        set("age", 61);
        set("long", "�͵�ŵ������Ⱥ�Ķ����ӡ�\n");
        set("attitude", "peaceful");

        set("str", 26);
        set("con", 30);
        set("int", 24);
        set("dex", 30);

        set("max_qi", 3400);
        set("max_jing", 1700);
        set("neili", 2800);
        set("max_neili", 2800);

        set("combat_exp", 150000);
        set("shen_type", -1);
        set("score", 5000);

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
                (: exert_function, "powerup" :),
                (: exert_function, "recover" :)
        }) );

        set_skill("unarmed", 95);
        set_skill("sword", 95);
        set_skill("force", 95);
        set_skill("parry", 95);
        set_skill("dodge", 95);
        set_skill("literate", 40);

        set_skill("huashan-sword", 95);
        set_skill("huashan-neigong", 95);
        set_skill("huashan-quan", 95);
        set_skill("feiyan-huixiang", 95);

        map_skill("sword", "huashan-sword");
        map_skill("parry", "huashan-sword");
        map_skill("force", "huashan-neigong");
        map_skill("unarmed", "huashan-quan");
        map_skill("dodge", "feiyan-huixiang");

        set_temp("apply/damage", 100);
        set_temp("apply/armor", 100);

        set("apply/dodge", 100);

        set("coagents", ({
                ([ "startroom" : "/d/huashan/qunxianguan",
                   "id"        : "yue buqun" ]),
        }));

        create_family("��ɽ��", 14, "����");
        setup();

        carry_object("/clone/weapon/changjian")->wield();
        carry_object("/clone/cloth/cloth")->wear();
}

int permit_pass(object me, string dir)
{
        object *inv;
        int i;

        if (! living(this_object()))
                return 1;

        if( query("family/family_name", me) == "��ɽ����" && 
            dir == "southup")
        {
                if( query_temp("beg_huashan", me) )
                {
                        command("say ��ͷ�ǰ�����ͷ�ǰ�����");
                        return 1;
                }

                message_vision("$NȰ������ʦ�н���������إ����"
                               "ǰ���뿪��ɽ�ˣ����ϲ��ǻ�ɽ�����ˡ�\n"
                               "�������Ϻ��ˣ���λ" + RANK_D->query_respect(me) +
                               "������ذɡ�\n", this_object());
                return -1;
        }


        notify_fail("����" + name() + "�����������ȥ��\n");

        inv = deep_inventory(me);
        for (i = 0; i < sizeof(inv); i++)
        {
                if (! playerp(inv[i])) continue;
                if( query("family/family_name", inv[i]) != "��ɽ��" )
                {
                        message_vision("$N��$n�ȵ����㱳����˭�����������£�\n",
                                       this_object(), me);
                        return 0;
                }
        }

        if( query("family/family_name", me) && 
            query("family/family_name", me) != "��ɽ��" || 
            !query("family/family_name", me) && 
            query("combat_exp", me)>20000 )
        {
                message_vision("$N��$n�����Բ��𣬲������ǻ�ɽ�ɵ���"
                               "�����뱾���صء�\n", this_object(), me);
                return 0;
        }

        return 1;
}

void relay_emote(object me, string verb)
{
        if (verb != "beg")
        {
                switch (random(5))
                {
                case 0:
                        command("bored");
                        break;

                case 1:
                        command("shzi");
                        break;

                case 2:
                        command("@@");
                        break;

                case 3:
                        command("cold");
                        break;

                default:
                        command("heihei");
                        break;
                }
                return;
        }

        if( query("family/family_name", me) != "��ɽ����" )
        {
                command("poor"+query("id", me));
                return;
        }

        command("sigh");
        command("say ��λ" + RANK_D->query_respect(me) +
                "�����˽��ڣ�����˰ɣ���Ҫ������ͷ�ǰ��");
        set_temp("beg_huashan", 1, me);
}