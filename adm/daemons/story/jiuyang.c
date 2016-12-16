// story:jiuyang ������

#include <ansi.h>

nosave string char_id;
nosave string char_name;

int give_gift();
object select_character();

nosave mixed *story = ({
        "һ�ѷ𾭡�",
        "$N����һ��ϸϸ�Ŀ��˰��죬ȴ����Ҫ�죬���������һ���𾭣�û�а���书���ϡ�",
        "$N��˼Ƭ�̣����鷴�˹��������������ֶ�������û��ʲô���",
        "ʢŭ֮�£�$N�����������˿�ȥ���ּ���һ����ȴ�ǡ���վ�����",
        "�ж�Ƭ�̣�����һ�ѷϻ���",
        "�����д����������µĲؾ���ͷ�����Щ��������$N���а�������Ī������ɮ����ʵ�������书����",
        "����涪�������Ѿ����˼�ʮ�����飬û���ҵ����ͷ����",
        "$N���ɵð����п࣬���˿��������ŵĻ���͵�һ���ˣ����룺��δ����ؾ���ʵ���Ǹ�ð���գ���Ȼ...",
        "һʱ��$N���ɵ���ͷ��ŭ�������еľ���˺�˸����飬��ʱֽƬ���",
        "��Ȼ������ֽƬƮ��$N����ǰ���ƺ�д�ţ������˽裬���ɼ������������ɼ���...",
        "$N��ʶ�����������⼸�����е�ʱһ����������Ǽ��ϳ˵���ѧ���ϣ�",
        "$N���˿�����ʣ�µİ�����飬��Ȼ��һ��������٤������ֻ���з������������д��һЩ���֡�",
        "ֻ�Ǿ����Ѿ�ȱ��һ�룬����Ǻã�$N��æ�ڵ�������Ѱ�ң�����ֽһƬƬѰ�ء�",
        "������ƬϤ���Ѻã���Ȼ��������ʱ����$Nϸϸ������ԭ���⡶��٤�����з��о�Ȼ¼��һ�ݾ����澭��",
        "$N���°�����ĥ�����껪ɽ�۽���Ҳ����������һ�������澭���ѣ������ҵ��˴��飬������������",
        "$N�漴����̨�ý���������Ħ�澭Ҫּ��ֻ���澭�����һʱδ��ȫȻ����",
        "�ۼ���ɫ������$N���Ҿ����������飬�����룬�ְѾ��Ŀ�ͷһ�γ���˺�飬����պá�",
        "$N���������һ�ȥϸϸ�ж�������澭�����������޵У������գ�",
        "��գ�$NԾ����ȥ����ȻƮʧ��ҹĻ�С�",
        (: give_gift :),
});

void create()
{
        seteuid(getuid());

        if (! objectp(select_character()))
        {
                STORY_D->remove_story("jiuyang");
                destruct(this_object());
                return;
        }
}

string prompt() { return HIG "������������" NOR; }

object select_character()
{
        object *obs;
        object ob;

        obs = filter_array(all_interactive(),
                           (: living($1) &&
                              $1->query_skill("jiuyang-shengong", 1) < 1 &&
/*
                              query("combat_exp", $1) >= 100000 && 
                              query("combat_exp", $1)<1500000 && 
*/
                              SKILL_D("jiuyang-shengong")->valid_learn($1) &&
                              ! wizardp($1) &&
                              !query("story/jiuyang", $1):));
        if (! sizeof(obs))
                return 0;

        ob = obs[random(sizeof(obs))];
        char_id=query("id", ob);
        char_name = ob->name(1);
        return ob;
}

mixed query_story_message(int step)
{
        mixed msg;

        if (step >= sizeof(story))
                return 0;

        msg = story[step];
        if (stringp(msg))
        {
                msg = replace_string(msg, "$N", char_name);
                msg = replace_string(msg, "$ID", char_id);
        }
        return msg;
}

int give_gift()
{
        object ob;
        object book;

        ob = find_player(char_id);
        if (! ob) return 1;

        if (ob->query_skill("jiuyang-shengong", 1) < 50)
                ob->set_skill("jiuyang-shengong", 50);

        set("story/jiuyang", 1, ob);
        tell_object(ob, HIC "��ѧϰ���˾����񹦡�\n" NOR);
        CHANNEL_D->do_channel(this_object(), "rumor", "��˵������"
                              "�ؾ���ʧ�ԣ���ʧ��٤��һ����");
        book = new("/clone/book/jiuyang-book");
        book->move(ob, 1);
        STORY_D->remove_story("jiuyang");
        return 1;
}