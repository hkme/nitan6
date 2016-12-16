//liangyi-jian.c ���ǽ���
// Last Modified by sir 10.22.2001

#include <ansi.h>;
inherit SKILL;
string type() { return "martial"; }
string martialtype() { return "skill"; }

mapping *action = ({
([      "skill_name" : "��������",
        "action"     : "$N���⽣â������һ��"HIC"���������ء�"NOR"������$w�����´󿪴��أ�һ��б�ϴ���$n��$l",
        "lvl"        : 0
]),
([      "skill_name" : "��������",
        "action"     : "$N����Ȧת������$w��ճ������ƽƽչչ�ӳ���һ��" HIR"������������"NOR"���Ữ��$n��$l",
        "lvl"        : 10
]),
([      "skill_name" : "Ԩ����ǳ",
        "action"     : "$N����������������������������$wʹ��һʽ" HIB"��Ԩ����ǳ��"NOR"����$n��$l",
        "lvl"        : 20
]),
([      "skill_name" : "ˮ�����",
        "action"     : "$N������ָ��������ת��һ��"HIG"��ˮ����ơ�"NOR"ֱȡ$n��$l",
        "lvl"        : 30
]),
([      "skill_name" : "�ƺ�����",
        "action"     : "$N��â���£��������ߣ�����$wʹ��һʽ"HIW"���ƺ����¡�"NOR"�����Ƽ����������$n��$l",
        "lvl"        : 40
]),
([      "skill_name" : "���ѵ�ׯ",
        "action"     : "$N�����ƽ���������ʵ��׷ɣ��ó�����ǹ⣬����$wʹ��һʽ"HIG"�����ѵ�ׯ��"NOR"ԾԾ����Ʈ��$n��$l",
        "lvl"        : 50
]),
([      "skill_name" : "��������",
        "action"     : "$N�ӽ��ֻ�����������ǰԾ��������$wһʽ"HIM"���������ǡ�"NOR"����������֮�ƣ�����$n��$l",
        "lvl"        : 60
]),
([      "skill_name" : "��ϵ�̳�",
        "action"     : "$N�˲������ֽ�ָ��ת������һŤ������$wһ��" GRN"����ϵ�̳���"NOR"���¶��ϴ���$n��$l",
        "lvl"        : 80
]),
});

int valid_enable(string usage){return (usage== "sword") || (usage== "parry");}
int valid_learn(object me)
{
        if( query("max_neili", me)<200 )
                return notify_fail("�������������\n");

        if ((int)me->query_skill("sword", 1) < (int)me->query_skill("liangyi-jian", 1) )
                return notify_fail("��Ļ����������̫ǳ��\n");
        return 1;
}
int practice_skill(object me)
{
        object weapon;

        if( !objectp(weapon=query_temp("weapon", me) )
                 || query("skill_type", weapon) != "sword" )
                return notify_fail("��ʹ�õ��������ԡ�\n");
        if( query("qi", me)<60 )
                return notify_fail("����������������ǽ�����\n");
        if( query("neili", me)<45 )
                return notify_fail("������������������ǽ�����\n");
        me->receive_damage("qi", 55);
        addn("neili", -40, me);
        return 1;
}
string query_skill_name(int level)
{
        int i;
        for(i = sizeof(action); i > 0; i--)
                if(level >= action[i-1]["lvl"])
                        return action[i-1]["skill_name"];
}
mapping query_action(object me, object weapon)
{
/* d_e=dodge_effect p_e=parry_effect f_e=force_effect m_e=damage_effect */
        int d_e1 = -25;
        int d_e2 = 0;
        int p_e1 = -50;
        int p_e2 = -35;
        int f_e1 = 150;
        int f_e2 = 200;
        int m_e1 = 200;
        int m_e2 = 300;
        int i, lvl, seq, ttl = sizeof(action);

        lvl = (int) me->query_skill("liangyi-jian", 1);
        if( random(lvl)>240 && me->query_skill("force")>200 && query("neili", me) >= 500 )
         {
        return ([
        "action":HIC"$N��ɫ���أ�����$w"NOR+HIY"һ�٣�������ת��$w"+HIY"�Ͻ�âͻʢ��
                   ����Ĭ������籩���ش���$n��$l��"NOR,
        "force" : 500,
        "dodge" : -140,
        "parry" : -140,
        "damage": 900,
        "weapon": HIY"���"NOR,
        "damage_type":  "����"
        ]);
        }
        for(i = ttl; i > 0; i--)
                if(lvl > action[i-1]["lvl"])
                {
                        seq = i; /* �������������� */
                        break;
                }
        seq = random(seq);       /* ѡ������������ */
        return ([
                "action"      : action[seq]["action"],
                "dodge"       : d_e1 + (d_e2 - d_e1) * seq / ttl,
                "parry"       : p_e1 + (p_e2 - p_e1) * seq / ttl,
                "force"       : f_e1 + (f_e2 - f_e1) * seq / ttl,
                "damage"      : m_e1 + (m_e2 - m_e1) * seq / ttl,
                "damage_type" : random(2) ? "����" : "����",
        ]);
}
int learn_bonus() { return 0; }
int practice_bonus() { return 0; }
int success() { return 5; }
int power_point(object me) { return 1.0; }

string perform_action_file(string action)
{
        return __DIR__"liangyi-jian/" + action;
}
mixed hit_ob(object me, object victim, int damage_bonus)
{
       string msg;
       int j, k;
       j = me->query_skill("liangyi-jian", 1);
       k = me->query_skill("qinqi-shuhua",1);
       if( random(10) >= 5
        && !me->is_busy()
        && !victim->is_busy()
        && j > 120
        && k > 120
        && me->query_skill_mapped("parry") == "liangyi-jian"
        && j > random(victim->query_skill("dodge",1))){
          switch(random(3)){
            case 0 :
               msg = HIW"$Nʩչ����·�����ƺƵ������Ϭ���ޱȣ�\n"NOR;
               msg+= HIW"$n�������ˣ�ȫ���мܣ��������У�\n"NOR;
               victim->start_busy(3);
               message_vision(msg, me, victim);
               COMBAT_D->do_attack(me,victim,query_temp("weapon", me),2);
               break;
            case 1 :
               msg = HIW"$N����$n�ɿ����ת��һ�������̳��ü�ʮ����\n"NOR;
               msg+= HIW"$n�����ֲ������һ�����飬��һ����ʵ������ͣ�¹��ƣ������Ż���\n"NOR;
               victim->start_busy(2);
               message_vision(msg, me, victim);
               COMBAT_D->do_attack(me,victim,query_temp("weapon", me),0);
               break;
            case 2 :
               msg = HIW"$N��۵糸������$n���ܣ�������������ֱ�����㲻��صķ���һ�㣡\n"NOR;
               msg+= HIW"$n����$Nת��תȥ����Ū���ۻ����ң�ҡҡ��׹��\n"NOR;
               victim->start_busy(3);
               message_vision(msg, me, victim);
               COMBAT_D->do_attack(me,victim,query_temp("weapon", me),1);
               break;
          }
       }
}

int help(object me)
{
        write(HIC"\n���ǽ�����"NOR"\n");
        write(@HELP

    ̫�������ǡ�
    �䵱����λ����������ʮ����Ѫ���������ǽ���������������
������������ᣬ������һ��������ٱ��

        ѧϰҪ��
                ̫����50��
                ����200
                �����������������ǽ���
HELP
        );
        return 1;
}